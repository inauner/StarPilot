import { html, reactive } from "/assets/vendor/arrow-core.js"

const state = reactive({
  loading: true,
  alerts: [],
  activeThreat: null,
  totalCount: 0,
  gps: { lat: 0, lon: 0, bearing: 0 },
  settings: {
    WazePoliceAutoSlowdown: true,
    WazePoliceSlowdownHidden: true,
    WazePoliceMinConfirmations: 3,
    WazePoliceTriggerDistance: 1.0,
    WazePoliceSlowdownActive: false,
    WazePoliceSlowdownDist: 0.0,
    RoadAlertShowPolice: true,
    RoadAlertShowTraffic: true,
    RoadAlertShowAccidents: true,
    RoadAlertShowClosures: true,
    RoadAlertShowLaneClosures: true,
    RoadAlertShowHazardOnRoad: true,
    RoadAlertShowHazardOnShoulder: true,
    RoadAlertShowWeather: true,
    RoadAlertShowGeneral: true,
    RoadAlertDisplayMaxDistMi: 30,
    RoadAlertDisplayMaxAgeMin: 0,
    RoadAlertSlowdownTraffic: true,
    RoadAlertSlowdownAccidents: true,
    RoadAlertSlowdownClosures: true,
    RoadAlertSlowdownLaneClosures: true,
    RoadAlertSlowdownHazardOnRoad: true,
    RoadAlertSlowdownWeather: false,
    RoadAlertSlowdownMaxAgeMin: 30,
    RoadAlertSlowdownSameRoadOnly: false,
    CurrentRoadName: "",
    WazeSessionId: "",
    WazeSecretKey: "",
    WazeAuthStatus: "Idle"
  },
  showSessionGrabber: false,
  grabberRawInput: "",
  lastUpdated: ""
})

let loadedOnce = false

function notify(msg, level) {
  if (typeof window.showSnackbar === "function") {
    window.showSnackbar(msg, level)
  } else {
    console.log("[Snackbar]", level || "info", msg)
  }
}

let isFetching = false

async function loadData() {
  if (isFetching) return
  isFetching = true
  try {
    const res = await fetch("/api/road_alerts/live", { cache: "no-store" })
    if (res.ok) {
      const data = await res.json()
      state.alerts = data.alerts || []
      state.activeThreat = data.active_threat || null
      state.totalCount = data.total_count || 0
      state.gps = data.gps || { lat: 0, lon: 0, bearing: 0 }
      if (data.settings) {
        for (const [k, v] of Object.entries(data.settings)) {
          state.settings[k] = v
        }
      }
      state.lastUpdated = new Date().toLocaleTimeString()
    }
  } catch (err) {
    console.error("Failed to load road alerts:", err)
  } finally {
    isFetching = false
    state.loading = false
  }
}

async function updateSetting(key, val) {
  state.settings[key] = val
  try {
    const res = await fetch("/api/road_alerts/settings", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ [key]: val, key, value: val })
    })
    if (res.ok) {
      loadData()
      notify(`Updated ${key}`)
    }
  } catch (err) {
    notify("Failed to update setting", "error")
  }
}

async function triggerAction(action, payload = {}) {
  try {
    const res = await fetch(`/api/road_alerts/action/${action}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    })
    const data = await res.json()
    if (res.ok && data.status === "ok") {
      notify(data.message || "Action successful", "success")
      await loadData()
    } else {
      notify(data.message || data.error || "Action failed", "error")
    }
  } catch (err) {
    notify("Request failed", "error")
  }
}

function openWazeLogin() {
  window.open("https://www.waze.com/live-map", "_blank", "width=850,height=750")
  state.showSessionGrabber = true
}

function parseAndSaveSessionString() {
  const raw = state.grabberRawInput.trim()
  if (!raw) {
    notify("Please paste your Waze cookie or session string", "error")
    return
  }

  let sessionId = ""
  let secretKey = ""

  // Case 1: JSON format
  if (raw.startsWith("{") && raw.endsWith("}")) {
    try {
      const obj = JSON.parse(raw)
      sessionId = obj.sessionId || obj.session_id || obj._waze_session || obj.session || ""
      secretKey = obj.secretKey || obj.secret_key || obj._csrf_token || obj.secret || ""
    } catch (e) {}
  }

  // Case 2: Cookie Header format (e.g. "_waze_session=abc123; secret=xyz456")
  if (!sessionId) {
    const sessionMatch = raw.match(/_?waze_session=([^;]+)/i) || raw.match(/session_?id=([^;]+)/i)
    if (sessionMatch) sessionId = sessionMatch[1].trim()
  }

  if (!secretKey) {
    const secretMatch = raw.match(/secret(_key)?=([^;]+)/i) || raw.match(/csrf_?token=([^;]+)/i)
    if (secretMatch) secretKey = secretMatch[1].trim()
  }

  // Case 3: Raw single token
  if (!sessionId && !raw.includes("=") && !raw.includes(";")) {
    sessionId = raw
  }

  if (sessionId) {
    updateSetting("WazeSessionId", sessionId)
    if (secretKey) updateSetting("WazeSecretKey", secretKey)
    updateSetting("WazeAuthStatus", "User Account Linked")
    notify("Successfully imported Waze User Account Session!", "success")
    state.showSessionGrabber = false
    state.grabberRawInput = ""
  } else {
    notify("Could not find a valid _waze_session token in pasted text", "error")
  }
}

let pollInterval = null

export function RoadAlerts() {
  if (!pollInterval) {
    loadData()
    pollInterval = setInterval(loadData, 3000)
  }

  return html`
    <link rel="stylesheet" href="/assets/components/tools/road_alerts.css">

    <style>
      details.road-card summary.road-card-title {
        cursor: pointer;
        user-select: none;
        list-style: none;
        display: flex;
        justify-content: space-between;
        align-items: center;
        transition: color 0.15s ease;
        margin-bottom: 0;
        padding-bottom: 6px;
      }
      details.road-card summary::-webkit-details-marker {
        display: none;
      }
      details.road-card[open] summary.road-card-title {
        border-bottom: 1px solid #2d2d38;
        margin-bottom: 16px;
        padding-bottom: 10px;
      }
      details.road-card summary .toggle-arrow {
        transition: transform 0.2s ease;
        font-size: 1rem;
        color: #8f92a1;
      }
      details.road-card[open] summary .toggle-arrow {
        transform: rotate(180deg);
        color: #fff;
      }
      .card-content-wrapper {
        margin-top: 10px;
      }
    </style>

    <div class="road-container">
      <!-- 1. Header -->
      <div class="road-header">
        <div class="road-header-title">
          <h1><i class="bi bi-shield-exclamation text-danger"></i> Live Road Alerts & Police Radar</h1>
          <p>Real-time Waze Police Speed Traps & Road Hazards (Internet Connection Required)</p>
        </div>
        <div class="road-header-actions">
          <button class="btn btn-sm btn-outline-secondary" @click="${loadData}">
            <i class="bi bi-arrow-clockwise"></i> Refresh
          </button>
        </div>
      </div>

      <!-- 2. Active Closest Threat Banner (4-Line Incident Layout) -->
      ${() => state.activeThreat ? html`
        <div class="active-threat-card ${() => (state.activeThreat.category || '').toLowerCase()}">
          <div class="threat-icon">${() => state.activeThreat.icon || '⚠️'}</div>
          <div class="threat-details">
            <!-- Line 1: Label (Subtype) Lane Distance -->
            <div class="threat-title-row">
              <span class="threat-label">
                ${() => state.activeThreat.label}${() => state.activeThreat.type ? ` (${state.activeThreat.type})` : ''}
                ${() => state.activeThreat.lane ? html`<span class="badge bg-warning text-dark ms-2 fw-bold"><i class="bi bi-signpost-2"></i> ${state.activeThreat.lane}</span>` : ''}
              </span>
              <span class="threat-distance">${() => state.activeThreat.is_radar ? 'LIVE DETECTION' : (state.activeThreat.distance_miles + ' mi')}</span>
            </div>
            <!-- Line 2: Location and Source -->
            <div class="threat-location">
              <i class="bi bi-geo-alt-fill"></i> ${() => state.activeThreat.location} 
              <span class="badge ${() => state.activeThreat.source === 'Waze' ? 'bg-primary' : 'bg-secondary'} ms-2">${() => state.activeThreat.source || 'Waze'}</span>
            </div>
            <!-- Line 3: Detail / Driver Confirmations -->
            ${() => state.activeThreat.detail ? html`<div class="threat-desc">${() => state.activeThreat.detail}</div>` : ''}
            <!-- Line 4: Reported Timestamp -->
            ${() => state.activeThreat.time ? html`<div class="threat-time text-muted small mt-1"><i class="bi bi-clock"></i> Reported: ${() => state.activeThreat.time}</div>` : ''}
          </div>
        </div>
      ` : html`
        <div class="no-threat-banner">
          <i class="bi bi-shield-check text-success"></i> Route Clear — No immediate hazards or police traps detected ahead
        </div>
      `}

      <!-- 3. Unified Road Alert & Police Auto-Slowdown (Collapsible, collapsed by default) -->
      <details class="road-card">
        <summary class="road-card-title">
          <span><i class="bi bi-speedometer2 text-danger"></i> Road Alert & Police Auto-Slowdown</span>
          <i class="bi bi-chevron-down toggle-arrow"></i>
        </summary>

        <div class="card-content-wrapper">
          <p class="text-muted small mb-3">Automatically drop vehicle cruise target down to the posted road speed limit when approaching verified hazards or police traps:</p>

          <!-- 1. Only Slow Down for Current Road (Top of section) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🛣️ Only Slow Down for Current Road</span>
              <span class="road-setting-desc">Filter slowdowns strictly to incidents on your active highway or street (matches route numbers, names, and driving corridor)</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertSlowdownSameRoadOnly}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertSlowdownSameRoadOnly', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <!-- 2. Minimum Confirmations (Global for crowd-sourced Waze reports) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">Minimum Confirmations</span>
              <span class="road-setting-desc">Minimum driver thumbs-up reports required for crowd-sourced Waze slowdown alerts</span>
            </div>
            <select class="road-select" 
                    value="${() => String(Number(state.settings.WazePoliceMinConfirmations || 3))}"
                    @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('WazePoliceMinConfirmations', parseInt(el.value, 10)); }}">
              <option value="1" selected="${() => Number(state.settings.WazePoliceMinConfirmations) === 1}">1+ Report (Most Sensitive)</option>
              <option value="2" selected="${() => Number(state.settings.WazePoliceMinConfirmations) === 2}">2+ Reports</option>
              <option value="3" selected="${() => Number(state.settings.WazePoliceMinConfirmations) === 3}">3+ Reports (Recommended)</option>
              <option value="5" selected="${() => Number(state.settings.WazePoliceMinConfirmations) === 5}">5+ Reports (High Confidence)</option>
              <option value="10" selected="${() => Number(state.settings.WazePoliceMinConfirmations) === 10}">10+ Reports (Verified Only)</option>
            </select>
          </div>

          <!-- 3. Trigger Distance (Global for all slowdown alerts) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">Trigger Distance</span>
              <span class="road-setting-desc">Distance ahead to begin slowing down to road speed limit for upcoming alerts</span>
            </div>
            <select class="road-select" 
                    value="${() => String(Number(state.settings.WazePoliceTriggerDistance || 1))}"
                    @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('WazePoliceTriggerDistance', parseFloat(el.value)); }}">
              <option value="0.5" selected="${() => Number(state.settings.WazePoliceTriggerDistance) === 0.5}">0.5 Miles</option>
              <option value="0.75" selected="${() => Number(state.settings.WazePoliceTriggerDistance) === 0.75}">0.75 Miles</option>
              <option value="1" selected="${() => Number(state.settings.WazePoliceTriggerDistance) === 1 || Number(state.settings.WazePoliceTriggerDistance) === 1.0}">1.0 Mile (Recommended)</option>
              <option value="1.5" selected="${() => Number(state.settings.WazePoliceTriggerDistance) === 1.5}">1.5 Miles</option>
              <option value="2" selected="${() => Number(state.settings.WazePoliceTriggerDistance) === 2 || Number(state.settings.WazePoliceTriggerDistance) === 2.0}">2.0 Miles</option>
            </select>
          </div>

          <!-- 4. Max Report Age (Slowdown) (5, 15, 30, 60, All; includes Active) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">Max Report Age (Slowdown)</span>
              <span class="road-setting-desc">Ignore older crowd reports for vehicle slowdown (Active road closures always included)</span>
            </div>
            <select class="road-select" 
                    value="${() => String(Number(state.settings.RoadAlertSlowdownMaxAgeMin ?? 30))}"
                    @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertSlowdownMaxAgeMin', parseInt(el.value, 10)); }}">
              <option value="5" selected="${() => Number(state.settings.RoadAlertSlowdownMaxAgeMin) === 5}">5 Minutes (+ Active)</option>
              <option value="15" selected="${() => Number(state.settings.RoadAlertSlowdownMaxAgeMin) === 15}">15 Minutes (+ Active)</option>
              <option value="30" selected="${() => Number(state.settings.RoadAlertSlowdownMaxAgeMin) === 30}">30 Minutes (+ Active, Recommended)</option>
              <option value="60" selected="${() => Number(state.settings.RoadAlertSlowdownMaxAgeMin) === 60}">60 Minutes (+ Active)</option>
              <option value="0" selected="${() => Number(state.settings.RoadAlertSlowdownMaxAgeMin) === 0}">All Reports (+ Active)</option>
            </select>
          </div>

          <!-- 5. Slowdown for Police Ahead (All police reports adhering to minimum confirmations) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🚨 Slowdown for Police Ahead</span>
              <span class="road-setting-desc">Slow down for all police reports that meet the minimum confirmations threshold</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.WazePoliceAutoSlowdown}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('WazePoliceAutoSlowdown', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <!-- 6. Slowdown for Police Hidden / Speed Traps (Ignores confirmation requirement) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🚨 Slowdown for Police Hidden</span>
              <span class="road-setting-desc">Immediately slow down for hidden speed traps (ignores confirmations requirement)</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.WazePoliceSlowdownHidden}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('WazePoliceSlowdownHidden', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <!-- 7. Slowdown for Traffic Jams & Standstills (Right under Police) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🛑 Slowdown for Traffic Jams & Standstills</span>
              <span class="road-setting-desc">Automatically drop cruise speed and hold banner when approaching heavy traffic or standstill jams (releases on gas)</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertSlowdownTraffic}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertSlowdownTraffic', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>
        </div>
      </details>

      <!-- 4. Incident Category Display Filters (Collapsible, collapsed by default) -->
      <details class="road-card">
        <summary class="road-card-title">
          <span><i class="bi bi-funnel-fill text-warning"></i> Incident Category Display Filters</span>
          <i class="bi bi-chevron-down toggle-arrow"></i>
        </summary>
        
        <div class="card-content-wrapper">
          <!-- 1. Max Distance (Display) (.5, .75, 1, 1.5, 2, 3, 5, 10, 15, 30 miles) -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">Max Distance (Display)</span>
              <span class="road-setting-desc">Distance forward along travel route to display road incidents and alerts</span>
            </div>
            <select class="road-select" 
                    value="${() => String(Number(state.settings.RoadAlertDisplayMaxDistMi ?? 30))}"
                    @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertDisplayMaxDistMi', parseFloat(el.value)); }}">
              <option value="0.5" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 0.5}">0.5 Miles</option>
              <option value="0.75" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 0.75}">0.75 Miles</option>
              <option value="1" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 1 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 1.0}">1.0 Mile</option>
              <option value="1.5" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 1.5}">1.5 Miles</option>
              <option value="2" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 2 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 2.0}">2.0 Miles</option>
              <option value="3" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 3 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 3.0}">3.0 Miles</option>
              <option value="5" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 5 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 5.0}">5.0 Miles</option>
              <option value="10" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 10 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 10.0}">10 Miles</option>
              <option value="15" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 15 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 15.0}">15 Miles</option>
              <option value="30" selected="${() => Number(state.settings.RoadAlertDisplayMaxDistMi) === 30 || Number(state.settings.RoadAlertDisplayMaxDistMi) === 30.0 || !state.settings.RoadAlertDisplayMaxDistMi}">30 Miles (Maximum)</option>
            </select>
          </div>

          <!-- 2. Max Report Age (Display) right below Max Distance -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">Max Report Age (Display)</span>
              <span class="road-setting-desc">Filter incident list & UI by how recently reported (Active road closures always included)</span>
            </div>
            <select class="road-select" 
                    value="${() => String(Number(state.settings.RoadAlertDisplayMaxAgeMin ?? 0))}"
                    @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertDisplayMaxAgeMin', parseInt(el.value, 10)); }}">
              <option value="5" selected="${() => Number(state.settings.RoadAlertDisplayMaxAgeMin) === 5}">5 Minutes (+ Active)</option>
              <option value="15" selected="${() => Number(state.settings.RoadAlertDisplayMaxAgeMin) === 15}">15 Minutes (+ Active)</option>
              <option value="30" selected="${() => Number(state.settings.RoadAlertDisplayMaxAgeMin) === 30}">30 Minutes (+ Active)</option>
              <option value="60" selected="${() => Number(state.settings.RoadAlertDisplayMaxAgeMin) === 60}">60 Minutes (+ Active)</option>
              <option value="0" selected="${() => Number(state.settings.RoadAlertDisplayMaxAgeMin) === 0}">All Reports (+ Active, Recommended)</option>
            </select>
          </div>

          <!-- Category Toggles -->
          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🚨 Police Traps & Cameras</span>
              <span class="road-setting-desc">Show hidden speed traps, visible police cars, and mobile speed cameras</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowPolice}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowPolice', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🛑 Traffic Jams & Standstills</span>
              <span class="road-setting-desc">Show heavy traffic congestion, slow traffic, and standstill jams</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowTraffic}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowTraffic', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">💥 Accidents & Collisions</span>
              <span class="road-setting-desc">Show major crashes, minor fender benders, and vehicle collisions</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowAccidents}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowAccidents', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">⛔ Road & Ramp Closures</span>
              <span class="road-setting-desc">Show full road closures, highway ramp closures, and blocked turns</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowClosures}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowClosures', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🚧 Lane Closures & Construction</span>
              <span class="road-setting-desc">Show blocked traffic lanes, road work crews, and construction zones</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowLaneClosures}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowLaneClosures', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">⚠️ Hazards On Road (Active Lane)</span>
              <span class="road-setting-desc">Show objects in travel lanes, potholes, emergency vehicles, stopped cars in lane</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowHazardOnRoad}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowHazardOnRoad', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🚗 Hazards On Shoulder</span>
              <span class="road-setting-desc">Show vehicles parked/stalled on shoulder, animals on shoulder, missing signs</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowHazardOnShoulder}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowHazardOnShoulder', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">🌧️ Severe Weather Hazards</span>
              <span class="road-setting-desc">Show dense fog, localized flooding, heavy downpours, snow/ice</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowWeather}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowWeather', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>

          <div class="road-setting-row">
            <div class="road-setting-info">
              <span class="road-setting-label">ℹ️ General & Unclassified Reports</span>
              <span class="road-setting-desc">Show general community advisories and reports without a specific subtype</span>
            </div>
            <label class="road-switch">
              <input type="checkbox" 
                     checked="${() => !!state.settings.RoadAlertShowGeneral}" 
                     @change="${(e) => { const el = e && (e.currentTarget || e.target); if (el) updateSetting('RoadAlertShowGeneral', el.checked); }}" />
              <span class="road-slider"></span>
            </label>
          </div>
        </div>
      </details>

      <!-- 6. Feed List (Active Incidents Along Route - 4 Line Layout) At The Bottom -->
      <div class="alerts-list-card mb-4">
        <div class="alerts-list-header">
          <h2><i class="bi bi-broadcast-pin"></i> Active Incidents Along Route (${() => state.alerts.length})</h2>
        </div>
        <div class="alerts-list">
          ${() => state.loading ? html`<div class="p-4 text-center"><i class="spinner-border spinner-border-sm"></i> Loading incidents...</div>` : ''}
          ${() => !state.loading && state.alerts.length === 0 ? html`
            <div class="p-4 text-center text-muted">No active incidents detected within 30 miles forward cone.</div>
          ` : ''}
          ${() => state.alerts.map(a => html`
            <div class="alert-item ${a.category.toLowerCase()}">
              <div class="alert-item-icon">${a.icon}</div>
              <div class="alert-item-body">
                <!-- Line 1: Label (Subtype) Lane Distance -->
                <div class="alert-item-header">
                  <span class="alert-item-type">
                    ${a.label}${a.type ? ` (${a.type})` : ''}
                    ${a.lane ? html`<span class="badge bg-warning text-dark ms-2 fw-bold"><i class="bi bi-signpost-2"></i> ${a.lane}</span>` : ''}
                  </span>
                  <span class="alert-item-dist">${a.distance_miles} mi</span>
                </div>
                <!-- Line 2: Location Source -->
                <div class="alert-item-loc">
                  <i class="bi bi-geo-alt-fill"></i> ${a.location} 
                  <span class="badge ${a.source === 'Waze' ? 'bg-primary' : 'bg-dark'} ms-1">${a.source || 'Waze'}</span>
                </div>
                <!-- Line 3: Detail / Confirmations -->
                ${a.detail ? html`<div class="alert-item-detail">${a.detail}</div>` : ''}
                <!-- Line 4: Reported Timestamp -->
                <div class="alert-item-time"><i class="bi bi-clock"></i> Reported: ${a.time}</div>
              </div>
            </div>
          `)}
        </div>
      </div>
    </div>
  `
}

export const RoadAlertsView = RoadAlerts;
