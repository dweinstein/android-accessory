cmake_minimum_required(VERSION 2.6)
project (accessory-mode)

# use local module (for Findlibusb-1.0.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package (libusb-1.0 REQUIRED)
if (NOT LIBUSB_1_FOUND)
  message(FATAL_ERROR "libusb-1.0 not installed. (try apt-get install libusb-1.0-0-dev)")
endif (NOT LIBUSB_1_FOUND)

include_directories ("${LIBUSB_1_INCLUDE_DIRS}")
set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_executable(accessory-mode src/generic.c)
target_link_libraries(accessory-mode ${LIBS})
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-Wall")
