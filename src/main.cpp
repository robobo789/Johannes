#include <Arduino.h>
#include <PID_v1.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#include <Motor.h>
#include <Encoder.h>
#include <MotorController.h>
#include <Sonar.h>
#include <HeadServo.h>
#include <Organ.h>
#include <DriveUnit.h>

Motor motorLeft(6, 30, 28, 30);
Motor motorRight(7, 26, 24, 30);
float WHEEL_SPREAD_CIRC;

Encoder *Encoder::instances[2] = {NULL, NULL};
Encoder encoderLeft(3, 5, PINE, 5, 3, PINE);
Encoder encoderRight(2, 4, PINE, 4, 5, PING);

MotorController leftController(motorLeft, encoderLeft);
MotorController rightController(motorRight, encoderRight);

DriveUnit driver(leftController, rightController);

HeadServo servo(10, 0, 180);

Sonar *Sonar::instance = NULL;
Sonar sonar(18, 34, 200);

Organ head(servo, sonar);

int drive_seqeunce = -1;

void setup()
{
  head.servo.begin(50, 0);
  head.sonar.begin();

  Serial.begin(9600);

  leftController.setup(1);
  rightController.setup(0);

  leftController.rpm_PID_setup(6, 1, 0, 255);
  rightController.rpm_PID_setup(6, 1, 0, 255);

  leftController.steps_PID_setup(4, 2, 0, 70);
  rightController.steps_PID_setup(4, 2, 0, 70);

  head.setScan(10, 0, 180);
  drive_seqeunce = 1;
}

void loop()
{

  driver.update();
  head.update();

  switch (drive_seqeunce)
  {
  case 1:
    if (head.state == -1)
    {
      head.servo.setTarget(90);
      driver.rotateBy(head.max_dist_angle - 90);
      drive_seqeunce = 2;
    }
    break;

  case 2:
    if (driver.rotationDone)
    {
      int dist = min(150, head.max_dist - 30);
      driver.driveCM(dist);
      drive_seqeunce = 3;
    }
    break;

  case 3:
    if (driver.arrived)
    {
      head.setScan(10, 0, 180);
      drive_seqeunce = 1;
    }
    break;

  default:
    break;
  }
}
