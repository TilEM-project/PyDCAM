#include <pybind11/pybind11.h>
#include "camera.hpp"
#include "exceptions.hpp"
#include "wrap_enums.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_PyDCAM, m) {
  m.doc() = "Python bindings for the Hamamatsu DCAM API.";

  py::class_<DCAM>(m, "DCAM")
    .def(py::init())
    .def("close", &DCAM::close)
    .def_property_readonly("num_cameras", &DCAM::get_num_cameras);
    // .def("get_camera", &DCAM::get_camera, py::arg("device_ind"))
    // .def("__enter__", [](DCAM &d){ return d; })
    // .def("__exit__", [](
    //   DCAM &d,
    //   py::object exc_type,
    //   py::object exc_value,
    //   py::object traceback
    // ){ d.close(); });

  py::class_<Camera>(m, "Camera")
    .def(py::init<int>(), py::arg("device_ind"))
    .def("allocate", &Camera::allocate, py::arg("framecount"))
    .def("start", &Camera::start, py::arg("type"))
    .def("trigger", &Camera::trigger)
    .def("wait", &Camera::wait, py::arg("event"), py::arg("timeout"))
    .def_property_readonly("frame_count", &Camera::get_frame_count)
    .def_property_readonly("newest_frame_ind", &Camera::get_newest_frame_ind)
    .def_property_readonly("status", &Camera::get_status)
    .def("copyframe", &Camera::copyframe, py::arg("frame_ind") = -1, py::arg("width") = 0, py::arg("height") = 0, py::arg("left") = 0, py::arg("top") = 0)
    .def("lockframe", &Camera::lockframe, py::arg("frame_ind") = -1)
    .def("stop", &Camera::stop)
    .def("close", &Camera::close)
    .def("get_string", &Camera::get_string, py::arg("prop_id"))
    .def("get_attrs", &Camera::get_attrs, py::arg("param_id"))
    .def("get_value", [](Camera &c, DCAMIDPROP prop_id){
      switch (c.get_attrs(prop_id).get_type()) {
        case DCAMPROP_TYPE_REAL:
          return py::cast(c.get_value_real(prop_id));
        case DCAMPROP_TYPE_LONG:
          return py::cast(c.get_value_long(prop_id));
        case DCAMPROP_TYPE_MODE:
          return py::cast(c.get_value_mode(prop_id));
        default:
          throw DCAMException("Value is of unknown type!");
      }
    }, py::arg("param_id"))
    .def("set_value", py::overload_cast<DCAMIDPROP, double>(&Camera::set_value), py::arg("param_id"), py::arg("value"))
    .def("set_value", py::overload_cast<DCAMIDPROP, int32>(&Camera::set_value), py::arg("param_id"), py::arg("value"))
    .def("set_value", py::overload_cast<DCAMIDPROP, DCAMPROPMODEVALUE>(&Camera::set_value), py::arg("param_id"), py::arg("value"));
    // .def("__enter__", [](Camera &c){ return c; })
    // .def("__exit__", [](
    //   Camera &c,
    //   py::object exc_type,
    //   py::object exc_value,
    //   py::object traceback
    // ){ c.close(); });

  py::class_<Frame>(m, "Frame", py::buffer_protocol())
    .def_property_readonly("width", &Frame::get_width)
    .def_property_readonly("height", &Frame::get_height)
    .def_property_readonly("row_bytes", &Frame::get_row_bytes)
    .def_property_readonly("pixel_type", &Frame::get_pixel_type)
    .def_property_readonly("timestamp", &Frame::get_timestamp)
    .def_property_readonly("framestamp", &Frame::get_framestamp)
    .def_property_readonly("camerastamp", &Frame::get_camerastamp)
    .def_property_readonly("frame_ind", &Frame::get_frame_ind)
    .def_buffer([](Frame &f) -> py::buffer_info {
      switch (f.get_pixel_type()) {
        case DCAM_PIXELTYPE_MONO8:
          return py::buffer_info(
            f.get_data(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            2,
            {f.get_height(), f.get_width()},
            {sizeof(uint8_t) * f.get_width(), sizeof(uint8_t)}
          );
        case DCAM_PIXELTYPE_MONO12: case DCAM_PIXELTYPE_MONO12P:
          throw DCAMException("12 Bit format unsupported!");
        case DCAM_PIXELTYPE_MONO16:
          return py::buffer_info(
            f.get_data(),
            sizeof(uint16_t),
            py::format_descriptor<uint16_t>::format(),
            2,
            {f.get_height(), f.get_width()},
            {sizeof(uint16_t) * f.get_width(), sizeof(uint16_t)}
          );
        case DCAM_PIXELTYPE_BGR24: case DCAM_PIXELTYPE_RGB24:
          return py::buffer_info(
            f.get_data(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            3,
            {f.get_height(), f.get_width(), 3},
            {sizeof(uint8_t) * f.get_width() * 3, sizeof(uint8_t) * 3, sizeof(uint8_t)}
          );
        case DCAM_PIXELTYPE_BGR48: case DCAM_PIXELTYPE_RGB48:
          return py::buffer_info(
            f.get_data(),
            sizeof(uint16_t),
            py::format_descriptor<uint16_t>::format(),
            3,
            {f.get_height(), f.get_width(), 3},
            {sizeof(uint16_t) * f.get_width() * 3, sizeof(uint16_t) * 3, sizeof(uint16_t)}
          );
        default: throw DCAMException("Unknown pixel type!");
      }
    });

  py::class_<Attrs>(m, "Attrs")
    .def_property_readonly("prop_id", &Attrs::get_prop_id)
    .def("get_attr", py::overload_cast<DCAMPROPATTRIBUTE>(&Attrs::get_attribute), py::arg("attr"))
    .def("get_attr", py::overload_cast<DCAMPROPATTRIBUTE2>(&Attrs::get_attribute), py::arg("attr"))
    .def_property_readonly("unit", &Attrs::get_unit)
    .def_property_readonly("min", &Attrs::get_min)
    .def_property_readonly("max", &Attrs::get_max)
    .def_property_readonly("step", &Attrs::get_step)
    .def_property_readonly("default", &Attrs::get_default)
    .def_property_readonly("max_channel", &Attrs::get_max_channel)
    .def_property_readonly("max_view", &Attrs::get_max_view)
    .def_property_readonly("num_elements_prop_id", &Attrs::get_num_elements_prop_id)
    .def_property_readonly("array_base_prop_id", &Attrs::get_array_base_prop_id)
    .def_property_readonly("next_prop_id", &Attrs::get_next_prop_id)
    .def_property_readonly("has_range", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_HASRANGE); })
    .def_property_readonly("hsa_step", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_HASSTEP); })
    .def_property_readonly("has_default", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_HASDEFAULT); })
    .def_property_readonly("autorounding", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_AUTOROUNDING); })
    .def_property_readonly("stepping_inconsistent", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_STEPPING_INCONSISTENT); })
    .def_property_readonly("volatile", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_VOLATILE); })
    .def_property_readonly("writable", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_WRITABLE); })
    .def_property_readonly("readable", [](Attrs &a){ return a.get_attribute(DCAMPROP_ATTR_READABLE); })
    .def_property_readonly("type", &Attrs::get_type);

  py::register_local_exception<DCAMException>(m, "DCAMException");

  wrap_enums(m);
}
