#pragma once

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

enum MotorType
{
    MOTOR_TYPE_LEFT = 0,
    MOTOR_TYPE_RIGHT = 1
};

int PWM_Start(void);
int PwmMotor_SetDutyCycle(enum MotorType type, int dutyCycle);