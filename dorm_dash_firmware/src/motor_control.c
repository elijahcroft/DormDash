#include "pid.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"

#define MOTOR_CONTROLLER_COUNT 2

static PIDController left_motor = {
    // TODO - initialize
};

static PIDController right_motor = {
    // TODO - initialize
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



int MotorControl_Start()
{
    // TODO
    // Create thread for pid stuff and motor control.
    // Create functions to set motor velocities.


    return 0;
}