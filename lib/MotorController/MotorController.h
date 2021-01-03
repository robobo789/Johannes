#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include <PID_v1.h>
#include <Motor.h>
#include <Encoder.h>

class MotorController
{

private:
    double steps_input, steps_setpoint, steps_PID_output;
    double stepKp, stepKi, stepKd;
    PID steps_PID;

    double rpm_input, rpm_setpoint, rpm_PID_output;
    PID rpm_PID;

    void step_Control(int desired_steps);
    void rpm_Control(int desired_rpm);

    unsigned long cmd_start_log;
    unsigned long cmd_end_log;
public:
    Motor motor;
    int motor_status;
    Encoder encoder;

    MotorController(Motor mtr, Encoder enc);
    void setup(int dir = 1);
    void rpm_PID_setup(double Kp, double Ki, double Kd, int limits = 255);
    void steps_PID_setup(double Kp, double Ki, double Kd, int limits = 255);

    void update();

    void moveRPM(int rpm, int t = 0, int acl = 0, int dcl = 0);


};

#endif