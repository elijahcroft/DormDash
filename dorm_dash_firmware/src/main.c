#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "leds.h"
#include "distance_sensor.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"
#include "motor_control.h"
#include "tests.h"

#include "uart_echo.h"

LOG_MODULE_REGISTER(main);

#define TESTS_ENABLED true

int main(void)
{
	if (LEDS_Init() == -1) {
		LOG_ERR("Unable to initialize LEDS!");
	}

    if (TESTS_ENABLED) {
        Tests_Start();
    }

	//LIDAR_Start();

	// if (LIDAR_Start() == -1) {
	// 	LOG_ERR("Unable to initialize LiDAR sensors!");
	// }

	//PWM_Start();
	//QuadratureDecoder_Start();
	//MotorControl_Start();

	//UART_Start();

	while (true) {
		k_sleep(K_SECONDS(2));
	}
}
