#ifndef CAMERA_H
#define CAMERA_H

#include "exceptions.hpp"
#include "dcamapi4.h"
#include "dcamprop.h"
#include <memory>
#include <cstdint>

#define STR_LEN 256

#define THROW_IF_ERROR(func) { DCAMERR error; if(failed(error = func)) throw DCAMException(error); }

class Frame {
public:
  Frame(DCAMBUF_FRAME frame, std::shared_ptr<uint8_t[]> buf) {
    this->frame = frame;
    this->buf = buf;
  };
  int get_width(void) {
    return this->frame.width;
  };
  int get_height(void) {
    return this->frame.height;
  };
  uint8_t* get_data(void) {
    return this->buf.get();
  };
private:
  DCAMBUF_FRAME frame;
  std::shared_ptr<uint8_t[]> buf;
};

class Camera {
public:
  Camera(HDCAM hdcam, int32 buf_size = 1);
  ~Camera() {
    this->close();
  };
  void start(void) {
    THROW_IF_ERROR(dcamcap_start(this->hdcam, DCAMCAP_START_SEQUENCE))
  };
  Frame capture(void);
  std::string get_model(void) {
    return this->get_string(DCAM_IDSTR_MODEL);
  };
  std::string get_camera_id(void) {
    return this->get_string(DCAM_IDSTR_CAMERAID);
  };
  double get_exposure(void) {
    return this->get_value(DCAM_IDPROP_EXPOSURETIME);
  }
  void set_exposure(double exposure) {
    this->set_value(DCAM_IDPROP_EXPOSURETIME, exposure);
  }
  double get_sensitivity(void) {
    return this->get_value(DCAM_IDPROP_SENSITIVITY);
  }
  void set_sensitivity(double sensitivity) {
    this->set_value(DCAM_IDPROP_SENSITIVITY, sensitivity);
  }
  int get_sensor_width(void) {
    return this->get_value(DCAM_IDPROP_IMAGE_WIDTH);
  }
  int get_sensor_height(void) {
    return this->get_value(DCAM_IDPROP_IMAGE_HEIGHT);
  }
  int get_width(void) {
    return this->get_value(DCAM_IDPROP_SUBARRAYHSIZE);
  }
  void set_width(int width) {
    this->set_value(DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__OFF);
    this->set_value(DCAM_IDPROP_SUBARRAYVSIZE, width);
    this->set_value(DCAM_IDPROP_SUBARRAYVPOS, (this->get_sensor_width() - width) / 2);
    this->set_value(DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
  }
  int get_height(void) {
    return this->get_value(DCAM_IDPROP_SUBARRAYVSIZE);
  }
  void set_height(int height) {
    this->set_value(DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__OFF);
    this->set_value(DCAM_IDPROP_SUBARRAYVSIZE, height);
    this->set_value(DCAM_IDPROP_SUBARRAYVPOS, (this->get_sensor_height() - height) / 2);
    this->set_value(DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
  }
  double get_sensor_temperature(void) {
    return this->get_value(DCAM_IDPROP_SENSORTEMPERATURE);
  }
  void stop(void) {
    dcamcap_stop(this->hdcam);
  }
  void close(void) {
    dcambuf_release(this->hdcam);
    dcamwait_close(this->wait_handle);
    dcamdev_close(this->hdcam);
  };
private:
  std::string get_string(int32 param_id);
  double get_value(int32 prop_id) {
    double value;
    THROW_IF_ERROR(dcamprop_getvalue(this->hdcam, prop_id, &value))
    return value;
  };
  void set_value(int32 prop_id, double value) {
    THROW_IF_ERROR(dcamprop_setvalue(this->hdcam, prop_id, value))
  }
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

#endif // CAMERA_H
