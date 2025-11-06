# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD rpi_pico)

list(APPEND ZEPHYR_EXTRA_MODULES
	"${CMAKE_SOURCE_DIR}/../../modules/lib/zenoh-pico"
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(dorm_dash_robot)

#set(ZEPHYR_GENERATED_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/zephyr/include/generated/zephyr/version.h")

target_sources(app PRIVATE
	src/main.c
	src/leds.c
#	src/distance_sensor.c
	src/pwm_motor.c
	src/quadrature_decoder.c
	src/motor_control.c
	src/pid.c
	src/uart_echo.c
	src/zenoh_pub.c
)

# ZENOH ----------------------------------------------------------------------------

# Path to zenoh-pico module relative to this app root (adjust if needed)
set(ZENOH_PICO_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../modules/lib/zenoh-pico")
if(NOT EXISTS "${ZENOH_PICO_MODULE_PATH}/CMakeLists.txt")
  message(FATAL_ERROR "zenoh-pico module not found at ${ZENOH_PICO_MODULE_PATH}")
endif()

# Force module options appropriate for Zephyr builds
set(WITH_ZEPHYR ON CACHE BOOL "Build zenoh-pico for Zephyr" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared libs for Zephyr" FORCE)
set(CHECK_THREADS OFF CACHE BOOL "Disable Threads check for embedded/Zephyr builds" FORCE)

# Add the module as a subdirectory; run its configure and generate steps
add_subdirectory("${ZENOH_PICO_MODULE_PATH}" zenohpico_build)

# Module build dir created by add_subdirectory above (second arg)
set(ZENOH_PICO_BUILD_DIR "${CMAKE_BINARY_DIR}/zenohpico_build")

# Zephyr generated include dir that contains version.h
set(ZEPHYR_GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/zephyr/include/generated/zephyr")

# Add all likely include paths so generated headers like version.h are found
target_include_directories(app PRIVATE
  "${ZENOH_PICO_MODULE_PATH}/include"
  "${ZENOH_PICO_MODULE_PATH}/include/zenoh-pico"
  "${ZENOH_PICO_BUILD_DIR}"
  "${ZEPHYR_GENERATED_INCLUDE_DIR}"
)

# Prefer the exported package target if available so interface props propagate
if(TARGET zenohpico::lib)
  target_link_libraries(app PRIVATE zenohpico::lib)
elseif(TARGET zenohpico::static)
  target_link_libraries(app PRIVATE zenohpico::static)
elseif(TARGET zenohpico_static)
  target_link_libraries(app PRIVATE zenohpico_static)
else()
  message(FATAL_ERROR "zenoh-pico CMake target not found after add_subdirectory. Inspect ${ZENOH_PICO_MODULE_PATH}/CMakeLists.txt for exported target names.")
endif()

if(NOT EXISTS "${ZEPHYR_GENERATED_INCLUDE_DIR}/version.h")
  message(WARNING "ZENOD_VERSION_H not found!!!!AAAAAAAAAAAAAAAAAAAAAAAAAAAAA ${ZEPHYR_GENERATED_INCLUDE_DIR}/version.h")
endif()

# ------------------------------------------------------------------------------------
