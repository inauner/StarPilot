#!/usr/bin/env python3
import math
import time
import json
import base64
import uuid
import re
import requests
from cereal import messaging
from openpilot.common.params import Params
from openpilot.starpilot.system.uniden_shm import set_shm_param, get_shm_param
from openpilot.starpilot.system.waze import waze_pb2

WAZE_RT_HOST = "rt-xlb-am.waze.com"
APP_VERSION = "5.17.1.0"
PROTOCOL_VERSION = 234

# Thresholds requested by user:
# 1. At least 2 driver confirmations (thumbs-up)
# 2. Reported or confirmed within the last 15 minutes (900 seconds)
WAZE_MIN_THUMBS_UP = 2
WAZE_MAX_AGE_SEC = 900.0  # 15 minutes

def haversine_miles(lat1, lon1, lat2, lon2):
    R = 3958.8  # Earth radius in miles
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = (math.sin(dlat / 2) ** 2 +
         math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) *
         math.sin(dlon / 2) ** 2)
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c

def calculate_bearing(lat1, lon1, lat2, lon2):
    lat1, lon1 = math.radians(lat1), math.radians(lon1)
    lat2, lon2 = math.radians(lat2), math.radians(lon2)
    dlon = lon2 - lon1
    y = math.sin(dlon) * math.cos(lat2)
    x = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(dlon)
    initial_bearing = math.atan2(y, x)
    initial_bearing = math.degrees(initial_bearing)
    return (initial_bearing + 360) % 360

def calculate_cross_track_feet(lat1, lon1, bearing1_deg, lat2, lon2):
    """
    Computes cross-track lateral error (feet) of point (lat2, lon2)
    relative to the great-circle path from (lat1, lon1) with heading bearing1_deg.
    """
    R_feet = 3958.8 * 5280.0
    d_dist = haversine_miles(lat1, lon1, lat2, lon2) * 5280.0
    if d_dist == 0:
        return 0.0
    brng_to_target = calculate_bearing(lat1, lon1, lat2, lon2)
    delta_angle = math.radians(brng_to_target - bearing1_deg)
    d_angular = d_dist / R_feet
    xt = math.asin(math.sin(d_angular) * math.sin(delta_angle)) * R_feet
    return abs(xt)

def extract_highway_route_keys(name_str):
    """
    Extracts canonical highway route tokens from a road string (e.g. 'I-280 S', 'Interstate 280', 'CA 85 N', 'US-101').
    Returns a set of canonical route strings like {'I-280', '280'} or {'CA-85', '85'}.
    """
    if not name_str:
        return set()
    tokens = set()
    s = str(name_str).upper()

    # Interstate matching: I-280, Interstate 280, I 280
    for m in re.finditer(r"(?:INTERSTATE\s+|I[- ]?)(\d+)", s):
        num = m.group(1)
        tokens.add(f"I-{num}")
        tokens.add(num)

    # US Highway: US-101, US Highway 101, US 101, Hwy 101
    for m in re.finditer(r"(?:US\s*(?:HIGHWAY|HWY)?[- ]?|HIGHWAY\s+|HWY\s*)(\d+)", s):
        num = m.group(1)
        tokens.add(f"US-{num}")
        tokens.add(num)

    # State Route / California / State Hwy: CA-85, CA 85, SR-85, State Route 85, Route 85
    for m in re.finditer(r"(?:CA[- ]?|SR[- ]?|STATE\s*ROUTE\s*|STATE\s*HWY\s*|ROUTE\s*)(\d+)", s):
        num = m.group(1)
        tokens.add(f"CA-{num}")
        tokens.add(num)

    return tokens

def normalize_street_name(name_str):
    """
    Normalizes a street name by stripping directions, suffixes, punctuation and casing.
    e.g. 'South Bascom Avenue' -> 'bascom'
    e.g. 'Stevens Creek Blvd' -> 'stevens creek'
    """
    if not name_str:
        return ""
    s = name_str.lower()
    # Remove city/state suffixes if present: 'Bascom Ave, San Jose' -> 'Bascom Ave'
    if "," in s:
        s = s.split(",")[0]
    # Remove 'to ...' exit descriptions: 'to Leigh Ave / Bascom Ave' -> 'Leigh Ave / Bascom Ave'
    if s.startswith("to "):
        s = s[3:]
    # Remove exit numbers: 'Exit 22: CA-85' -> 'CA-85'
    s = re.sub(r"exit\s+\w+:\s*", "", s)
    # Remove compass directions
    s = re.sub(r"\b(north|south|east|west|nb|sb|eb|wb|[nsew])\b", "", s)
    # Remove standard road suffixes
    s = re.sub(r"\b(avenue|ave|street|st|boulevard|blvd|road|rd|parkway|pkwy|expressway|expwy|drive|dr|lane|ln|way|court|ct|highway|hwy|route|rt)\b", "", s)
    # Remove non-alphanumeric characters
    s = re.sub(r"[^a-z0-9\s]", " ", s)
    # Collapse multiple whitespace
    s = re.sub(r"\s+", " ", s).strip()
    return s

def is_same_road(current_road, incident_location, current_lat=0.0, current_lon=0.0, current_bearing=0.0, inc_lat=0.0, inc_lon=0.0):
    """
    Checks whether an incident is on the same road as current_road using a 3-tier check:
    1. Highway route number match (I-280, CA-85, US-101, etc.)
    2. Normalized surface street match
    3. Lateral cross-track geometric distance fallback (within ~150 ft of heading line)
    """
    curr = (current_road or "").strip()
    loc = (incident_location or "").strip()

    # If current road name is unknown or empty, fallback to cross-track lateral error
    if not curr:
        if current_lat != 0.0 and inc_lat != 0.0:
            xt_feet = calculate_cross_track_feet(current_lat, current_lon, current_bearing, inc_lat, inc_lon)
            return xt_feet <= 150.0  # within 150 feet of vehicle trajectory
        return True

    # Tier 1: Canonical Highway / Route match
    curr_hwys = extract_highway_route_keys(curr)
    loc_hwys = extract_highway_route_keys(loc)
    if curr_hwys and loc_hwys:
        if not curr_hwys.isdisjoint(loc_hwys):
            return True

    # Tier 2: Normalized Street Name match
    norm_curr = normalize_street_name(curr)
    norm_loc = normalize_street_name(loc)
    if norm_curr and norm_loc:
        curr_words = set(norm_curr.split())
        loc_words = set(norm_loc.split())
        # Filter out very short generic words
        meaningful_curr = {w for w in curr_words if len(w) >= 3}
        meaningful_loc = {w for w in loc_words if len(w) >= 3}
        if meaningful_curr and meaningful_loc and not meaningful_curr.isdisjoint(meaningful_loc):
            return True

    # Tier 3: Lateral geometric fallback (when on highway or ramp where name might differ)
    if current_lat != 0.0 and inc_lat != 0.0:
        xt_feet = calculate_cross_track_feet(current_lat, current_lon, current_bearing, inc_lat, inc_lon)
        if xt_feet <= 100.0:
            return True

    return False

def extract_lane_info(subtype_val, subtype_name, desc_str, label_str):
    """
    Extracts high-value highway lane placement context from alert subtype enum,
    description, or report label.
    Distinguishes between which lane is BLOCKED/CLOSED vs instruction to MOVE.
    """
    s = f"{subtype_name} {desc_str} {label_str}".lower()
    
    # 1. Direct protobuf enum matching (ground truth from Waze reporting buttons)
    if subtype_val == 2003 or "left_lane" in subtype_name.lower():
        return "Left Lane Closed"
    if subtype_val == 2004 or "right_lane" in subtype_name.lower():
        return "Right Lane Closed"
    if subtype_val == 2005 or "center_lane" in subtype_name.lower():
        return "Center Lane Closed"

    # 2. Maneuver instructions (e.g. "Road Work Ahead - move LEFT" means Right Lane is blocked!)
    if "move left" in s or "merge left" in s:
        return "Move Left (Right Blocked)"
    if "move right" in s or "merge right" in s:
        return "Move Right (Left Blocked)"

    # 3. Explicit lane blockage mentions
    if "left lane" in s or "in left" in s or "on left" in s:
        return "Left Lane"
    if "right lane" in s or "in right" in s or "on right" in s:
        return "Right Lane"
    if "center lane" in s or "middle lane" in s:
        return "Center Lane"

    # 4. Shoulder matching
    if "left shoulder" in s:
        return "Left Shoulder"
    if "right shoulder" in s:
        return "Right Shoulder"
    if "shoulder" in s:
        return "Shoulder"

    # 5. General Lane matching
    if "in lane" in s or "blocked lane" in s or "lane closed" in s:
        return "In Lane"

    return ""

class WazeSessionManager:
    def __init__(self):
        self.session = requests.Session()
        self.session_id = get_shm_param("WazeSessionId", None)
        self.secret_key = get_shm_param("WazeSecretKey", None)
        self.username = get_shm_param("WazeUsername", None)
        self.password = get_shm_param("WazePassword", None)
        self.seq = 1
        self.device_uuid = str(uuid.uuid4())
        self.last_login_attempt = 0.0
        self.login_backoff_sec = 15.0

    def _next_seq(self):
        s = str(self.seq)
        self.seq += 1
        return s

    def _proto_base64_line(self, element):
        batch = waze_pb2.Batch()
        batch.element.extend([element])
        data = batch.SerializeToString()
        b64 = base64.b64encode(data).decode("ascii")
        return f"ProtoBase64,{b64}"

    def register_and_login(self, lat=37.7749, lon=-122.4194, force=False):
        if not force and (time.monotonic() - self.last_login_attempt < self.login_backoff_sec):
            return False

        self.last_login_attempt = time.monotonic()
        set_shm_param("WazeAuthStatus", "Registering guest session...")
        if force:
            self.username = None
            self.password = None
            self.session_id = None
            self.secret_key = None
            self.server_session_id = None
            set_shm_param("WazeSessionId", "")
            set_shm_param("WazeSecretKey", "")

        try:
            self.device_uuid = str(uuid.uuid4())
            element_ci = waze_pb2.Element()
            ci = element_ci.client_info
            ci.protocol = PROTOCOL_VERSION
            ci.client_version = APP_VERSION
            ci.last_position.lon_times1000000 = int(round(lon * 1_000_000))
            ci.last_position.lat_times1000000 = int(round(lat * 1_000_000))
            ci.manufacturer = "Google"
            ci.model = "Pixel"
            ci.os_version = "14"
            ci.locale = "en"
            ci.installation_id = self.device_uuid
            ci.device_type = waze_pb2.DeviceType.ANDROID_DEVICE
            ci.app_type = waze_pb2.AppType.WAZE
            ci.os_language_id = "en"
            ci.session_uuid = str(uuid.uuid4())
            ci.current_time_millis = int(time.time() * 1000)
            ci.app_flavor = waze_pb2.AppFlavor.ALPHA

            element_reg = waze_pb2.Element()
            element_reg.register.SetInParent()

            body = self._proto_base64_line(element_ci) + "\n" + self._proto_base64_line(element_reg)
            headers = {
                "User-Agent": APP_VERSION,
                "x-waze-network-version": "3",
                "sequence-number": self._next_seq(),
                "Content-Type": "binary/octet-stream"
            }

            url = f"https://{WAZE_RT_HOST}/rtserver/distrib/static"
            r = self.session.post(url, data=body.encode("utf-8"), headers=headers, timeout=10)
            if r.status_code == 200:
                batch = waze_pb2.Batch()
                batch.ParseFromString(r.content)
                for el in batch.element:
                    if el.HasField("register_successful"):
                        self.username = el.register_successful.username
                        self.password = el.register_successful.password
                        set_shm_param("WazeUsername", self.username)
                        set_shm_param("WazePassword", self.password)
                        self.login_backoff_sec = 15.0
                        break

            if not self.username or not self.password:
                set_shm_param("WazeAuthStatus", "Rate limited by Waze (429). Use token injection or wait cooldown.")
                self.login_backoff_sec = min(self.login_backoff_sec * 1.5, 300.0)
                return False

            element_login = waze_pb2.Element()
            lr = element_login.login_request
            lr.password_credential.username = self.username
            lr.password_credential.password = self.password
            lr.reason = waze_pb2.LoginRequest.LoginReason.NORMAL

            element_ads = waze_pb2.Element()
            element_ads.report_ads_setting.SetInParent()

            body_login = (
                self._proto_base64_line(element_ci) + "\n"
                + self._proto_base64_line(element_login) + "\n"
                + self._proto_base64_line(element_ads)
            )
            headers_login = {
                "User-Agent": APP_VERSION,
                "x-waze-network-version": "3",
                "sequence-number": self._next_seq(),
                "x-waze-wait-timeout": "8500",
                "Content-Type": "binary/octet-stream"
            }

            r_login = self.session.post(url, data=body_login.encode("utf-8"), headers=headers_login, timeout=12)
            if r_login.status_code == 200:
                batch_res = waze_pb2.Batch()
                batch_res.ParseFromString(r_login.content)
                for el in batch_res.element:
                    if el.HasField("login_response"):
                        resp = el.login_response
                        if resp.HasField("login_success"):
                            self.server_session_id = resp.login_success.server_session_id
                            self.session_id = resp.login_success.session_id
                            self.secret_key = resp.login_success.secret_key
                            set_shm_param("WazeSessionId", str(self.server_session_id or self.session_id))
                            set_shm_param("WazeSecretKey", str(self.secret_key))
                            set_shm_param("WazeAuthStatus", f"Guest Active (ID: {self.server_session_id or self.session_id})")
                            return True
                        elif resp.HasField("login_error"):
                            err_msg = str(resp.login_error)
                            set_shm_param("WazeAuthStatus", f"Login Error: {err_msg}")
                            self.username = None
                            self.password = None
                            self.session_id = None
                            self.secret_key = None
                            set_shm_param("WazeSessionId", "")
                            set_shm_param("WazeSecretKey", "")
                            return False

            set_shm_param("WazeAuthStatus", "Login challenge failed. Retrying...")
            self.username = None
            self.password = None
            set_shm_param("WazeSessionId", "")
            set_shm_param("WazeSecretKey", "")
            return False
        except Exception as e:
            set_shm_param("WazeAuthStatus", f"Error: {e}")
            return False

    def query(self, lat, lon, box_radius_deg=0.08):
        # Reload from shared memory if manually injected via UI
        saved_session = get_shm_param("WazeSessionId", "")
        saved_key = get_shm_param("WazeSecretKey", "")
        if saved_session and saved_key:
            try:
                self.session_id = int(saved_session) if str(saved_session).isdigit() else saved_session
                self.secret_key = int(saved_key) if str(saved_key).isdigit() else saved_key
            except Exception:
                pass

        if not self.session_id or not self.secret_key:
            if not self.register_and_login(lat, lon):
                return []

        lon_min = lon - box_radius_deg
        lon_max = lon + box_radius_deg
        lat_min = lat - (box_radius_deg * 0.8)
        lat_max = lat + (box_radius_deg * 0.8)
        mid_lon = (lon_min + lon_max) / 2.0
        mid_lat = (lat_min + lat_max) / 2.0

        cmd_map = (
            f"MapDisplayed,{lon_min:.6f},{lat_max:.6f},{lon_max:.6f},{lat_max:.6f},"
            f"{lon_max:.6f},{lat_min:.6f},{lon_min:.6f},{lat_min:.6f},"
            f"{mid_lon:.6f},{mid_lat:.6f},20000,"
            f"{lon_min:.6f},{lat_max:.6f},{lon_max:.6f},{lat_max:.6f},"
            f"{lon_max:.6f},{lat_min:.6f},{lon_min:.6f},{lat_min:.6f}"
        )
        body = f"SeeMe,1,2,T,T,T,1,-1,1,7\nSetMood,1\nLocation,{lon:.6f},{lat:.6f}\n{cmd_map}"

        uid = waze_pb2.UID()
        try:
            uid.id = int(getattr(self, "server_session_id", None) or self.session_id)
        except Exception:
            uid.id = 0
        uid.secret_key = str(self.secret_key)
        uid_hdr = base64.b64encode(uid.SerializeToString()).decode("ascii")

        headers = {
            "User-Agent": APP_VERSION,
            "x-waze-network-version": "3",
            "sequence-number": self._next_seq(),
            "x-waze-wait-timeout": "10500",
            "uid": uid_hdr,
            "Content-Type": "binary/octet-stream"
        }

        try:
            url = f"https://{WAZE_RT_HOST}/rtserver/distrib/command"
            r = self.session.post(url, data=body.encode("utf-8"), headers=headers, timeout=15)
            if r.status_code != 200:
                self.session_id = None
                if self.register_and_login(lat, lon, force=True):
                    return self.query(lat, lon, box_radius_deg=box_radius_deg)
                return []

            batch = waze_pb2.Batch()
            batch.ParseFromString(r.content)
            alerts = []
            now_epoch = time.time()

            geoms = {}
            road_infos = {}

            for el in batch.element:
                if el.HasField("old_command"):
                    parts = el.old_command.split(",")
                    if parts[0] == "RoadInfoGeom" and len(parts) >= 5:
                        try:
                            road_id = parts[1]
                            lon_c = int(parts[3]) / 1_000_000.0
                            lat_c = int(parts[4]) / 1_000_000.0
                            geoms[road_id] = (lat_c, lon_c)
                        except Exception:
                            pass
                    elif parts[0] == "AddRoadInfo" and len(parts) >= 7:
                        road_id = parts[1]
                        road_infos[road_id] = parts
                elif el.HasField("add_alert_action"):
                    ra = el.add_alert_action.realtime_alert
                    alert_type = ra.alert_info.type
                    alert_lat = ra.alert_info.position.lat_times1000000 / 1_000_000.0
                    alert_lon = ra.alert_info.position.lon_times1000000 / 1_000_000.0
                    
                    street = ra.alert_reporting_info.alert_address.street if ra.alert_reporting_info.HasField("alert_address") else ""
                    city = ra.alert_reporting_info.alert_address.city if ra.alert_reporting_info.HasField("alert_address") else ""
                    thumbs = int(ra.alert_reporting_info.thumbs_up_count or 0)
                    
                    # Report timestamp (seconds or milliseconds)
                    report_time_raw = ra.alert_reporting_info.report_time
                    report_time_sec = (report_time_raw / 1000.0) if report_time_raw > 1_000_000_000_000 else float(report_time_raw)
                    age_seconds = (now_epoch - report_time_sec) if report_time_sec > 0 else 0.0

                    alert_subtype = ra.alert_info.sub_type if ra.alert_info.HasField("sub_type") else waze_pb2.AlertSubType.NO_SUBTYPE
                    subtype_name = waze_pb2.AlertSubType.Name(alert_subtype) if alert_subtype in waze_pb2.AlertSubType.values() else ""

                    desc_text = ra.alert_reporting_info.description if ra.alert_reporting_info.HasField("description") else ""

                    category = "HAZARD"
                    label = "Road Hazard"
                    icon = "⚠️"
                    is_verified_police = False

                    # 1. Police Traps & Cameras
                    if alert_type == waze_pb2.AlertType.POLICE or "police" in subtype_name.lower():
                        category = "POLICE"
                        if alert_subtype == waze_pb2.AlertSubType.POLICE_HIDING:
                            label = "Police Hidden (Speed Trap)"
                            is_verified_police = True
                        elif alert_subtype == waze_pb2.AlertSubType.POLICE_VISIBLE:
                            label = "Police Visible"
                            is_verified_police = True
                        elif alert_subtype in (waze_pb2.AlertSubType.POLICE_WITH_MOBILE_CAMERA, waze_pb2.AlertSubType.DEFAULT_CAMERA):
                            label = "Police Speed Camera"
                            is_verified_police = True
                        else:
                            label = "Police Reported"
                        icon = "🚨"

                    # 2. Accidents & Collisions
                    elif alert_type == waze_pb2.AlertType.ACCIDENT or "accident" in subtype_name.lower():
                        if alert_subtype == waze_pb2.AlertSubType.ACCIDENT_MAJOR or "major" in desc_text.lower():
                            category = "ACCIDENT"
                            label = "Major Accident"
                            icon = "💥"
                        elif alert_subtype == waze_pb2.AlertSubType.ACCIDENT_MINOR or "minor" in desc_text.lower():
                            category = "ACCIDENT"
                            label = "Minor Accident"
                            icon = "🚗"
                        else:
                            category = "ACCIDENT"
                            label = "Accident Reported"
                            icon = "💥"

                    # 3. Traffic Jams & Standstills
                    elif alert_type == waze_pb2.AlertType.JAM \
                         or alert_subtype in (waze_pb2.AlertSubType.JAM_MODERATE_TRAFFIC, waze_pb2.AlertSubType.JAM_HEAVY_TRAFFIC,
                                                waze_pb2.AlertSubType.JAM_STAND_STILL_TRAFFIC, waze_pb2.AlertSubType.JAM_LIGHT_TRAFFIC) \
                         or "jam" in subtype_name.lower() or "traffic" in subtype_name.lower():
                        category = "TRAFFIC"
                        if alert_subtype == waze_pb2.AlertSubType.JAM_STAND_STILL_TRAFFIC or "stand_still" in subtype_name.lower() or "standstill" in desc_text.lower():
                            label = "Traffic Standstill Ahead"
                            icon = "🛑"
                        elif alert_subtype == waze_pb2.AlertSubType.JAM_HEAVY_TRAFFIC or "heavy" in subtype_name.lower() or "heavy" in desc_text.lower():
                            label = "Heavy Traffic Congestion"
                            icon = "🚗🚗"
                        elif alert_subtype == waze_pb2.AlertSubType.JAM_MODERATE_TRAFFIC or "moderate" in subtype_name.lower():
                            label = "Moderate Traffic Slowdown"
                            icon = "🚙"
                        else:
                            label = "Traffic Slowdown Ahead"
                            icon = "🚗"

                    # 4. Full Road & Turn Closures
                    elif alert_type in (waze_pb2.AlertType.ROAD_CLOSED, waze_pb2.AlertType.SYSTEM_ROAD_CLOSED, waze_pb2.AlertType.TURN_CLOSED) \
                         or alert_subtype in (waze_pb2.AlertSubType.ROAD_CLOSED_HAZARD, waze_pb2.AlertSubType.ROAD_CLOSED_EVENT, waze_pb2.AlertSubType.TURN_CLOSED_EVENT):
                        category = "CLOSURE"
                        label = "Road Closed"
                        icon = "⛔"

                    # 4. Lane Closures & Construction
                    elif alert_subtype in (waze_pb2.AlertSubType.HAZARD_ON_ROAD_LANE_CLOSED, waze_pb2.AlertSubType.LANE_CLOSURE_BLOCKED_LANES,
                                           waze_pb2.AlertSubType.LANE_CLOSURE_LEFT_LANE, waze_pb2.AlertSubType.LANE_CLOSURE_RIGHT_LANE,
                                           waze_pb2.AlertSubType.LANE_CLOSURE_CENTER_LANE, waze_pb2.AlertSubType.HAZARD_ON_ROAD_CONSTRUCTION,
                                           waze_pb2.AlertSubType.ROAD_CLOSED_CONSTRUCTION) \
                         or alert_type in (waze_pb2.AlertType.CONSTRUCTION, waze_pb2.AlertType.NEW_LANE_CLOSED) \
                         or "lane_closed" in subtype_name.lower() or "construction" in subtype_name.lower() or "construction" in desc_text.lower():
                        category = "LANE_CLOSURE"
                        label = "Lane Closed" if ("lane" in subtype_name.lower() or "lane" in desc_text.lower()) else "Road Work / Construction"
                        icon = "🚧"

                    # 5. Hazards On Road (Active Traffic Lane)
                    elif alert_subtype in (waze_pb2.AlertSubType.HAZARD_ON_ROAD_CAR_STOPPED, waze_pb2.AlertSubType.HAZARD_ON_ROAD_EMERGENCY_VEHICLE,
                                           waze_pb2.AlertSubType.HAZARD_ON_ROAD_OBJECT, waze_pb2.AlertSubType.HAZARD_ON_ROAD_POT_HOLE,
                                           waze_pb2.AlertSubType.HAZARD_ON_ROAD_ROAD_KILL, waze_pb2.AlertSubType.HAZARD_ON_ROAD_OIL,
                                           waze_pb2.AlertSubType.HAZARD_ON_ROAD_ICE, waze_pb2.AlertSubType.HAZARD_ON_ROAD_TRAFFIC_LIGHT_FAULT,
                                           waze_pb2.AlertSubType.HAZARD_ON_ROAD) \
                         or "on_road" in subtype_name.lower():
                        category = "HAZARD_ON_ROAD"
                        if alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_ROAD_CAR_STOPPED:
                            label = "Stopped Car in Lane"
                            icon = "🚗"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_ROAD_EMERGENCY_VEHICLE:
                            label = "Emergency Vehicle in Lane"
                            icon = "🚒"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_ROAD_POT_HOLE or "pothole" in subtype_name.lower():
                            label = "Pothole in Lane"
                            icon = "⚠️"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_ROAD_ROAD_KILL or "animal" in subtype_name.lower() or "kill" in subtype_name.lower():
                            label = "Animal / Roadkill in Lane"
                            icon = "⚠️"
                        elif alert_subtype in (waze_pb2.AlertSubType.HAZARD_ON_ROAD_OIL, waze_pb2.AlertSubType.HAZARD_ON_ROAD_ICE) or "oil" in subtype_name.lower() or "ice" in subtype_name.lower():
                            label = "Slippery Road / Oil Spill"
                            icon = "⚠️"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_ROAD_TRAFFIC_LIGHT_FAULT or "light" in subtype_name.lower():
                            label = "Traffic Light Out"
                            icon = "🚦"
                        else:
                            label = "Hazard / Object on Road"
                            icon = "⚠️"

                    # 6. Hazards On Shoulder (Off to the side / Breakdown Lane)
                    elif alert_subtype in (waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_CAR_STOPPED, waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_ANIMALS,
                                           waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_MISSING_SIGN, waze_pb2.AlertSubType.HAZARD_ON_SHOULDER) \
                         or "shoulder" in subtype_name.lower():
                        category = "HAZARD_ON_SHOULDER"
                        if alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_CAR_STOPPED:
                            label = "Stopped Vehicle on Shoulder"
                            icon = "🚗"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_ANIMALS:
                            label = "Animal on Shoulder"
                            icon = "🦌"
                        elif alert_subtype == waze_pb2.AlertSubType.HAZARD_ON_SHOULDER_MISSING_SIGN:
                            label = "Missing Sign on Shoulder"
                            icon = "⚠️"
                        else:
                            label = "Hazard on Shoulder"
                            icon = "⚠️"

                    # 7. Severe Weather & Flooding
                    elif alert_subtype in (waze_pb2.AlertSubType.HAZARD_WEATHER, waze_pb2.AlertSubType.HAZARD_WEATHER_FOG,
                                           waze_pb2.AlertSubType.HAZARD_WEATHER_HAIL, waze_pb2.AlertSubType.HAZARD_WEATHER_HEAVY_RAIN,
                                           waze_pb2.AlertSubType.HAZARD_WEATHER_HEAVY_SNOW, waze_pb2.AlertSubType.HAZARD_WEATHER_FLOOD,
                                           waze_pb2.AlertSubType.HAZARD_WEATHER_MONSOON, waze_pb2.AlertSubType.HAZARD_WEATHER_TORNADO,
                                           waze_pb2.AlertSubType.HAZARD_WEATHER_HEAT_WAVE, waze_pb2.AlertSubType.HAZARD_WEATHER_HURRICANE,
                                           waze_pb2.AlertSubType.HAZARD_WEATHER_FREEZING_RAIN, waze_pb2.AlertSubType.BAD_WEATHER_DEFAULT,
                                           waze_pb2.AlertSubType.BAD_WEATHER_SLIPPERY_ROAD) \
                         or alert_type == waze_pb2.AlertType.NEW_BAD_WEATHER \
                         or "weather" in subtype_name.lower() or "flood" in subtype_name.lower() or "fog" in subtype_name.lower() or "snow" in subtype_name.lower():
                        category = "WEATHER"
                        if "flood" in subtype_name.lower() or "flood" in desc_text.lower():
                            label = "Flooding on Roadway"
                            icon = "🌊"
                        elif "fog" in subtype_name.lower() or "fog" in desc_text.lower():
                            label = "Dense Fog Warning"
                            icon = "🌫️"
                        elif "snow" in subtype_name.lower() or "ice" in subtype_name.lower():
                            label = "Snow / Ice on Road"
                            icon = "❄️"
                        else:
                            label = "Severe Weather Alert"
                            icon = "🌧️"

                    # 8. Generic / NO_SUBTYPE Unclassified Reports
                    else:
                        category = "GENERAL"
                        label = desc_text if desc_text else "General Road Advisory"
                        icon = "ℹ️"

                    age_mins = max(1, int(round(age_seconds / 60.0))) if age_seconds > 0 else 1
                    if age_seconds <= 0 or age_mins > 720:
                        time_str = "Active"
                    elif age_mins >= 60:
                        time_str = f"{age_mins // 60}h {age_mins % 60}m ago"
                    else:
                        time_str = f"{age_mins}m ago"

                    detail_str = desc_text if desc_text else f"{thumbs} driver confirmations ({time_str})"
                    lane_str = extract_lane_info(alert_subtype, subtype_name, desc_text, label)

                    alerts.append({
                        "id": f"waze_{ra.id}",
                        "source": "Waze",
                        "category": category,
                        "label": label,
                        "lane": lane_str,
                        "icon": icon,
                        "type": subtype_name or "Waze Alert",
                        "subtype": alert_subtype,
                        "thumbs": thumbs,
                        "is_verified_police": is_verified_police,
                        "location": f"{street}, {city}" if street and city else (street or city or "Roadway"),
                        "desc": f"Waze report: {detail_str}",
                        "area": city or "Waze Community",
                        "time": time_str,
                        "age_mins": age_mins,
                        "is_active": bool(time_str == "Active"),
                        "lat": alert_lat,
                        "lon": alert_lon,
                        "detail": detail_str
                    })

            # Process live traffic jams reported via AddRoadInfo / RoadInfoGeom
            for road_id, r_parts in road_infos.items():
                if road_id in geoms:
                    lat_c, lon_c = geoms[road_id]
                    try:
                        speed_kmh = int(r_parts[2]) if r_parts[2].isdigit() else 0
                        severity = int(r_parts[3]) if r_parts[3].isdigit() else 0
                    except Exception:
                        speed_kmh = 0
                        severity = 0

                    if severity >= 2:
                        speed_mph = int(round(speed_kmh * 0.621371))
                        street = r_parts[5] if len(r_parts) > 5 else ""
                        city = r_parts[6] if len(r_parts) > 6 else ""
                        to_street = r_parts[8] if len(r_parts) > 8 else ""
                        loc_str = f"{street}, {city}" if street and city else (street or city or "Roadway")
                        if to_street:
                            loc_str += f" (to {to_street})"

                        if severity >= 5 or speed_mph <= 5:
                            label = "Traffic Standstill Ahead"
                            icon = "🛑"
                            type_name = "JAM_STAND_STILL_TRAFFIC"
                        elif severity == 4 or speed_mph <= 15:
                            label = "Severe Traffic Jam"
                            icon = "🚗🛑"
                            type_name = "JAM_HEAVY_TRAFFIC"
                        elif severity == 3 or speed_mph <= 30:
                            label = "Heavy Traffic Congestion"
                            icon = "🚗🚗"
                            type_name = "JAM_HEAVY_TRAFFIC"
                        else:
                            label = "Moderate Traffic Slowdown"
                            icon = "🚗"
                            type_name = "JAM_MODERATE_TRAFFIC"

                        detail_str = f"Flow: {speed_mph} mph (Severity {severity}/5 • Active)"
                        alerts.append({
                            "id": f"waze_traffic_{road_id}",
                            "source": "Waze",
                            "category": "TRAFFIC",
                            "label": label,
                            "icon": icon,
                            "type": type_name,
                            "subtype": severity,
                            "thumbs": 5,
                            "is_verified_police": False,
                            "location": loc_str,
                            "desc": f"Waze Live Traffic: {detail_str}",
                            "area": city or "Waze Traffic Network",
                            "time": "Active",
                            "age_mins": 0,
                            "is_active": True,
                            "lat": lat_c,
                            "lon": lon_c,
                            "detail": detail_str
                        })

            return alerts
        except Exception:
            return []

class RoadAlertsDaemon:
    def __init__(self):
        self.sm = messaging.SubMaster(["gpsLocationExternal", "livePose", "mapdOut"])
        self.waze = WazeSessionManager()
        self.waze_incidents = []
        self.last_waze_fetch = 0.0
        self.current_lat = float(get_shm_param("LastGpsLat", 0.0) or 0.0)
        self.current_lon = float(get_shm_param("LastGpsLon", 0.0) or 0.0)
        self.current_bearing = float(get_shm_param("LastGpsBearing", 0.0) or 0.0)
        self.current_road_name = str(get_shm_param("CurrentRoadName", "") or "")
        self.has_gps = bool(self.current_lat != 0.0)
        if not self.has_gps:
            self._load_fallback_gps()

    def _load_fallback_gps(self):
        try:
            raw_pos = Params().get("LastGPSPosition")
            if raw_pos:
                pos = json.loads(raw_pos)
                if pos.get("latitude") and pos.get("longitude"):
                    self.current_lat = float(pos["latitude"])
                    self.current_lon = float(pos["longitude"])
                    self.current_bearing = float(pos.get("bearing", 0.0))
                    self.has_gps = True
                    set_shm_param("LastGpsLat", self.current_lat)
                    set_shm_param("LastGpsLon", self.current_lon)
                    set_shm_param("LastGpsBearing", self.current_bearing)
        except Exception:
            pass

    def fetch_waze(self):
        self.update_gps()
        lat = self.current_lat
        lon = self.current_lon
        bearing = self.current_bearing
        if lat == 0 or lon == 0:
            return

        all_alerts = {}
        # Multi-tile corridor scan:
        if bearing != 0.0:
            scan_points = [(0.0, 0.0)]
            for d in [5, 10, 15, 20, 25, 30]:
                scan_points.append((d, 0.0))
                if d in (10, 20, 30):
                    scan_points.append((d, -25.0))
                    scan_points.append((d, 25.0))
        else:
            scan_points = [(0.0, 0.0)]
            for d in [6, 14, 22, 30]:
                for angle in [0, 60, 120, 180, 240, 300]:
                    scan_points.append((d, angle))

        for dist_mi, offset_deg in scan_points:
            eff_bearing = (bearing + offset_deg) % 360
            d_lat = (dist_mi / 69.0) * math.cos(math.radians(eff_bearing))
            d_lon = (dist_mi / (69.0 * math.cos(math.radians(lat)))) * math.sin(math.radians(eff_bearing))
            t_lat = lat + d_lat
            t_lon = lon + d_lon
            
            alerts = self.waze.query(t_lat, t_lon, box_radius_deg=0.05)
            for a in alerts:
                all_alerts[a["id"]] = a

        self.waze_incidents = list(all_alerts.values())
        self.last_waze_fetch = time.monotonic()

    def update_gps(self):
        self.sm.update(0)
        if self.sm.updated["gpsLocationExternal"]:
            gps = self.sm["gpsLocationExternal"]
            if gps.flags & 1 and gps.latitude != 0.0:  # Position valid
                self.current_lat = gps.latitude
                self.current_lon = gps.longitude
                self.current_bearing = gps.bearingDeg
                self.has_gps = True
                set_shm_param("LastGpsLat", self.current_lat)
                set_shm_param("LastGpsLon", self.current_lon)
                set_shm_param("LastGpsBearing", self.current_bearing)
        if self.sm.updated["mapdOut"]:
            mapd = self.sm["mapdOut"]
            road_name = getattr(mapd, "roadName", "") or ""
            if road_name:
                self.current_road_name = road_name
                set_shm_param("CurrentRoadName", road_name)
        if self.current_lat == 0.0:
            self._load_fallback_gps()

    def process_upcoming_alerts(self, max_radius_miles=30.0):
        if self.current_lat == 0 or self.current_lon == 0:
            return []

        # Subtype Display Filters
        show_police = get_shm_param("RoadAlertShowPolice", True)
        show_traffic = get_shm_param("RoadAlertShowTraffic", True)
        show_accidents = get_shm_param("RoadAlertShowAccidents", True)
        show_closures = get_shm_param("RoadAlertShowClosures", True)
        show_lane_closures = get_shm_param("RoadAlertShowLaneClosures", True)
        show_hazard_on_road = get_shm_param("RoadAlertShowHazardOnRoad", True)
        show_hazard_on_shoulder = get_shm_param("RoadAlertShowHazardOnShoulder", True)
        show_weather = get_shm_param("RoadAlertShowWeather", True)
        show_general = get_shm_param("RoadAlertShowGeneral", True)

        # Time & Distance Display Filters
        display_max_age = int(get_shm_param("RoadAlertDisplayMaxAgeMin", 0) or 0)
        display_max_dist = float(get_shm_param("RoadAlertDisplayMaxDistMi", 30.0) or 30.0)

        combined = getattr(self, "waze_incidents", [])
        upcoming = []
        for inc in combined:
            cat = inc.get("category", "")
            if cat == "POLICE" and not show_police:
                continue
            if cat == "TRAFFIC" and not show_traffic:
                continue
            if cat == "ACCIDENT" and not show_accidents:
                continue
            if cat == "CLOSURE" and not show_closures:
                continue
            if cat == "LANE_CLOSURE" and not show_lane_closures:
                continue
            if cat == "HAZARD_ON_ROAD" and not show_hazard_on_road:
                continue
            if cat == "HAZARD_ON_SHOULDER" and not show_hazard_on_shoulder:
                continue
            if cat == "WEATHER" and not show_weather:
                continue
            if cat == "GENERAL" and not show_general:
                continue

            # Time filter check (always include "Active" / long-term)
            age_mins = inc.get("age_mins", 0)
            is_active = inc.get("is_active", False) or (inc.get("time") == "Active")
            if display_max_age > 0 and not is_active:
                if age_mins > display_max_age:
                    continue

            dist = haversine_miles(self.current_lat, self.current_lon, inc["lat"], inc["lon"])
            effective_radius = min(max_radius_miles, display_max_dist)
            if dist <= effective_radius:
                target_bearing = calculate_bearing(self.current_lat, self.current_lon, inc["lat"], inc["lon"])
                bearing_diff = abs((target_bearing - self.current_bearing + 180) % 360 - 180)
                
                # Forward cone: if moving / heading active, check <= 80 deg. If parked / heading 0, show radius.
                is_ahead = (bearing_diff <= 80.0) if (self.current_bearing != 0.0) else True
                if is_ahead or dist <= 0.5:
                    inc_copy = dict(inc)
                    inc_copy["distance_miles"] = round(dist, 1)
                    inc_copy["bearing_diff"] = round(bearing_diff, 1)
                    upcoming.append(inc_copy)

        # Root Cause Correlation for Traffic Incidents (correlate against all raw corridor incidents)
        non_traffic = [x for x in combined if x.get("category") != "TRAFFIC"]
        cause_priority = {
            "ACCIDENT": 1,
            "HAZARD_ON_ROAD": 2,
            "LANE_CLOSURE": 3,
            "CLOSURE": 4,
            "WEATHER": 5,
            "POLICE": 6,
            "GENERAL": 7,
            "HAZARD_ON_SHOULDER": 8
        }
        for inc in upcoming:
            if inc.get("category") == "TRAFFIC":
                best_cause = None
                best_score = (99, 99.0)
                for o in non_traffic:
                    d = haversine_miles(inc["lat"], inc["lon"], o["lat"], o["lon"])
                    if d <= 2.0:
                        prio = cause_priority.get(o.get("category"), 10)
                        score = (prio, d)
                        if score < best_score:
                            best_score = score
                            best_cause = o
                if best_cause:
                    inc["cause_label"] = best_cause["label"]
                    inc["cause_icon"] = best_cause["icon"]
                    inc["cause_type"] = best_cause["type"]
                    inc["cause_location"] = best_cause["location"]
                    inc["cause_dist"] = round(best_score[1], 1)
                    inc["label"] = f"{inc['label']} (Due to: {best_cause['label']})"
                    inc["detail"] = f"Cause: {best_cause['icon']} {best_cause['label']} at {best_cause['location']} ({best_score[1]:.1f} mi) • {inc['thumbs']} confirmations"

        upcoming.sort(key=lambda x: x["distance_miles"])
        return upcoming

    def publish_to_shm(self, upcoming):
        slowdown_same_road = get_shm_param("RoadAlertSlowdownSameRoadOnly", False)

        if upcoming:
            # If Same Road Only is active, find the closest incident that matches the current road
            closest = None
            if slowdown_same_road:
                for u in upcoming:
                    if is_same_road(
                        self.current_road_name,
                        u.get("location", ""),
                        self.current_lat,
                        self.current_lon,
                        self.current_bearing,
                        u.get("lat", 0.0),
                        u.get("lon", 0.0)
                    ):
                        closest = u
                        break
            else:
                closest = upcoming[0]

            if closest:
                set_shm_param("RoadAlertActive", True)
                set_shm_param("RoadAlertCategory", closest["category"])
                set_shm_param("RoadAlertLabel", closest["label"])
                set_shm_param("RoadAlertSubtype", closest.get("type", ""))
                set_shm_param("RoadAlertLane", closest.get("lane", ""))
                set_shm_param("RoadAlertIcon", closest["icon"])
                set_shm_param("RoadAlertDistance", closest["distance_miles"])
                set_shm_param("RoadAlertLocation", closest["location"])
                set_shm_param("RoadAlertDetail", closest["detail"])
                set_shm_param("RoadAlertTime", closest["time"])
                set_shm_param("RoadAlertSource", closest["source"])
            else:
                set_shm_param("RoadAlertActive", False)
                set_shm_param("RoadAlertCategory", "")
                set_shm_param("RoadAlertLabel", "")
                set_shm_param("RoadAlertSubtype", "")
                set_shm_param("RoadAlertLane", "")
                set_shm_param("RoadAlertIcon", "")
                set_shm_param("RoadAlertDistance", 0.0)
                set_shm_param("RoadAlertLocation", "")
                set_shm_param("RoadAlertDetail", "")
                set_shm_param("RoadAlertSource", "")

            set_shm_param("RoadAlertCount", len(upcoming))
            set_shm_param("RoadAlertsList", json.dumps(upcoming))

            # 1. Waze Police Auto-Slowdown
            slowdown_police = get_shm_param("WazePoliceAutoSlowdown", True)
            slowdown_police_hidden = get_shm_param("WazePoliceSlowdownHidden", True)
            slowdown_same_road = get_shm_param("RoadAlertSlowdownSameRoadOnly", False)
            min_confirmations = int(get_shm_param("WazePoliceMinConfirmations", 3) or 3)
            trigger_distance = float(get_shm_param("WazePoliceTriggerDistance", 1.0) or 1.0)
            slowdown_max_age = int(get_shm_param("RoadAlertSlowdownMaxAgeMin", 30) or 30)

            police_active = False
            police_dist = 0.0
            if slowdown_police or slowdown_police_hidden:
                for u in upcoming:
                    if u.get("source") == "Waze" and u.get("category") == "POLICE":
                        # If ON_SHOULDER is mentioned anywhere, never slow down
                        subtype_str = str(u.get("type", "")).upper()
                        label_str = str(u.get("label", "")).lower()
                        desc_str = str(u.get("desc", "")).lower()
                        if "ON_SHOULDER" in subtype_str or "shoulder" in label_str or "shoulder" in desc_str:
                            continue

                        # Slowdown time filter check (strictly respects max report age)
                        age_mins = u.get("age_mins", 0)
                        is_active = u.get("is_active", False) or (u.get("time") == "Active")
                        if slowdown_max_age > 0 and not is_active and (age_mins > slowdown_max_age):
                            continue

                        # Distance check (strictly respects trigger distance)
                        if u.get("distance_miles", 99.0) > trigger_distance:
                            continue

                        # Same Road Only Filter: check route number, normalized street name, or cross-track error
                        if slowdown_same_road:
                            if not is_same_road(
                                self.current_road_name,
                                u.get("location", ""),
                                self.current_lat,
                                self.current_lon,
                                self.current_bearing,
                                u.get("lat", 0.0),
                                u.get("lon", 0.0)
                            ):
                                continue

                        is_hidden_police = (
                            u.get("subtype") == waze_pb2.AlertSubType.POLICE_HIDING
                            or "POLICE_HIDING" in subtype_str
                            or "hidden" in label_str
                            or "hidden" in desc_str
                            or "speed trap" in label_str
                        )

                        # Trigger conditions:
                        # 1. If Police Hidden toggle is enabled: 0 confirmations required (ignores confirmation requirement)
                        # 2. If general Police Ahead toggle is enabled: all police reports qualify provided they meet min_confirmations
                        should_slow = False
                        if is_hidden_police and slowdown_police_hidden:
                            should_slow = True
                        elif slowdown_police and (u.get("thumbs", 0) >= min_confirmations):
                            should_slow = True

                        if should_slow:
                            police_active = True
                            police_dist = u["distance_miles"]
                            set_shm_param("RoadPoliceSlowdownLocation", u.get("location", ""))
                            break
            set_shm_param("WazePoliceSlowdownActive", police_active)
            set_shm_param("WazePoliceSlowdownDist", police_dist)
            if not police_active:
                set_shm_param("RoadPoliceSlowdownLocation", "")

            # 2. Traffic Jam Slowdown (Only Police and Traffic Jams trigger auto-slowdown)
            slowdown_traffic = get_shm_param("RoadAlertSlowdownTraffic", True)
            traffic_slowdown_active = False
            traffic_dist = 0.0
            if slowdown_traffic:
                for u in upcoming:
                    cat = u.get("category", "")
                    dist = u.get("distance_miles", 99.0)
                    source = u.get("source", "")
                    thumbs = u.get("thumbs", 0)

                    # If ON_SHOULDER is mentioned anywhere, strictly skip slowdown
                    subtype_str = str(u.get("type", "")).upper()
                    label_str = str(u.get("label", "")).lower()
                    desc_str = str(u.get("desc", "")).lower()
                    cat_str = str(cat).upper()
                    if cat_str == "HAZARD_ON_SHOULDER" or "ON_SHOULDER" in subtype_str or "shoulder" in label_str or "shoulder" in desc_str:
                        continue

                    # Slowdown time filter check
                    age_mins = u.get("age_mins", 0)
                    is_active = u.get("is_active", False) or (u.get("time") == "Active")
                    if slowdown_max_age > 0 and not is_active and (age_mins > slowdown_max_age):
                        continue

                    # Distance check (strictly respects trigger distance)
                    if dist > trigger_distance:
                        continue

                    # Same Road Only Filter
                    if slowdown_same_road:
                        if not is_same_road(
                            self.current_road_name,
                            u.get("location", ""),
                            self.current_lat,
                            self.current_lon,
                            self.current_bearing,
                            u.get("lat", 0.0),
                            u.get("lon", 0.0)
                        ):
                            continue

                    # Global minimum confirmations filter on crowd-sourced Waze alerts
                    if source == "Waze" and thumbs < min_confirmations:
                        continue

                    if cat == "TRAFFIC":
                        traffic_slowdown_active = True
                        traffic_dist = dist
                        set_shm_param("RoadTrafficSlowdownLabel", u.get("label", "Traffic Standstill Ahead"))
                        set_shm_param("RoadTrafficSlowdownLocation", u.get("location", ""))
                        set_shm_param("RoadTrafficSlowdownDetail", u.get("detail", "Slowing down for upcoming traffic"))
                        break

            set_shm_param("RoadHazardSlowdownActive", traffic_slowdown_active)
            set_shm_param("RoadTrafficSlowdownDist", traffic_dist)
            if not traffic_slowdown_active:
                set_shm_param("RoadTrafficSlowdownLabel", "")
                set_shm_param("RoadTrafficSlowdownLocation", "")
                set_shm_param("RoadTrafficSlowdownDetail", "")

        else:
            set_shm_param("RoadAlertActive", False)
            set_shm_param("RoadAlertCategory", "")
            set_shm_param("RoadAlertLabel", "")
            set_shm_param("RoadAlertIcon", "")
            set_shm_param("RoadAlertDistance", 0.0)
            set_shm_param("RoadAlertLocation", "")
            set_shm_param("RoadAlertDetail", "")
            set_shm_param("RoadAlertSource", "")
            set_shm_param("RoadAlertCount", 0)
            set_shm_param("WazePoliceSlowdownActive", False)
            set_shm_param("WazePoliceSlowdownDist", 0.0)
            set_shm_param("RoadHazardSlowdownActive", False)

    def run(self):
        print("[road_alerts_d] Starting Waze Live Road Alerts daemon...")
        while True:
            try:
                self.update_gps()
                
                # Fetch Waze every 30s
                if time.monotonic() - self.last_waze_fetch > 30.0:
                    self.fetch_waze()

                upcoming = self.process_upcoming_alerts(max_radius_miles=30.0)
                self.publish_to_shm(upcoming)
                time.sleep(1.0)
            except Exception as e:
                print(f"[road_alerts_d] Loop error: {e}")
                time.sleep(2.0)

def main():
    daemon = RoadAlertsDaemon()
    daemon.run()

if __name__ == "__main__":
    main()
