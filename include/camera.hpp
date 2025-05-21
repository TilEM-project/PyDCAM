#ifndef CAMERA_H
#define CAMERA_H

#include "exceptions.hpp"
#include "dcamapi4.h"
#include "dcamprop.h"
#include <memory>
#include <cstring>

#define STR_LEN 256

#define THROW_IF_ERROR(func) { DCAMERR error; if(failed(error = func)) throw DCAMException(error); }

#define INIT_MSG(type, name) type name; memset(&name, 0, sizeof(name)); name.size = sizeof(name);

class Frame {
public:
  Frame(DCAMBUF_FRAME frame, std::shared_ptr<uint8_t[]> buf) {
    this->frame = frame;
    this->buf = buf;
  };
  Frame(DCAMBUF_FRAME frame) {
    this->frame = frame;
  };
  int get_width(void) {
    return this->frame.width;
  };
  int get_height(void) {
    return this->frame.height;
  };
  int get_row_bytes(void) {
    return this->frame.rowbytes;
  };
  DCAM_PIXELTYPE get_pixel_type(void) {
    return this->frame.type;
  };
  float get_timestamp(void) {
    return this->frame.timestamp.sec + (float) this->frame.timestamp.microsec / 1000000;
  };
  int get_framestamp(void) {
    return this->frame.framestamp;
  };
  int get_camerastamp(void) {
    return this->frame.camerastamp;
  };
  int get_frame_ind(void) {
    return this->frame.iFrame;
  };
  template<typename T = uint8_t>
  T* get_data(void) {
    return (T*) this->frame.buf;
  };
private:
  DCAMBUF_FRAME frame;
  std::shared_ptr<uint8_t[]> buf;
};

class Attrs {
public:
  Attrs(DCAMPROP_ATTR param) {
    this->param = param;
  };
  DCAMIDPROP get_prop_id(void) {
    return (DCAMIDPROP) this->param.iProp;
  };
  bool get_attribute(DCAMPROPATTRIBUTE attr) {
    return this->param.attribute & attr;
  };
  bool get_attribute(DCAMPROPATTRIBUTE2 attr) {
    return this->param.attribute2 & attr;
  };
  DCAMPROPUNIT get_unit(void) {
    return (DCAMPROPUNIT) this->param.iUnit;
  };
  double get_min(void) {
    return this->param.valuemin;
  };
  double get_max(void) {
    return this->param.valuemax;
  };
  double get_step(void) {
    return this->param.valuestep;
  };
  double get_default(void) {
    return this->param.valuedefault;
  };
  int get_max_channel(void) {
    return this->param.nMaxChannel;
  };
  int get_max_view(void) {
    return this->param.nMaxView;
  };
  DCAMIDPROP get_num_elements_prop_id(void) {
    return (DCAMIDPROP) this->param.iProp_NumberOfElement;
  };
  DCAMIDPROP get_array_base_prop_id(void) {
    return (DCAMIDPROP) this->param.iProp_ArrayBase;
  };
  DCAMIDPROP get_next_prop_id(void) {
    return (DCAMIDPROP) this->param.iPropStep_Element;
  };
  DCAMPROPATTRIBUTE get_type(void) {
    return (DCAMPROPATTRIBUTE) (((DCAMPROPATTRIBUTE) this->param.attribute) & DCAMPROP_TYPE_MASK);
  };
private:
  DCAMPROP_ATTR param;
};

class Camera {
public:
  Camera(int device_ind, int32 buf_size = 1);
  ~Camera() {
    this->close();
  };
  void allocate(int32 framecount) {
    THROW_IF_ERROR(dcambuf_alloc(this->hdcam, framecount));
  };
  void start(DCAMCAP_START type) {
    THROW_IF_ERROR(dcamcap_start(this->hdcam, type))
  };
  void trigger(void) {
    THROW_IF_ERROR(dcamcap_firetrigger(this->hdcam, 0))
  };
  bool wait(DCAMWAIT_EVENT event, int32 timeout) {
    INIT_MSG(DCAMWAIT_START, wait)
    wait.eventmask = event;
    wait.timeout = timeout;
    THROW_IF_ERROR(dcamwait_start(this->wait_handle, &wait))
    return wait.eventhappened;
  };
  int32 get_frame_count(void) {
    INIT_MSG(DCAMCAP_TRANSFERINFO, info)
    THROW_IF_ERROR(dcamcap_transferinfo(this->hdcam, &info))
    return info.nFrameCount;
  }
  int32 get_newest_frame_ind(void) {
    INIT_MSG(DCAMCAP_TRANSFERINFO, info)
    THROW_IF_ERROR(dcamcap_transferinfo(this->hdcam, &info))
    return info.nNewestFrameIndex;
  }
  DCAMCAP_STATUS get_status(void) {
    int status;
    THROW_IF_ERROR(dcamcap_status(this->hdcam, &status))
    return (DCAMCAP_STATUS) status;
  };
  Frame copyframe(
    int32 frame_ind = -1,
    int32 width = 0,
    int32 height = 0,
    int32 left = 0,
    int32 top = 0
  );
  Frame lockframe(int32 frame_ind = -1);
  void stop(void) {
    dcamcap_stop(this->hdcam);
  };
  void close(void) {
    this->stop();
    dcambuf_release(this->hdcam);
    dcamwait_close(this->wait_handle);
    dcamdev_close(this->hdcam);
  };
  std::string get_string(DCAM_IDSTR param_id);
  Attrs get_attrs(DCAMIDPROP param_id) {
    DCAMPROP_ATTR param;
    memset(&param, 0, sizeof(param));
    param.cbSize = sizeof(param);
    param.iProp = param_id;
    THROW_IF_ERROR(dcamprop_getattr(this->hdcam, &param))
    return Attrs(param);
  };
  double get_value_real(DCAMIDPROP prop_id) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_REAL)
      throw DCAMException("Incorrect type");
    double value;
    THROW_IF_ERROR(dcamprop_getvalue(this->hdcam, prop_id, &value))
    return value;
  };
  int32 get_value_long(DCAMIDPROP prop_id) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_LONG)
      throw DCAMException("Incorrect type");
    double value;
    THROW_IF_ERROR(dcamprop_getvalue(this->hdcam, prop_id, &value))
    return (int32) value;
  };
  DCAMPROPMODEVALUE get_value_mode(DCAMIDPROP prop_id) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_MODE)
      throw DCAMException("Incorrect type");
    double value;
    THROW_IF_ERROR(dcamprop_getvalue(this->hdcam, prop_id, &value))
    return (DCAMPROPMODEVALUE) value;
  };
  double set_value(DCAMIDPROP prop_id, double value) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_REAL)
      throw DCAMException("Incorrect type");
    THROW_IF_ERROR(dcamprop_setgetvalue(this->hdcam, prop_id, &value, 0))
    return value;
  };
  int32 set_value(DCAMIDPROP prop_id, int32 value) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_LONG)
      throw DCAMException("Incorrect type");
    double cvalue = value;
    THROW_IF_ERROR(dcamprop_setgetvalue(this->hdcam, prop_id, &cvalue))
    return (int32) cvalue;
  };
  int set_value(DCAMIDPROP prop_id, DCAMPROPMODEVALUE value) {
    if (this->get_attrs(prop_id).get_type() != DCAMPROP_TYPE_MODE)
      throw DCAMException("Incorrect type");
    double cvalue = value;
    THROW_IF_ERROR(dcamprop_setgetvalue(this->hdcam, prop_id, &cvalue))
    return (DCAMPROPMODEVALUE) cvalue;
  }
private:
  HDCAM hdcam;
  HDCAMWAIT wait_handle;
};

class DCAM {
public:
  DCAM(void);
  ~DCAM() {
    this->close();
  };
  void close(void) {
    dcamapi_uninit();
  };
  int32 get_num_cameras(void) {
    return this->apiinit.iDeviceCount;
  };
  Camera get_camera(int32 device_ind);
private:
  DCAMAPI_INIT apiinit;
};

inline float get_pixel_size(DCAM_PIXELTYPE type) {
  switch (type) {
    case DCAM_PIXELTYPE_MONO8: return 1;
    case DCAM_PIXELTYPE_MONO12: case DCAM_PIXELTYPE_MONO12P: return 1.5;
    case DCAM_PIXELTYPE_BGR24: case DCAM_PIXELTYPE_RGB24: return 3;
    case DCAM_PIXELTYPE_BGR48: case DCAM_PIXELTYPE_RGB48: return 6;
    default: throw DCAMException("Pixel type of unknown size!");
  }
};

inline float get_pixel_size(int32 type) {
  return get_pixel_size((DCAM_PIXELTYPE) type);
};

#endif // CAMERA_H
