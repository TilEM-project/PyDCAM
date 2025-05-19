#include <pybind11/pybind11.h>
#include "camera.hpp"
#include "exceptions.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_PyDCAM, m) {
  m.doc() = "Python bindings for the Hamamatsu DCAM API.";

  py::class_<DCAM>(m, "DCAM")
    .def(py::init())
    .def("close", &DCAM::close)
    .def_property_readonly("num_cameras", &DCAM::get_num_cameras)
    .def("get_camera", &DCAM::get_camera, py::arg("device_ind"))
    .def("__enter__", [](const DCAM &d){ return d; })
    .def("__exit__", &DCAM::close);

  py::class_<Camera>(m, "Camera")
    .def("start", &Camera::start)
    .def("capture", &Camera::capture)
    .def_property_readonly("model", &Camera::get_model)
    .def_property_readonly("camera_id", &Camera::get_camera_id)
    .def_property("exposure", &Camera::get_exposure, &Camera::set_exposure)
    .def_property("sensitivity", &Camera::get_sensitivity, &Camera::set_sensitivity)
    .def_property_readonly("sensor_width", &Camera::get_sensor_width)
    .def_property_readonly("sensor_height", &Camera::get_sensor_height)
    .def_property("width", &Camera::get_width, &Camera::set_width)
    .def_property("height", &Camera::get_height, &Camera::set_height)
    .def_property_readonly("get_sensor_temperature", &Camera::get_sensor_temperature)
    .def("stop", &Camera::stop)
    .def("close", &Camera::close)
    .def("__enter__", [](const Camera &c){ return c; })
    .def("__exit__", &Camera::close);

  py::class_<Frame>(m, "Frame", py::buffer_protocol())
    .def_property_readonly("width", &Frame::get_width)
    .def_property_readonly("height", &Frame::get_height)
    .def_buffer([](Frame &f) -> py::buffer_info {
      return py::buffer_info(
        f.get_data(),
        1,
        py::format_descriptor<uint8_t>::format(),
        2,
        {f.get_height(), f.get_width()},
        {f.get_width(), 1}
      );
    });

  py::register_local_exception<DCAMException>(m, "DCAMException");
}
