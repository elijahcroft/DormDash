#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "leds.h"
#include "distance_sensor.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"
#include "motor_control.h"


LOG_MODULE_REGISTER(main);

int main(void)
{
	if (LEDS_Init() == -1) {
		LOG_ERR("Unable to initialize LEDS!");
	}

	LIDAR_Start();

	// if (LIDAR_Start() == -1) {
	// 	LOG_ERR("Unable to initialize LiDAR sensors!");
	// }

	//PWM_Start();
	//QuadratureDecoder_Start();

	//MotorControl_Start();

	while (true) {
		//k_sleep(K_MSEC(500));
		//PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, 0);
		//k_sleep(K_MSEC(500));
		//PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, -50);
		//k_sleep(K_MSEC(500));

		// LOG_INF("Hello World!");
		k_sleep(K_SECONDS(2));
	}
}
