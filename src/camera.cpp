#include "camera.hpp"
#include "string.h"
#include <cmath>

DCAM::DCAM(void) {
  memset(&this->apiinit, 0, sizeof(this->apiinit));
  this->apiinit.size = sizeof(this->apiinit);
  THROW_IF_ERROR(dcamapi_init(&this->apiinit))
}

Camera DCAM::get_camera(int32 device_ind) {
  return Camera(device_ind);
}

Camera::Camera(int device_ind, int32 buf_size) {
  DCAMDEV_OPEN devopen;
  memset(&devopen, 0, sizeof(devopen));
	devopen.size = sizeof(devopen);
	devopen.index = device_ind;
  THROW_IF_ERROR(dcamdev_open(&devopen))
  this->hdcam = devopen.hdcam;

  DCAMWAIT_OPEN waitopen;
	memset(&waitopen, 0, sizeof(waitopen));
	waitopen.size = sizeof(waitopen);
	waitopen.hdcam = this->hdcam;

	THROW_IF_ERROR(dcamwait_open(&waitopen))

  this->wait_handle = waitopen.hwait;
}

Frame Camera::copyframe(
  int32 frame_ind,
  int32 width,
  int32 height,
  int32 left,
  int32 top) {
  if (width == 0) width = this->get_value_long(DCAM_IDPROP_IMAGE_WIDTH);
  if (height == 0) height = this->get_value_long(DCAM_IDPROP_IMAGE_HEIGHT);

  int row_size = std::ceil(get_pixel_size(this->get_value_long(DCAM_IDPROP_IMAGE_PIXELTYPE)) * width);
  int buf_size = row_size * height;
  std::shared_ptr<uint8_t[]> buf(
    new uint8_t[buf_size],
    std::default_delete<uint8_t[]>()
  );

  INIT_MSG(DCAMBUF_FRAME, frame_buf)
  frame_buf.buf = buf.get();
  frame_buf.rowbytes = row_size;
  frame_buf.width = width;
  frame_buf.height = height;
  frame_buf.left = left;
  frame_buf.top = top;
  frame_buf.type = DCAM_PIXELTYPE_NONE;
	frame_buf.iFrame = frame_ind;
  THROW_IF_ERROR(dcambuf_copyframe(this->hdcam, &frame_buf))

  return Frame(frame_buf, buf);
}

Frame Camera::lockframe(int32 frame_ind) {
  INIT_MSG(DCAMBUF_FRAME, frame_buf)
  frame_buf.iFrame = frame_ind;
  THROW_IF_ERROR(dcambuf_lockframe(this->hdcam, &frame_buf))
  return Frame(frame_buf);
}

std::string Camera::get_string(DCAM_IDSTR param_id) {
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
