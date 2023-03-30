#include "main.h"

using namespace okapi;

Motor intake(intakePort1, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

void updateIntake()
{
  if(controller.getDigital(ControllerDigital::R2) == 1)
  {
    intake.moveVelocity(-600);
  }
  else if(controller.getDigital(ControllerDigital::R1) == 1)
  {
    intake.moveVelocity(600);
  }
  else
  {
    intake.moveVelocity(0);
  }
}
