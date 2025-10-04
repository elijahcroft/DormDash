#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "leds.h"
#include "distance_sensor.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"


LOG_MODULE_REGISTER(main);

int main(void)
{
	if (LEDS_Init() == -1) {
		LOG_ERR("Unable to initialize LEDS!");
	}

	// if (LIDAR_Start() == -1) {
	// 	LOG_ERR("Unable to initialize LiDAR sensors!");
	// }

	// PWM_Start();

	QuadratureDecoder_Start();

	while (true) {
		// LOG_INF("Hello World!");
		k_sleep(K_SECONDS(2));
	}
}
