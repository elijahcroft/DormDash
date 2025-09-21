# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD rpi_pico)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(dorm_dash_robot)

target_sources(app PRIVATE 
	src/main.c
	src/leds.c
	# src/distance_sensor.c
	src/pwm_motor.c
)
