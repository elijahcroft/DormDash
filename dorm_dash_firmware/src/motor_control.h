#pragma once

enum MotorControllerType
{
    CONTROLLER_LEFT = 0,
    CONTROLLER_RIGHT = 1
};


int MotorControl_Start();
void MotorControl_SetVelocity(enum MotorControllerType type, float velocity_step_per_sec);