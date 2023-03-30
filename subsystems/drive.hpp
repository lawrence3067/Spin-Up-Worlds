//Defines all drivetrain-related electronics and functions

//Front Side: Flywheel Side
//Back Side: Intake Side

#include "main.h"

using namespace okapi;

//Drive Right Motors
extern Motor rightFront;
extern Motor rightMiddle;
extern Motor rightBack;

//Drive Left Motors
extern Motor leftFront;
extern Motor leftMiddle;
extern Motor leftBack;

//Drive Chassis Configuration
extern std::shared_ptr<ChassisController> drive;

//Sensors
extern IMU inertialSensor;
extern pros::ADIEncoder encoder;

//Op-Control Movements
void updateDrive();

//Autonomous Movements
void translatePID(double targetDistance, int ms);
void rotatePID(double targetAngle);
void rotateSwingRight(double targetAngle);
void rotateSwingLeft(double targetAngle);

void swingLeftTranslate(double targetDistance, double targetInitialAngle, int ms);
void translateSwingLeft(double targetDistance, double targetFinalAngle, int ms);
void translateSwingRight(double targetDistance, double targetFinalAngle, int ms);
