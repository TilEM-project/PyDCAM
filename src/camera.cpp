#include "camera.hpp"
#include "string.h"

DCAM::DCAM(void) {
  memset(&this->apiinit, 0, sizeof(this->apiinit));
  this->apiinit.size = sizeof(this->apiinit);
  THROW_IF_ERROR(dcamapi_init(&this->apiinit))
}

Camera DCAM::get_camera(int32 device_ind) {
  DCAMDEV_OPEN devopen;
  memset(&devopen, 0, sizeof(devopen));
	devopen.size = sizeof(devopen);
	devopen.index = device_ind;
  THROW_IF_ERROR(dcamdev_open(&devopen))
  return Camera(devopen.hdcam);
}

Camera::Camera(HDCAM hdcam, int32 buf_size) {
  this->hdcam = hdcam;

  this->set_value(DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE);
  this->set_value(DCAM_IDPROP_DIRECTEMGAIN_MODE, DCAMPROP_MODE__ON);
  this->set_value(DCAM_IDPROP_IMAGE_PIXELTYPE, DCAM_PIXELTYPE_MONO8);
  this->set_value(DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
  this->set_value(DCAM_IDPROP_EXPOSURETIME_CONTROL, DCAMPROP_MODE__ON);

  DCAMWAIT_OPEN waitopen;
	memset(&waitopen, 0, sizeof(waitopen));
	waitopen.size = sizeof(waitopen);
	waitopen.hdcam = this->hdcam;

	THROW_IF_ERROR(dcamwait_open(&waitopen))

  this->wait_handle = waitopen.hwait;

  THROW_IF_ERROR(dcambuf_alloc(this->hdcam, buf_size))
}

Frame Camera::capture(void) {
  THROW_IF_ERROR(dcamcap_firetrigger( hdcam ))

  DCAMWAIT_START wait_frame;
	memset(&wait_frame, 0, sizeof(wait_frame));
	wait_frame.size = sizeof(wait_frame);
	wait_frame.eventmask = DCAMWAIT_CAPEVENT_FRAMEREADY;
	wait_frame.timeout = 1000;
  THROW_IF_ERROR(dcamwait_start(this->wait_handle, &wait_frame))

  std::shared_ptr<uint8_t[]> buf(
    new uint8_t[this->get_width() * this->get_height()],
    std::default_delete<uint8_t[]>()
  );

  DCAMBUF_FRAME	frame_buf;
	memset(&frame_buf, 0, sizeof(frame_buf));
	frame_buf.size = sizeof(frame_buf);
  frame_buf.buf = buf.get();
  frame_buf.rowbytes = this->get_width();
  frame_buf.width = this->get_width();
  frame_buf.height = this->get_height();
  frame_buf.left = 0;
  frame_buf.top = 0;
  frame_buf.type = DCAM_PIXELTYPE_NONE;
	frame_buf.iFrame = -1;
  THROW_IF_ERROR(dcambuf_copyframe(this->hdcam, &frame_buf))
  return Frame(frame_buf, buf);
}

std::string Camera::get_string(int32 param_id) {
  char text[STR_LEN];
  DCAMDEV_STRING param;
  memset(&param, 0, sizeof(param));
  param.size = sizeof(param);
  param.text = text;
  param.textbytes = sizeof(text);
  param.iString = param_id;
  THROW_IF_ERROR(dcamdev_getstring(this->hdcam, &param))
  return std::string(text);
}
