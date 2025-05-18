#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#define MSG_LEN 256

#include <string>
#include <stdexcept>
#include <sstream>
#include "dcamapi4.h"

inline std::string error_msg(DCAMERR error) {
  switch (error) {
    case DCAMERR_BUSY: return "DCAMERR_BUSY: device status is DCAM_STATUS_BUSY.";
    case DCAMERR_NOTREADY: return "DCAMERR_NOTREADY: device status is not DCAM_STATUS_READY.";
    case DCAMERR_NOTSTABLE: return "DCAMERR_NOTSTABLE: device status is not DCAM_STATUS_STABLE.";
    case DCAMERR_UNSTABLE: return "DCAMERR_UNSTABLE: device status is not DCAM_STATUS_STABLE.";
    case DCAMERR_NOTBUSY: return "DCAMERR_NOTBUSY: device status is not DCAM_STATUS_BUSY.";
    case DCAMERR_EXCLUDED: return "DCAMERR_EXCLUDED: some resource is exclusive and already used";
    case DCAMERR_COOLINGTROUBLE: return "DCAMERR_COOLINGTROUBLE: a problem occurred with the cooler";
    case DCAMERR_NOTRIGGER: return "DCAMERR_NOTRIGGER: no trigger was received when expected. Some devices support this error.";
    case DCAMERR_TEMPERATURE_TROUBLE: return "DCAMERR_TEMPERATURE_TROUBLE: temperature warning on the device";
    case DCAMERR_TOOFREQUENTTRIGGER: return "DCAMERR_TOOFREQUENTTRIGGER: input trigger is too frequent. Some devices support this error.";
    case DCAMERR_ABORT: return "DCAMERR_ABORT: process was aborted";
    case DCAMERR_TIMEOUT: return "DCAMERR_TIMEOUT: process has timed out";
    case DCAMERR_LOSTFRAME: return "DCAMERR_LOSTFRAME: frame data has been lost";
    case DCAMERR_MISSINGFRAME_TROUBLE: return "DCAMERR_MISSINGFRAME_TROUBLE: frame data has been lost due to problems in the low level driver";
    case DCAMERR_INVALIDIMAGE: return "DCAMERR_INVALIDIMAGE: hpk format data is invalid data";
    case DCAMERR_NORESOURCE: return "DCAMERR_NORESOURCE: not enough resources other than memory";
    case DCAMERR_NOMEMORY: return "DCAMERR_NOMEMORY: not enough memory";
    case DCAMERR_NOMODULE: return "DCAMERR_NOMODULE: no sub module";
    case DCAMERR_NODRIVER: return "DCAMERR_NODRIVER: no driver";
    case DCAMERR_NOCAMERA: return "DCAMERR_NOCAMERA: no device";
    case DCAMERR_NOGRABBER: return "DCAMERR_NOGRABBER: no grabber";
    case DCAMERR_FAILOPEN: return "DCAMERR_FAILOPEN: Fail to open";
    case DCAMERR_INVALIDMODULE: return "DCAMERR_INVALIDMODULE: dcamapi_init() function found invalid module";
    case DCAMERR_INVALIDCOMMPORT: return "DCAMERR_INVALIDCOMMPORT: invalid serial port";
    case DCAMERR_FAILOPENBUS: return "DCAMERR_FAILOPENBUS: the bus or driver are not available";
    case DCAMERR_FAILOPENCAMERA: return "DCAMERR_FAILOPENCAMERA: device report error during opening";
    case DCAMERR_DEVICEPROBLEM: return "DCAMERR_DEVICEPROBLEM: Device has the trobule.";
    case DCAMERR_FRAMEGRABBER_NEEDS_FIRMWAREUPDATE: return "DCAMERR_FRAMEGRABBER_NEEDS_FIRMWAREUPDATE: Need to update the firmware of frame grabber.";
    case DCAMERR_INVALIDCAMERA: return "DCAMERR_INVALIDCAMERA: invalid device";
    case DCAMERR_INVALIDHANDLE: return "DCAMERR_INVALIDHANDLE: invalid device handle";
    case DCAMERR_INVALIDPARAM: return "DCAMERR_INVALIDPARAM: invalid parameter";
    case DCAMERR_INVALIDVALUE: return "DCAMERR_INVALIDVALUE: invalid property value";
    case DCAMERR_OUTOFRANGE: return "DCAMERR_OUTOFRANGE: value is out of range";
    case DCAMERR_NOTWRITABLE: return "DCAMERR_NOTWRITABLE: the property is not writable";
    case DCAMERR_NOTREADABLE: return "DCAMERR_NOTREADABLE: the property is not readable";
    case DCAMERR_INVALIDPROPERTYID: return "DCAMERR_INVALIDPROPERTYID: the property id is invalid";
    case DCAMERR_NEWAPIREQUIRED: return "DCAMERR_NEWAPIREQUIRED: action requires use of newer API version";
    case DCAMERR_WRONGHANDSHAKE: return "DCAMERR_WRONGHANDSHAKE: DCAM received an error code from the device unexpectedly";
    case DCAMERR_NOPROPERTY: return "DCAMERR_NOPROPERTY: there is no alternative or influence id, or no more property id";
    case DCAMERR_INVALIDCHANNEL: return "DCAMERR_INVALIDCHANNEL: the property id specifies invalid channel";
    case DCAMERR_INVALIDVIEW: return "DCAMERR_INVALIDVIEW: the property id specifies invalid view";
    case DCAMERR_INVALIDSUBARRAY: return "DCAMERR_INVALIDSUBARRAY: the combination of subarray values are invalid. e.g. DCAM_IDPROP_SUBARRAYHPOS + DCAM_IDPROP_SUBARRAYHSIZE is greater than the number of horizontal pixel of sensor.";
    case DCAMERR_ACCESSDENY: return "DCAMERR_ACCESSDENY: the property cannot be accessed during this DCAM STATUS";
    case DCAMERR_NOVALUETEXT: return "DCAMERR_NOVALUETEXT: the property does not have value text";
    case DCAMERR_WRONGPROPERTYVALUE: return "DCAMERR_WRONGPROPERTYVALUE: at least one property value is wrong";
    case DCAMERR_DISHARMONY: return "DCAMERR_DISHARMONY: the paired devices do not have the same parameters";
    case DCAMERR_FRAMEBUNDLESHOULDBEOFF: return "DCAMERR_FRAMEBUNDLESHOULDBEOFF: framebundle mode should be OFF under current property settings";
    case DCAMERR_INVALIDFRAMEINDEX: return "DCAMERR_INVALIDFRAMEINDEX: the frame index is invalid";
    case DCAMERR_INVALIDSESSIONINDEX: return "DCAMERR_INVALIDSESSIONINDEX: the session index is invalid";
    case DCAMERR_NOCORRECTIONDATA: return "DCAMERR_NOCORRECTIONDATA: dark or shading correction data has not been taken yet.";
    case DCAMERR_NOTSUPPORT: return "DCAMERR_NOTSUPPORT: device does not support the function or property with current settings";
    case DCAMERR_CHANNELDEPENDENTVALUE: return "DCAMERR_CHANNELDEPENDENTVALUE: each channel has own property value so can't return overall property value.";
    case DCAMERR_VIEWDEPENDENTVALUE: return "DCAMERR_VIEWDEPENDENTVALUE: each view has own property value so can't return overall property value.";
    case DCAMERR_NODEVICEBUFFER: return "DCAMERR_NODEVICEBUFFER: Frame count is larger than device buffer size.";
    case DCAMERR_REQUIREDSNAP: return "DCAMERR_REQUIREDSNAP: Need to set snap as capture mode on using device memory.";
    // case DCAMERR_INVALIDCALIBSETTING: return "DCAMERR_INVALIDCALIBSETTING: Setting of properties is invalid on sampling calibraiton data.";
    case DCAMERR_LESSSYSTEMMEMORY: return "DCAMERR_LESSSYSTEMMEMORY: System memory size is too small to control camera. PC doesn't have enough memory or is limited memory by WOW64.";
    case DCAMERR_INVALID_REALTIMEGAINCORRECTREGIONS: return "DCAMERR_INVALID_REALTIMEGAINCORRECTREGIONS: the combination of hpos and hsize for correct region of realtime gain are invalid. e.g. DCAM_IDPROP_REALTIMECORRECTREGION_HPOS + DCAM_IDPROP_REALTIMECORRECTREGION_HSIZE is grater than the number of horizontal pixel of sensor.";
    case DCAMERR_FAILREADCAMERA: return "DCAMERR_FAILREADCAMERA: Receiving or reading register was failed.";
    case DCAMERR_FAILWRITECAMERA: return "DCAMERR_FAILWRITECAMERA: Sending or writing register was failed.";
    case DCAMERR_CONFLICTCOMMPORT: return "DCAMERR_CONFLICTCOMMPORT: Comm port setting is conflict";
    case DCAMERR_OPTICS_UNPLUGGED: return "DCAMERR_OPTICS_UNPLUGGED: Optics part is unplugged";
    case DCAMERR_FAILCALIBRATION: return "DCAMERR_FAILCALIBRATION: fail calibration";
    case DCAMERR_MISMATCH_CONFIGURATION: return "DCAMERR_MISMATCH_CONFIGURATION: Mismatch between camera output(connection) and frame grabber specification.";
    // case DCAMERR_INVALIDMEMBER_x: return "DCAMERR_INVALIDMEMBER_x: x-th member variable is invalid value.";
    case DCAMERR_FAILEDOPENRECFILE: return "DCAMERR_FAILEDOPENRECFILE: Failed to open recording file";
    case DCAMERR_INVALIDRECHANDLE: return "DCAMERR_INVALIDRECHANDLE: Recording handle is invalid";
    case DCAMERR_FAILEDWRITEDATA: return "DCAMERR_FAILEDWRITEDATA: Failed to write data into recording file";
    case DCAMERR_FAILEDREADDATA: return "DCAMERR_FAILEDREADDATA: Failed to read data from recording file";
    case DCAMERR_NOWRECORDING: return "DCAMERR_NOWRECORDING: Device is now recording";
    case DCAMERR_WRITEFULL: return "DCAMERR_WRITEFULL: There is no space to write to target";
    case DCAMERR_ALREADYOCCUPIED: return "DCAMERR_ALREADYOCCUPIED: Recording file is already attached to different HDCAM";
    case DCAMERR_TOOLARGEUSERDATASIZE: return "DCAMERR_TOOLARGEUSERDATASIZE: Specified user data size is too large";
    case DCAMERR_INVALIDWAITHANDLE: return "DCAMERR_INVALIDWAITHANDLE: Invalid wait handle";
    case DCAMERR_NEWRUNTIMEREQUIRED: return "DCAMERR_NEWRUNTIMEREQUIRED: DCAM module version is older than the version that the camera requests";
    case DCAMERR_VERSIONMISMATCH: return "DCAMERR_VERSIONMISMATCH: camera returns the error on setting parameter to limit version";
    case DCAMERR_RUNAS_FACTORYMODE: return "DCAMERR_RUNAS_FACTORYMODE: Camera runs as factory mode. Camera may need to be rewritten with firmware.";
    case DCAMERR_IMAGE_UNKNOWNSIGNATURE: return "DCAMERR_IMAGE_UNKNOWNSIGNATURE: signature of image header is unknown or corrupted";
    case DCAMERR_IMAGE_NEWRUNTIMEREQUIRED: return "DCAMERR_IMAGE_NEWRUNTIMEREQUIRED: version of image header is newer than version that used DCAM supports";
    case DCAMERR_IMAGE_ERRORSTATUSEXIST: return "DCAMERR_IMAGE_ERRORSTATUSEXIST: image header stands error status";
    case DCAMERR_IMAGE_HEADERCORRUPTED: return "DCAMERR_IMAGE_HEADERCORRUPTED: image header value is strange";
    case DCAMERR_IMAGE_BROKENCONTENT: return "DCAMERR_IMAGE_BROKENCONTENT: image content is corrupted";
    case DCAMERR_UNKNOWNMSGID: return "DCAMERR_UNKNOWNMSGID: unknown message id";
    case DCAMERR_UNKNOWNSTRID: return "DCAMERR_UNKNOWNSTRID: unknown string id";
    case DCAMERR_UNKNOWNPARAMID: return "DCAMERR_UNKNOWNPARAMID: unknown parameter id";
    case DCAMERR_UNKNOWNBITSTYPE: return "DCAMERR_UNKNOWNBITSTYPE: unknown bitmap bits type";
    case DCAMERR_UNKNOWNDATATYPE: return "DCAMERR_UNKNOWNDATATYPE: unknown frame data type";
    case DCAMERR_NONE: return "DCAMERR_NONE: no error";
    case DCAMERR_INSTALLATIONINPROGRESS: return "DCAMERR_INSTALLATIONINPROGRESS: installation in progress";
    case DCAMERR_UNREACH: return "DCAMERR_UNREACH: internal error";
    case DCAMERR_UNLOADED: return "DCAMERR_UNLOADED: calling after process terminated";
    case DCAMERR_NOCONNECTION: return "DCAMERR_NOCONNECTION: HDCAM lost connection to device";
    case DCAMERR_NOTIMPLEMENT: return "DCAMERR_NOTIMPLEMENT: not yet implemented";
    case DCAMERR_DELAYEDFRAME: return "DCAMERR_DELAYEDFRAME: Specified frame fails in real-time transfer. this frame is retransferred after stopping acquisition.";
    case DCAMERR_FAILRELOADFRAME: return "DCAMERR_FAILRELOADFRAME: Failed to reload frame from hardware buffer with device buffer mode.";
    case DCAMERR_CANCELRELOADFRAME: return "DCAMERR_CANCELRELOADFRAME: Canceled to reload frame from hardware buffer with device buffer mode.";
    case DCAMERR_DEVICEINITIALIZING: return "DCAMERR_DEVICEINITIALIZING: Device is in the process of being initialized.";
    case DCAMERR_APIINIT_INITOPTIONBYTES: return "DCAMERR_APIINIT_INITOPTIONBYTES: DCAMAPI_INIT::initoptionbytes is wrong";
    case DCAMERR_APIINIT_INITOPTION: return "DCAMERR_APIINIT_INITOPTION: DCAMAPI_INIT::initoption is wrong";
    case DCAMERR_INITOPTION_COLLISION_BASE: return "DCAMERR_INITOPTION_COLLISION_BASE: Between DCAMERR_INITOPTION_COLLISION_BASE and DCAMERR_INITOPTION_COLLISION_MAX means there is collision with initoption in DCAMAPI_INIT. The value \"(error code) - DCAMERR_INITOPTION_COLLISION_BASE\" indicates the index which second INITOPTION group happens.";
    case DCAMERR_INITOPTION_COLLISION_MAX: return "DCAMERR_INITOPTION_COLLISION_MAX: Reserved for indication INITOPTION collision.";
    case DCAMERR_MISSPROP_TRIGGERSOURCE: return "DCAMERR_MISSPROP_TRIGGERSOURCE: Need to set internal or syncreadout　as trigger mode on using device memory.";
    case DCAMERR_SUCCESS: return "DCAMERR_SUCCESS: no error, general success code, app should check the value is positive.";
    default:
      std::stringstream message;
      message << "DCAM error number " << error;
      return message.str();
    }
}


class DCAMException : public std::runtime_error {
public:
    DCAMException(const std::string &what_arg)
        : std::runtime_error(what_arg) {};

    DCAMException(DCAMERR error)
        : std::runtime_error(error_msg(error)) {};
};

#endif // EXCEPTIONS_H
