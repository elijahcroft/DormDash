#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "leds.h"
#include "distance_sensor.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"
#include "motor_control.h"

#include "uart_echo.h"

LOG_MODULE_REGISTER(main);

int main(void)
{
	if (LEDS_Init() == -1) {
		LOG_ERR("Unable to initialize LEDS!");
	}

	//LIDAR_Start();

	// if (LIDAR_Start() == -1) {
	// 	LOG_ERR("Unable to initialize LiDAR sensors!");
	// }

	//PWM_Start();
	//QuadratureDecoder_Start();
	//MotorControl_Start();

	UART_Start();

	while (true) {
		k_sleep(K_SECONDS(2));
	}
}
