#include "pwm_motor.h"

#include <stdlib.h>

LOG_MODULE_REGISTER(pwm_motor);

#define PWM_NODE DT_NODELABEL(motor_pwms)
#define MAX_PULSE_NSEC DT_PROP(PWM_NODE, max_pulse)

#define MOTOR_COUNT 2

struct pwm_motor
{
	const struct pwm_dt_spec motor_forward_spec;
	const struct pwm_dt_spec motor_backward_spec;
	const long max_pulse_nsec;

	int current_duty_cycle;
	int current_direction;

	char* name;
};

struct pwm_motor motors[MOTOR_COUNT] = {
	{
		.name = "motor_left",
		.motor_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor0_forward),
		.motor_backward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor0_backward),
		.max_pulse_nsec = MAX_PULSE_NSEC
	},
	{
		.name = "motor_right",
		.motor_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor1_forward),
		.motor_backward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor1_backward),
		.max_pulse_nsec = MAX_PULSE_NSEC
	},
};

static bool check_pwm_devices_ready() {

	for (int i = 0; i < MOTOR_COUNT; i++)
	{
		if (!pwm_is_ready_dt(&motors[i].motor_forward_spec)
			|| !pwm_is_ready_dt(&motors[i].motor_backward_spec))
		{
			LOG_ERR("PWM device %s not ready!", motors[i].name);
			return false;
		}
	}

	return true;
}

int PwmMotor_SetDutyCycle(enum MotorType type, int dutyCycle)
{
	int ret;
	int pulse_width_ns = 0;
	struct pwm_motor* motor = &motors[type];

	pulse_width_ns = (abs(dutyCycle) / 100.0f) * motor->max_pulse_nsec;

	// TODO - do we need delay between dir switches to prevent short?????
	if (dutyCycle > 0)
	{
		ret = pwm_set_pulse_dt(&motor->motor_backward_spec, 0);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		k_sleep(K_MSEC(1));

		ret = pwm_set_pulse_dt(&motor->motor_forward_spec, pulse_width_ns);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		motor->current_direction = 1;
	}
	else if (dutyCycle < 0)
	{
		ret = pwm_set_pulse_dt(&motor->motor_forward_spec, 0);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		k_sleep(K_MSEC(1));

		ret = pwm_set_pulse_dt(&motor->motor_backward_spec, pulse_width_ns);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		motor->current_direction = -1;
	}
	else
	{
		ret = pwm_set_pulse_dt(&motor->motor_forward_spec, 0);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		ret = pwm_set_pulse_dt(&motor->motor_backward_spec, 0);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		motor->current_direction = 0;
	}

	return 0;
}

int PWM_Start(void)
{
	if (check_pwm_devices_ready() == false) {
		LOG_ERR("One or more PWM devices not ready!\n");
		return -1;
	}

	//pwm_set_pulse_dt(&motors[MOTOR_TYPE_LEFT].motor_backward_spec, 25000);
	//pwm_set_pulse_dt(&motors[MOTOR_TYPE_LEFT].motor_forward_spec, 25000);
	//pwm_set_pulse_dt(&motors[MOTOR_TYPE_RIGHT].motor_backward_spec, 25000);
	//pwm_set_pulse_dt(&motors[MOTOR_TYPE_RIGHT].motor_forward_spec, 25000);

	return 0;
}
