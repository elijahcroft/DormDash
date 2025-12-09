#include "tests.h"
#include "pwm_motor.h"
#include "quadrature_decoder.h"
#include "distance_sensor.h"
#include "motor_control.h"
#include "zephyr/kernel.h"

void test_motor_pwm()
{
    int ret_1 = 0;
    int ret_2 = 0;

    ret_1 = PWM_Start();
    __ASSERT(ret_1 == 0, "PWM devices not ready, err: %d", ret_1);

    ret_1 = PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, -100.0f);
    ret_2 = PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, -100.0f);
    __ASSERT(ret_1 == 0, "Unable to set left motor duty cycle to -100, err: %d", ret_1);
    __ASSERT(ret_2 == 0, "Unable to set right motor duty cycle to -100, err: %d", ret_2);

    k_sleep(K_MSEC(500));

    ret_1 = PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, 0.0f);
    ret_2 = PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, 0.0f);
    __ASSERT(ret_1 == 0, "Unable to set left motor duty cycle to 0, err: %d", ret_1);
    __ASSERT(ret_2 == 0, "Unable to set right motor duty cycle to 0, err: %d", ret_2);

    k_sleep(K_MSEC(500));

    ret_1 = PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, 100.0f);
    ret_2 = PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, 100.0f);
    __ASSERT(ret_1 == 0, "Unable to set left motor duty cycle to 100, err: %d", ret_1);
    __ASSERT(ret_2 == 0, "Unable to set right motor duty cycle to 100, err: %d", ret_2);

    k_sleep(K_MSEC(500));

    ret_1 = PwmMotor_SetDutyCycle(MOTOR_TYPE_LEFT, 0.0f);
    ret_2 = PwmMotor_SetDutyCycle(MOTOR_TYPE_RIGHT, 0.0f);
    __ASSERT(ret_1 == 0, "Unable to set left motor duty cycle to 0, err: %d", ret_1);
    __ASSERT(ret_2 == 0, "Unable to set right motor duty cycle to 0, err: %d", ret_2);
}

void test_quadrature_decoder()
{
    int ret = QuadratureDecoder_Start();
    __ASSERT(ret == 0, "Unable to start quadrature decoding, err: %d", ret);
}

void test_lidar_sensors()
{
    int ret = LIDAR_Start();
    __ASSERT(ret == 0, "Not all lidar modules are functioning!, err: %d", ret);
}

void test_motor_pid_control()
{
    int ret = 0;

    ret = MotorControl_Start();
    __ASSERT(ret == 0, "Unable to start motor control!, err: %d", ret);

    MotorControl_SetVelocity(CONTROLLER_LEFT, -200);
    MotorControl_SetVelocity(CONTROLLER_RIGHT, -200);
    k_sleep(K_MSEC(4000));
    MotorControl_SetVelocity(CONTROLLER_LEFT, 200);
    MotorControl_SetVelocity(CONTROLLER_RIGHT, 200);
    k_sleep(K_MSEC(4000));
    MotorControl_SetVelocity(CONTROLLER_LEFT, 0);
    MotorControl_SetVelocity(CONTROLLER_RIGHT, 0);
}


void Tests_Start()
{
    test_motor_pwm();
    test_quadrature_decoder();
    test_lidar_sensors();

    // must have quadrature and motor_pwm tested beforehand
    // Must be verified visually
    test_motor_pid_control();
}