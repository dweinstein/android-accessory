cmake_minimum_required(VERSION 2.6)
project (accessory-mode)

# use local module (for Findlibusb-1.0.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package (libusb-1.0 REQUIRED)
include_directories ("${LIBUSB_1_INCLUDE_DIRS}")
set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_executable(accessory-mode src/generic.c)
target_link_libraries(accessory-mode ${LIBS})


