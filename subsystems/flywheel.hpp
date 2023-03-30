//Defines all flywheel-related electronics and functions

#include "main.h"

using namespace okapi;

//Flywheel Motor
extern pros::Motor prosFlywheel;

//Op-Control Movements
void updateFlywheel();

//Autonomous Movements
void flywheelTask();
void flywheelControl();
void flywheelControl2(double wheelSpeed);
void flywheelBangBang();
