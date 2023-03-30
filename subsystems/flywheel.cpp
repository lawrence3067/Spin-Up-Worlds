#include "main.h"

pros::Motor prosFlywheel(flywheelPort1, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

using namespace okapi;

int flywheelState;
double flywheelSpeed;
double wheelSpeed;
double kV;
int i = 0;

typedef struct PID pid;
pid flywheel;

void updateFlywheel(){

  if(controller.getDigital(ControllerDigital::L2) == 1)
  {
    if (blooperState = 1)
    {
      flywheelState = 1;
    }
    else if (blooperState = 2)
    {
      flywheelState = 2;
    }
  }
  if(controller.getDigital(ControllerDigital::L1) == 1)
  {
    flywheelState = 3;
  }

  switch (flywheelState)
  {
    case 1:
      //prosFlywheel.move(90);
      flywheelControl2(-100);
      break;
    case 2:
      flywheelControl2(-115);
      break;
    case 3:
      prosFlywheel.move(0);
      break;
  }
}

void flywheelTask()
{
  flywheel.kP = 0;
  flywheel.kI = 0;
  flywheel.kD = 0.25;
  kV = 1.5;

  flywheel.error = flywheelSpeed - prosFlywheel.get_actual_velocity();
  flywheel.integral += flywheel.error;
  flywheel.derivative = flywheel.error - flywheel.prevError;
  flywheel.prevError = flywheel.error;

  flywheel.power = (flywheel.kP * flywheel.error) + (flywheel.kI * flywheel.integral) + (flywheel.derivative * flywheel.kD) + (kV * flywheelSpeed);
  prosFlywheel.move(flywheel.power);
  //pros::lcd::set_text(3, std::to_string(flywheel.power));
  //pros::lcd::set_text(4, std::to_string(prosFlywheel.get_actual_velocity()));
}

void flywheelControl()
{
  if ((prosFlywheel.get_actual_velocity()) * 127 / 600 < flywheelSpeed)
  {
    while ((prosFlywheel.get_actual_velocity()) * 127 / 600 < flywheelSpeed)
    {
      prosFlywheel.move(127);
      pros::delay(50);
    }
  }
  else
  {
    prosFlywheel.move(flywheelSpeed);
  }
  //pros::lcd::set_text(3, std::to_string(prosFlywheel.get_actual_velocity() * 127 / 600));
}
void flywheelControl2(double wheelSpeed)
{
  if ((prosFlywheel.get_actual_velocity()) * 127 / 600 < wheelSpeed)
  {
    prosFlywheel.move(127);
    pros::delay(50);
  }
  else
  {
    prosFlywheel.move(wheelSpeed);
  }
  //pros::lcd::set_text(3, std::to_string(prosFlywheel.get_actual_velocity() * 127 / 600));
}

void flywheelBangBang()
{
  while (true)
  {
    flywheelControl();
    //pros::lcd::set_text(3, std::to_string(prosFlywheel.get_actual_velocity() * 127 / 600));
    pros::delay(100);
  }
}
