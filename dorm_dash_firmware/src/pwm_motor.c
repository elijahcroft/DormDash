#include "pwm_motor.h"

LOG_MODULE_REGISTER(pwm_motor);

#define PWM_DEV_NODE DT_PATH(soc, pwm_40050000)

static const struct device *const pwn_dev = DEVICE_DT_GET(PWM_DEV_NODE);

#define PWM_PERIOD_USEC 50 // 50 micro-sec = 20k HZ

static const struct pwm_dt_spec motor0_forward_spec = {
	.dev = pwn_dev,
	.channel = 0,
	.period = PWM_USEC(PWM_PERIOD_USEC),
	.flags = PWM_POLARITY_NORMAL
};

static const struct pwm_dt_spec motor1_forward_spec = {
	.dev = pwn_dev,
	.channel = 1,
	.period = PWM_USEC(PWM_PERIOD_USEC),
	.flags = PWM_POLARITY_NORMAL
};

static const struct pwm_dt_spec motor2_forward_spec = {
	.dev = pwn_dev,
	.channel = 9,
	.period = PWM_USEC(PWM_PERIOD_USEC),
	.flags = PWM_POLARITY_NORMAL
};

static const struct pwm_dt_spec motor3_forward_spec = {
	.dev = pwn_dev,
	.channel = 8,
	.period = PWM_USEC(PWM_PERIOD_USEC),
	.flags = PWM_POLARITY_NORMAL
};


static const uint32_t min_pulse = PWM_USEC(0);
static const uint32_t max_pulse = PWM_USEC(50);

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

#define STEP PWM_USEC(5)

int PWM_Start(void)
{
	uint32_t pulse_width = min_pulse;
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
			if (pulse_width <= min_pulse) {
				dir = UP;
				pulse_width = min_pulse;
			} else {
				pulse_width -= STEP;
			}
		} else {
			pulse_width += STEP;

			if (pulse_width >= max_pulse) {
				dir = DOWN;
				pulse_width = max_pulse;
			}
		}

		k_sleep(K_MSEC(50));
	}
	return 0;
}
