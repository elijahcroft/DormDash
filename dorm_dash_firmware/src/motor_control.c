#include "pid.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"

#define UPDATE_RATE_HZ 100.0f
#define PID_LOOP_DELAY_MS ((1.0f / UPDATE_RATE_HZ) * 1000.0f)

/* Controller parameters */
#define PID_KP  0.01f
#define PID_KI  0.0f
#define PID_KD  0.0f

#define PID_TAU 0.02f

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

int MotorControl_Start()
{
    init_pid_controllers();

    // TODO
    // Create thread for pid stuff and motor control.
    // Create functions to set motor velocities.
    // TODO - add current speed measurement from encoders.

    float target_left_speed = 5;
    float target_right_speed = 0;
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

        left_motor_pid.T = delta_time;
        right_motor_pid.T = delta_time;

		current_pos_left = QuadratureEncoder_GetPos(Q_ENCODER_LEFT_MOTOR);
		current_pos_right = QuadratureEncoder_GetPos(Q_ENCODER_RIGHT_MOTOR);

		current_left_speed = (current_pos_left - last_pos_left) / delta_time;
		current_right_speed = (current_pos_right - last_pos_right) / delta_time;

		last_pos_left = current_pos_left;
		last_pos_right = current_pos_right;

        float left_pid_out = PIDController_Update(&left_motor_pid, target_left_speed, current_left_speed);
        float right_pid_out = PIDController_Update(&right_motor_pid, target_right_speed, current_right_speed);

		PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, left_pid_out * 100.0f);
		PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, right_pid_out * 100.0f);

        k_msleep(PID_LOOP_DELAY_MS - delta_time);
    }

    return 0;
}