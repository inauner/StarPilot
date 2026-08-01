from types import SimpleNamespace

import pytest

from openpilot.selfdrive.ui.mici.onroad import cameraview as mici_cameraview
from openpilot.selfdrive.ui.onroad import cameraview as big_cameraview


class FakeFrame:
  def __init__(self, frame_id: int, idx: int):
    self.frame_id = frame_id
    self.idx = idx


def _camera_view():
  view = big_cameraview.CameraView.__new__(big_cameraview.CameraView)
  view._name = "camerad"
  view.frame = None
  view._last_frame_id = -1
  view._regressive_frame_count = 0
  view._texture_needs_update = False
  view._closed = True
  return view


def test_mici_uses_shared_camera_view():
  assert mici_cameraview.CameraView is big_cameraview.CameraView


def test_pending_switch_is_cancelled_when_requested_stream_is_current():
  view = _camera_view()
  view._stream_type = big_cameraview.VisionStreamType.VISION_STREAM_ROAD
  view._target_stream_type = big_cameraview.VisionStreamType.VISION_STREAM_DRIVER
  view._target_client = object()
  view._switching = True

  view.switch_stream(big_cameraview.VisionStreamType.VISION_STREAM_ROAD)

  assert view._target_client is None
  assert view._target_stream_type is None
  assert not view._switching


def test_reused_egl_slot_cannot_move_camera_backwards(monkeypatch):
  monkeypatch.setattr(big_cameraview.cloudlog, "warning", lambda *_args, **_kwargs: None)
  view = _camera_view()

  displayed = FakeFrame(frame_id=10, idx=0)
  assert view._accept_frame(displayed, packet_frame_id=10)

  # camerad cycles back to this shared slot while it remains displayed.
  displayed.frame_id = 30
  view._observe_displayed_frame()

  delayed = FakeFrame(frame_id=20, idx=1)
  assert not view._accept_frame(delayed, packet_frame_id=20)
  assert view.frame is displayed
  assert view._last_frame_id == 30
  assert view._regressive_frame_count == 1


def test_newer_camera_frame_is_accepted():
  view = _camera_view()
  view._last_frame_id = 30
  view._regressive_frame_count = 2
  newer = FakeFrame(frame_id=31, idx=2)

  assert view._accept_frame(newer, packet_frame_id=31)
  assert view.frame is newer
  assert view._last_frame_id == 31
  assert view._regressive_frame_count == 0
  assert view._texture_needs_update


def test_shared_camera_has_upstream_shaders_and_driver_enhancement():
  assert "samplerExternalOES" in big_cameraview.FRAME_FRAGMENT_SHADER_EXTERNAL
  assert "pow(color.rgb, vec3(1.0/1.28))" in big_cameraview.FRAME_FRAGMENT_SHADER_EXTERNAL
  assert "uniform sampler2D texture0" in big_cameraview.FRAME_FRAGMENT_SHADER_YUV
  assert "uniform sampler2D texture1" in big_cameraview.FRAME_FRAGMENT_SHADER_YUV
  assert "uniform int enhance_driver" in big_cameraview.FRAME_FRAGMENT_SHADER_EXTERNAL
  assert "uniform int enhance_driver" in big_cameraview.FRAME_FRAGMENT_SHADER_YUV
  assert "uniform int engaged" not in big_cameraview.FRAME_FRAGMENT_SHADER_EXTERNAL
  assert "uniform int engaged" not in big_cameraview.FRAME_FRAGMENT_SHADER_YUV
  assert hasattr(big_cameraview.CameraView, "_render_egl")
  assert hasattr(big_cameraview.CameraView, "_fallback_to_textures")


def test_shared_camera_falls_back_after_repeated_regressive_frames(monkeypatch):
  monkeypatch.setattr(big_cameraview.cloudlog, "warning", lambda *_args, **_kwargs: None)
  view = _camera_view()
  view._use_egl = True
  view.frame = FakeFrame(frame_id=30, idx=0)
  view._last_frame_id = 30
  fallback_reasons = []
  view._fallback_to_textures = fallback_reasons.append

  for frame_id in (20, 19, 18):
    assert not view._accept_frame(FakeFrame(frame_id=frame_id, idx=1), packet_frame_id=frame_id)

  assert fallback_reasons == ["repeated regressive frames"]
  assert view.frame.frame_id == 30


def test_shared_camera_fallback_reloads_texture_backend(monkeypatch):
  view = _camera_view()
  view._use_egl = True
  view.shader = SimpleNamespace(id=1)
  events = []
  view._clear_textures = lambda: events.append("clear")
  view._load_frame_shader = lambda: events.append(("shader", view._use_egl))
  view._initialize_textures = lambda: events.append("textures")
  monkeypatch.setattr(big_cameraview.cloudlog, "error", lambda *_args, **_kwargs: None)
  monkeypatch.setattr(big_cameraview.rl, "unload_shader", lambda _shader: events.append("unload_shader"))

  view._fallback_to_textures("test")

  assert events == ["clear", "unload_shader", ("shader", False), "textures"]
  assert not view._use_egl


def test_egl_image_creation_failure_is_reported(monkeypatch):
  view = _camera_view()
  view.frame = SimpleNamespace(idx=0, width=1928, height=1208, stride=2048, fd=7, uv_offset=2473984)
  view.egl_texture = SimpleNamespace(id=1)
  view.egl_images = {}
  monkeypatch.setattr(big_cameraview, "create_egl_image", lambda *_args: None)

  assert not view._render_egl(None, None)
  assert view.egl_images == {}


def test_invalid_egl_texture_is_reported_without_binding(monkeypatch):
  view = _camera_view()
  view.frame = SimpleNamespace(idx=0)
  view.egl_texture = SimpleNamespace(id=0)
  view.egl_images = {0: object()}
  monkeypatch.setattr(big_cameraview, "bind_egl_image_to_texture",
                      lambda *_args: pytest.fail("invalid EGL texture was bound"))

  assert not view._render_egl(None, None)


def test_egl_render_always_ends_shader_mode(monkeypatch):
  view = _camera_view()
  view.frame = SimpleNamespace(idx=0, width=1928, height=1208)
  view.egl_texture = SimpleNamespace(id=1, width=0, height=0)
  view.egl_images = {0: object()}
  view.shader = SimpleNamespace(id=1)
  view._update_shader_state = lambda: None
  events = []
  monkeypatch.setattr(big_cameraview, "bind_egl_image_to_texture", lambda *_args: None)
  monkeypatch.setattr(big_cameraview.rl, "begin_shader_mode", lambda *_args: events.append("begin"))
  def fail_draw(*_args):
    raise RuntimeError("draw failed")

  monkeypatch.setattr(big_cameraview.rl, "draw_texture_pro", fail_draw)
  monkeypatch.setattr(big_cameraview.rl, "end_shader_mode", lambda: events.append("end"))

  with pytest.raises(RuntimeError, match="draw failed"):
    view._render_egl(None, None)

  assert events == ["begin", "end"]


def test_driver_enhancement_tracks_active_stream(monkeypatch):
  view = _camera_view()
  view.shader = SimpleNamespace(id=1)
  view._enhance_driver_loc = 2
  view._enhance_driver_val = [0]
  values = []
  monkeypatch.setattr(big_cameraview.rl, "set_shader_value",
                      lambda _shader, _loc, value, _type: values.append(value[0]))

  view._stream_type = big_cameraview.VisionStreamType.VISION_STREAM_ROAD
  view._update_shader_state()
  view._stream_type = big_cameraview.VisionStreamType.VISION_STREAM_DRIVER
  view._update_shader_state()
  view._stream_type = big_cameraview.VisionStreamType.VISION_STREAM_WIDE_ROAD
  view._update_shader_state()

  assert values == [0, 1, 0]


def test_texture_fallback_survives_egl_cleanup_failure(monkeypatch):
  view = _camera_view()
  view._use_egl = True
  view.shader = SimpleNamespace(id=1)
  events = []

  def fail_cleanup():
    raise RuntimeError("cleanup failed")

  view._clear_textures = fail_cleanup
  view._load_frame_shader = lambda: events.append(("shader", view._use_egl))
  view._initialize_textures = lambda: events.append("textures")
  monkeypatch.setattr(big_cameraview.cloudlog, "error", lambda *_args, **_kwargs: None)
  monkeypatch.setattr(big_cameraview.cloudlog, "exception", lambda *_args, **_kwargs: None)
  monkeypatch.setattr(big_cameraview.rl, "unload_shader", lambda _shader: events.append("unload_shader"))

  view._fallback_to_textures("test")

  assert not view._use_egl
  assert events == ["unload_shader", ("shader", False), "textures"]
