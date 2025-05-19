cmake_minimum_required(VERSION 3.11)

project(PyDCAM VERSION 0.1.0 LANGUAGES CXX)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(FATAL_ERROR "Only Linux is supported!")
endif()

include(FetchContent)

FetchContent_Declare(
  dcam_api_linux
  URL https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/static/sys/dcam-api-for-linux/tar-gz/DCAM-API_Lite_for_Linux_v24.12.6898.tar.gz
)
FetchContent_Declare(
  dcam_sdk
  URL https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/static/sys/dcam-sdk/tar-gz/Hamamatsu_DCAMSDK4_v25056964.tar.gz
)

FetchContent_Populate(dcam_api_linux)
FetchContent_Populate(dcam_sdk)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(dcam_arch x86_64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(dcam_arch i386)
else()
  message(FATAL_ERROR "Unknown architecture!")
endif()

set(dcam_lib_dir ${dcam_api_linux_SOURCE_DIR}/api/runtime/${dcam_arch}/core)
set(dcam_lib dcamapi)
file(GLOB dcam_lib_path ${dcam_lib_dir}/lib${dcam_lib}.so*)
cmake_path(GET dcam_lib_path FILENAME dcam_lib_filename)
string(REGEX REPLACE "lib${dcam_lib}\\.so(\\.?[0-9]*)[\\.0-9]*" "lib${dcam_lib}.so\\1" dcam_lib_install ${dcam_lib_filename})
file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${dcam_sdk_SOURCE_DIR}/dcamsdk4/inc)
link_directories(${dcam_lib_dir})

find_package(pybind11 CONFIG)
if(NOT pybind11_FOUND)
    execute_process(COMMAND pybind11-config --cmake OUTPUT_VARIABLE PYBIND_DIR)
    find_package(pybind11 CONFIG REQUIRED PATHS ${PYBIND_DIR}/..)
endif()

pybind11_add_module(_PyDCAM MODULE ${src})
target_link_libraries(
    _PyDCAM
    PRIVATE
    ${dcam_lib_path}
)

set_target_properties(_PyDCAM PROPERTIES INSTALL_RPATH "$ORIGIN" INSTALL_RPATH_USE_LINK_PATH false BUILD_WITH_INSTALL_RPATH true)

install(TARGETS _PyDCAM DESTINATION .)
install(FILES ${dcam_lib_path} DESTINATION . RENAME ${dcam_lib_install})
