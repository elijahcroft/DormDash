#include "pid.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"
#include "motor_control.h"
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(motor_control);

#define THREAD_PRIO 10
#define THREAD_STACK_SIZE 8192
static K_THREAD_STACK_DEFINE(stack_area, THREAD_STACK_SIZE);
static struct k_thread thread_data;

K_MUTEX_DEFINE(motor_velocity_set_mutex);

#define UPDATE_RATE_HZ 100.0f
#define PID_LOOP_DELAY_MS ((1.0f / UPDATE_RATE_HZ) * 1000.0f)

/* Controller parameters */
#define PID_KP  0.005f
#define PID_KI  0.01f
#define PID_KD  0.0f

#define PID_TAU 0.9f

#define PID_LIM_MIN -1.0f
#define PID_LIM_MAX  1.0f

#define PID_LIM_MIN_INT -1.0f
#define PID_LIM_MAX_INT  1.0f

// -----------------------------


#define MOTOR_CONTROLLER_COUNT 2

static PIDController left_motor_pid = {
    .Kp = PID_KP,
    .Ki = PID_KI,
    .Kd = PID_KD,

    .tau = PID_TAU,

    .limMin = PID_LIM_MIN,
    .limMax = PID_LIM_MAX,

    .limMinInt = PID_LIM_MIN_INT,
    .limMaxInt = PID_LIM_MAX_INT
};

static PIDController right_motor_pid = {
    .Kp = PID_KP,
    .Ki = PID_KI,
    .Kd = PID_KD,

    .tau = PID_TAU,

    .limMin = PID_LIM_MIN,
    .limMax = PID_LIM_MAX,

    .limMinInt = PID_LIM_MIN_INT,
    .limMaxInt = PID_LIM_MAX_INT
};

struct motor_controller
{
    enum MotorType motor_type;
    enum QuadEncoderType encoder_type;
	float target_velocity_steps;
	float current_velocity_steps;
};

struct motor_controller controllers[MOTOR_CONTROLLER_COUNT] = {
    {
        .motor_type = MOTOR_TYPE_LEFT,
        .encoder_type = Q_ENCODER_LEFT_MOTOR
    },
    {
        .motor_type = MOTOR_TYPE_RIGHT,
        .encoder_type = Q_ENCODER_RIGHT_MOTOR
    },
};

void init_pid_controllers() {
    PIDController_Init(&left_motor_pid);
    PIDController_Init(&right_motor_pid);
}

void MotorControl_SetVelocity(enum MotorControllerType type, float velocity_step_per_sec)
{
	k_mutex_lock(&motor_velocity_set_mutex, K_FOREVER);
	{
		controllers[type].target_velocity_steps = velocity_step_per_sec;
	}
	k_mutex_unlock(&motor_velocity_set_mutex);
}

static void motor_control_thread_entry(void* a, void* b, void* c)
{
	ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

	float target_left_speed_local = 0;
    float target_right_speed_local = 0;
	float current_left_speed = 0;
	float current_right_speed = 0;

	long last_pos_left = 0;
	long last_pos_right = 0;
	long current_pos_left = 0;
	long current_pos_right = 0;

    uint32_t current_time = 0;
    uint32_t last_time = 0;
    float delta_time = 0;

    last_time = sys_clock_cycle_get_32();

    while (true) {

        current_time = sys_clock_cycle_get_32();
        const uint64_t cycles_diff = current_time - last_time;
        delta_time = (float)k_cyc_to_ns_ceil32(cycles_diff) / NSEC_PER_SEC;
        last_time = current_time;

		k_mutex_lock(&motor_velocity_set_mutex, K_FOREVER);
		{
			target_left_speed_local = controllers[CONTROLLER_LEFT].target_velocity_steps;
			target_right_speed_local = controllers[CONTROLLER_RIGHT].target_velocity_steps;
		}
		k_mutex_unlock(&motor_velocity_set_mutex);

		current_pos_left = QuadratureEncoder_GetPos(controllers[CONTROLLER_LEFT].encoder_type);
		current_pos_right = QuadratureEncoder_GetPos(controllers[CONTROLLER_RIGHT].encoder_type);

		current_left_speed = (current_pos_left - last_pos_left) / delta_time;
		current_right_speed = (current_pos_right - last_pos_right) / delta_time;

		last_pos_left = current_pos_left;
		last_pos_right = current_pos_right;

		//LOG_INF("Left pos %f | Right pos %f", (double)current_pos_left, (double)current_pos_right);
		//LOG_INF("Left speed %f | Right speed %f", (double)current_left_speed, (double)current_right_speed);

        float left_pid_out = PIDController_Update(&left_motor_pid, target_left_speed_local, current_left_speed, delta_time);
        float right_pid_out = PIDController_Update(&right_motor_pid, target_right_speed_local, current_right_speed, delta_time);

		PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, left_pid_out * 100.0f);
		PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, right_pid_out * 100.0f);

 		LOG_INF("target right speed: %f", (double)target_right_speed_local);
		LOG_INF("current right speed: %f", (double)current_right_speed);

        k_msleep(PID_LOOP_DELAY_MS - delta_time);
    }

}

int MotorControl_Start()
{
    init_pid_controllers();

    k_thread_create(&thread_data, stack_area, K_THREAD_STACK_SIZEOF(stack_area), motor_control_thread_entry, NULL,
                    NULL, NULL, THREAD_PRIO, 0, K_NO_WAIT);

    return 0;
}