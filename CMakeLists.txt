cmake_minimum_required(VERSION 3.14)

project(MyProject LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES
	"src/*.c"
	"src/*.cpp"
	"src/*.h"
	"src/*.hpp"
	"src/*.s"
	"src/*.S"
	"src/*.ld"
	"src/*.ld.S"
	"src/*.ld.s"
	"src/*.ld.cpp"
	"src/*.ld.c"
	"src/*.ld.h"
	"src/*.ld.hpp"
)

add_executable(MyFirmware ${SOURCES})

# debug mode
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR:INTERNAL "Ninja")

set_target_properties(MyFirmware
    PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
	RUEL_MESSAGES OFF
)

# Add LVGL as a subdirectory
add_subdirectory(lvgl)
message("-- LVGL_INCLUDE_DIRS: ${LVGL_INCLUDE_DIRS}")

# Add LVGL's include directory to the include path
target_include_directories(MyFirmware PRIVATE ${LVGL_INCLUDE_DIRS})

# add lv_drivers
add_subdirectory(lv_drivers)

# Add LVGL's drivers include directory to the include path
target_include_directories(MyFirmware PRIVATE ${LV_DRIVERS_PUBLIC_HEADERS})

target_link_libraries(MyFirmware PRIVATE lv_drivers)
target_include_directories(MyFirmware PRIVATE lv_drivers)

find_package(SDL2 REQUIRED)
target_link_libraries(MyFirmware PRIVATE SDL2::SDL2)

# make definition for LV_CONF_INCLUDE_SIMPLE
target_compile_definitions(MyFirmware PRIVATE LV_CONF_INCLUDE_SIMPLE)

# The target "MyFirmware" depends on LVGL
target_link_libraries(MyFirmware PRIVATE lvgl)
