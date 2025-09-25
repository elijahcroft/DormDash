#include "pwm_motor.h"

LOG_MODULE_REGISTER(pwm_motor);

#define PWM_NODE DT_NODELABEL(motor_pwms)
#define MIN_PULSE_NS DT_PROP(PWM_NODE, min_pulse)
#define MAX_PULSE_NS DT_PROP(PWM_NODE, max_pulse)
#define STEP PWM_USEC(5)

static const struct pwm_dt_spec motor0_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor0);
static const struct pwm_dt_spec motor1_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor1);
static const struct pwm_dt_spec motor2_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor2);
static const struct pwm_dt_spec motor3_forward_spec = PWM_DT_SPEC_GET_BY_NAME(PWM_NODE, motor3);

enum direction {
	DOWN,
	UP,
};

static bool check_pwm_devices_ready() {
	if (!pwm_is_ready_dt(&motor0_forward_spec)) {
		LOG_ERR("0PWM device not ready!\n");
		return false;
	}

	if (!pwm_is_ready_dt(&motor1_forward_spec)) {
		LOG_ERR("1PWM device not ready!\n");
		return false;
	}

	if (!pwm_is_ready_dt(&motor2_forward_spec)) {
		LOG_ERR("2PWM device not ready!\n");
		return false;
	}

	if (!pwm_is_ready_dt(&motor3_forward_spec)) {
		LOG_ERR("3PWM device not ready!\n");
		return false;
	}

	return true;
}


int PWM_Start(void)
{
	uint32_t pulse_width = MIN_PULSE_NS;
	enum direction dir = UP;
	int ret;

	if (check_pwm_devices_ready() == false) {
		LOG_ERR("One or more PWM devices not ready!\n");
		return -1;
	}

	while (true) {

		ret = pwm_set_pulse_dt(&motor3_forward_spec, pulse_width);
		if (ret < 0) {
			LOG_ERR("Error %d: failed to set pulse width\n", ret);
			return -1;
		}

		if (dir == DOWN) {
			if (pulse_width <= MIN_PULSE_NS) {
				dir = UP;
				pulse_width = MIN_PULSE_NS;
			} else {
				pulse_width -= STEP;
			}
		} else {
			pulse_width += STEP;

			if (pulse_width >= MAX_PULSE_NS) {
				dir = DOWN;
				pulse_width = MAX_PULSE_NS;
			}
		}

		k_msleep(50);
	}
	return 0;
}
