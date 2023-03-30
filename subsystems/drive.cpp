#include "main.h"

using namespace okapi;

//Right Side Motor Configurations
Motor rightFront(rightFrontPort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor rightMiddle(rightMiddlePort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor rightBack(rightBackPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//Left Side Motor Configurations
Motor leftFront(leftFrontPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor leftMiddle(leftMiddlePort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor leftBack(leftBackPort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//Inertial Sensor Configurations
IMU inertialSensor(inertialPort, IMUAxes::z);

//Encoder Configurations
pros::ADIEncoder encoder(encoderTopPort, encoderBottomPort, false); // has to be ab or cd

//Chassis Configurations (combines all 6 motors into a singular controller)
std::shared_ptr<ChassisController> drive =
  ChassisControllerBuilder()
  .withMotors({leftFront,leftMiddle, leftBack}, {rightFront, rightMiddle, rightBack}) //uses motor groups {} to group motors on one side
  .withDimensions(AbstractMotor::gearset::blue, {{3.25_in, 13.7_in}, imev5BlueTPR}) //defines cartridge used, {wheel diameter, chassis width}, internal motor encoder cartridge
  .build(); //builds entire controller

typedef struct PID pid;
pid rotate; //PID used for autonomous rotations
pid translate; //PID used for autonomous translations (forward/backward)

int driveStateReverse; //state controller to decide which side of the robot is the 'front' during op-control
int timer;
double driveSpeed = 1; //value to put a cap speed on the drivetrain (change when we want to go slower)

void updateDrive()
{
  if(controller.getDigital(ControllerDigital::R1) == 1)
  {
    driveStateReverse = 1; //changes state controller to first state
  }
  else if(controller.getDigital(ControllerDigital::L2) == 1)
  {
    driveStateReverse = 2; //changes state controller to second state
  }

  switch(driveStateReverse) //switch between different states
  {
    case 1:
      //changes 'front' side of the robot to intake side
      drive -> getModel() -> tank(-controller.getAnalog(ControllerAnalog::rightY), -controller.getAnalog(ControllerAnalog::leftY));
      break;

    case 2:
      //changes 'front' side of the robot to flywheel side
      drive -> getModel() -> tank(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY));
      break;
  }
}

void translatePID(double targetDistance, int ms)
{
  timer = 0;
  encoder.reset();

  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  translate.target = targetDistance * (360 / (3.25 * M_PI)); //converts the targetDistance units from inches to wheel rotation degrees

  //PID constants for translating (kP, kI, kD)
  translate.kP = 0.003; //0.003
  translate.kI = 0.0002;
  translate.kD = 0.00017; //0.000047

  auto translateController = IterativeControllerFactory::posPID(translate.kP, translate.kI, translate.kD); //creates PID controller to process repeating updated error values
  translateController.setOutputLimits(-driveSpeed, driveSpeed); //sets cap speed for drivetrain

  while(timer < ms)
  {
    translate.error = translate.target - (encoder.get_value() * -1); //calculates distance left to travel (using external encoder readings for better accuracy)
    translate.power = translateController.step(translate.error); //pass calculated error value to PID controller -> outputs calculated power to be sent to motors
    pros::lcd::set_text(3, std::to_string(translate.error));

    drive -> getModel() -> tank(-translate.power, -translate.power); //moves drivetrain according to power value from PID loop

    if ((abs(translate.error) < 10) && (rightFront.getActualVelocity() < 0.1) && (rightBack.getActualVelocity() < 0.1) //ensure motors are not moving (drive is at complete stop) before completing the movement
        && (leftFront.getActualVelocity() < 0.1) && (leftBack.getActualVelocity() < 0.1))
    {
      drive -> getModel() -> tank(0, 0); //stops drivetrain immediately to prevent additional movements
      break;
    }

    timer +=10;
    pros::delay(10);
  }
  drive -> getModel() -> tank(0, 0); //stops drivetrain again to ensure full stop
}




void rotatePID(double targetDegrees)
{
  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  //PID constants for rotating (kP, kI, kD)
  rotate.kP = 0.05;
  rotate.kI = 0.006;
  rotate.kD = 0.001; //0.0045

  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD); //creates PID controller to process repeating updated error values

  while(true)
  {
    pros::lcd::set_text(2, "inertial val"+std::to_string(inertialSensor.get()));
    pros::lcd::set_text(3, "rotate error"+std::to_string(rotate.error));

    //IMU values range from (-180, 180) so it would jump from -180 to 180 instantly if it crossed the threshold which makes drastic error values
    //the following conditional statements ensure that the robot is taking the smallest turn possible, never turning the long way

    if (abs(targetDegrees - inertialSensor.get()) < 180) //tests for normal turning angles values
    {
      rotate.error = targetDegrees - inertialSensor.get();
    }
    else if (targetDegrees < 0 && inertialSensor.get() > 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() - 360); //takes away 360 degrees from inertial sensor readings to find appropriate coterminal angle
    }
    else if (targetDegrees > 0 && inertialSensor.get() < 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() + 360); //adds 360 degrees to inertial sensor readings to find appropriate coterminal angle
    }

    rotate.power = rotateController.step(rotate.error); //calculates angle left to turn
    drive -> getModel() -> tank(-rotate.power, rotate.power); //moves drivetrain according to power value from PID loop (one side is negated to perform a point turn instead of swing turn)

    if (abs(rotate.error) < 3 //error threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0);
      break;
    }
    pros::delay(10);
  }

  drive -> getModel() -> tank(0, 0); //stops drivetrain again to ensure full stop
  pros::lcd::set_text(6, "rotate complete");
}


void rotateSwingLeft(double targetDegrees)
{
  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  //PID constants for rotating (kP, kI, kD)
  rotate.kP = 0.04;
  rotate.kI = 0.006;
  rotate.kD = 0.001;

  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD); //creates PID controller to process repeating updated error values

  while(true)
  {
    pros::lcd::set_text(2, "inertial val"+std::to_string(inertialSensor.get()));
    pros::lcd::set_text(3, "rotate error"+std::to_string(rotate.error));

    //IMU values range from (-180, 180) so it would jump from -180 to 180 instantly if it crossed the threshold which makes drastic error values
    //the following conditional statements ensure that the robot is taking the smallest turn possible, never turning the long way

    if (abs(targetDegrees - inertialSensor.get()) < 180) //tests for normal turning angles values
    {
      rotate.error = targetDegrees - inertialSensor.get();
    }
    else if (targetDegrees < 0 && inertialSensor.get() > 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() - 360); //takes away 360 degrees from inertial sensor readings to find appropriate coterminal angle
    }
    else if (targetDegrees > 0 && inertialSensor.get() < 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() + 360); //adds 360 degrees to inertial sensor readings to find appropriate coterminal angle
    }

    rotate.power = rotateController.step(rotate.error); //calculates angle left to turn
    drive -> getModel() -> tank(0, rotate.power); //moves drivetrain according to power value from PID loop (right side is 0 to perform a swing turn to the right)

    if (abs(rotate.error) < 3 //error threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0);
      break;
    }
    pros::delay(10);
  }

  drive -> getModel() -> tank(0, 0); //stops drivetrain again to ensure full stop
  pros::lcd::set_text(6, "rotate complete");
}


void rotateSwingRight(double targetDegrees)
{
  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  //PID constants for rotating (kP, kI, kD)
  rotate.kP = 0.04;
  rotate.kI = 0.006;
  rotate.kD = 0.001;

  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD); //creates PID controller to process repeating updated error values

  while(true)
  {
    pros::lcd::set_text(2, "inertial val"+std::to_string(inertialSensor.get()));
    pros::lcd::set_text(3, "rotate error"+std::to_string(rotate.error));

    //IMU values range from (-180, 180) so it would jump from -180 to 180 instantly if it crossed the threshold which makes drastic error values
    //the following conditional statements ensure that the robot is taking the smallest turn possible, never turning the long way

    if (abs(targetDegrees - inertialSensor.get()) < 180) //tests for normal turning angles values
    {
      rotate.error = targetDegrees - inertialSensor.get();
    }
    else if (targetDegrees < 0 && inertialSensor.get() > 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() - 360); //takes away 360 degrees from inertial sensor readings to find appropriate coterminal angle
    }
    else if (targetDegrees > 0 && inertialSensor.get() < 0) //tests for if the turn must cross the -180/180 angle
    {
      rotate.error = targetDegrees - (inertialSensor.get() + 360); //adds 360 degrees to inertial sensor readings to find appropriate coterminal angle
    }

    rotate.power = rotateController.step(rotate.error); //calculates angle left to turn
    drive -> getModel() -> tank(-rotate.power, 0); //moves drivetrain according to power value from PID loop (left side is 0 to perform a swing turn to the left)

    if (abs(rotate.error) < 3 //error threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0);
      break;
    }
    pros::delay(10);
  }

  drive -> getModel() -> tank(0, 0); //stops drivetrain again to ensure full stop
  pros::lcd::set_text(6, "rotate complete");
}

void swingLeftTranslate(double targetDistance, double targetInitialAngle, int ms)
{
  timer = 0;

  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  translate.target = targetDistance * (360 / (3.25 * M_PI)); //converts the targetDistance units from inches to wheel rotation degrees

  //PID constants for translating (kP, kI, kD)
  translate.kP = 0.003;
  translate.kI = 0;
  translate.kD = 0.000047;

  auto translateController = IterativeControllerFactory::posPID(translate.kP, translate.kI, translate.kD); //creates PID controller to process repeating updated error values
  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD);
  translateController.setOutputLimits(-driveSpeed, driveSpeed); //sets cap speed for drivetrain

  while (abs(targetInitialAngle - inertialSensor.get()) > 3)
  {
    drive -> getModel() -> tank(0, 0.5);
    pros::delay(10);
  }

  encoder.reset();

  while(timer < ms)
  {
    translate.error = translate.target - (encoder.get_value() * -1); //calculates distance left to travel (using external encoder readings for better accuracy)
    translate.power = translateController.step(translate.error); //pass calculated error value to PID controller -> outputs calculated power to be sent to motors

    drive -> getModel() -> tank(-translate.power,-translate.power); //moves drivetrain according to power value from PID loop

    if (abs(translate.error) < 5 //degrees threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0); //stops drivetrain immediately to prevent additional movements
      break;
    }

    timer +=10;
    pros::delay(10);
  }
  drive -> getModel() -> tank(0, 0); //stops drivetrain again to ensure full stop
}

void translateSwingLeft(double targetDistance, double targetFinalAngle, int ms)
{
  timer = 0;

  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  translate.target = targetDistance * (360 / (3.25 * M_PI)); //converts the targetDistance units from inches to wheel rotation degrees

  //PID constants for translating (kP, kI, kD)
  translate.kP = 0.003;
  translate.kI = 0;
  translate.kD = 0.000047;

  rotate.kP = 0.05;
  rotate.kI = 0.006;
  rotate.kD = 0.001;

  auto translateController = IterativeControllerFactory::posPID(translate.kP, translate.kI, translate.kD); //creates PID controller to process repeating updated error values
  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD); //creates PID controller to process repeating updated error values

  translateController.setOutputLimits(-driveSpeed, driveSpeed); //sets cap speed for drivetrain

  while(timer < ms)
  {
    translate.error = translate.target - (encoder.get_value() * -1); //calculates distance left to travel (using external encoder readings for better accuracy)
    translate.power = translateController.step(translate.error); //pass calculated error value to PID controller -> outputs calculated power to be sent to motors

    drive -> getModel() -> tank(-translate.power,-translate.power * 0.85); //moves drivetrain according to power value from PID loop

    if (abs(translate.error) < 176.295) //degrees threshold to break out of PID loop
    {
      break;
    }

    timer +=10;
    pros::delay(10);
  }

  while(true)
  {
    pros::lcd::set_text(2, "inertial val"+std::to_string(inertialSensor.get()));
    pros::lcd::set_text(3, "rotate error"+std::to_string(rotate.error));

    rotate.error = targetFinalAngle - inertialSensor.get();
    rotate.power = rotateController.step(rotate.error); //calculates angle left to turn
    drive -> getModel() -> tank(0, rotate.power); //moves drivetrain according to power value from PID loop (left side is 0 to perform a swing turn to the left)

    if (abs(rotate.error) < 3 //error threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0);
      break;
    }
    pros::delay(10);
  }

  drive -> getModel() -> tank(0, 0);
}

void translateSwingRight(double targetDistance, double targetFinalAngle, int ms)
{
  timer = 0;

  //sets all the drive motors to 'hold' mode -> prevents wheels from drifting by momentum
  rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightBack.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftMiddle.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftBack.setBrakeMode(AbstractMotor::brakeMode::hold);

  translate.target = targetDistance * (360 / (3.25 * M_PI)); //converts the targetDistance units from inches to wheel rotation degrees

  //PID constants for translating (kP, kI, kD)
  translate.kP = 0.003;
  translate.kI = 0;
  translate.kD = 0.000047;

  rotate.kP = 0.05;
  rotate.kI = 0.006;
  rotate.kD = 0.001;

  auto translateController = IterativeControllerFactory::posPID(translate.kP, translate.kI, translate.kD); //creates PID controller to process repeating updated error values
  auto rotateController = IterativeControllerFactory::posPID(rotate.kP, rotate.kI, rotate.kD); //creates PID controller to process repeating updated error values

  translateController.setOutputLimits(-driveSpeed, driveSpeed); //sets cap speed for drivetrain

  while(timer < ms)
  {
    translate.error = translate.target - (encoder.get_value() * -1); //calculates distance left to travel (using external encoder readings for better accuracy)
    translate.power = translateController.step(translate.error); //pass calculated error value to PID controller -> outputs calculated power to be sent to motors

    drive -> getModel() -> tank(-translate.power, -translate.power * 0.85); //moves drivetrain according to power value from PID loop

    if (abs(translate.error) < 176.295) //degrees threshold to break out of PID loop
    {
      break;
    }

    timer +=10;
    pros::delay(10);
  }


  while(true)
  {
    pros::lcd::set_text(2, "inertial val"+std::to_string(inertialSensor.get()));
    pros::lcd::set_text(3, "rotate error"+std::to_string(rotate.error));

    rotate.error = targetFinalAngle - inertialSensor.get();
    rotate.power = rotateController.step(rotate.error); //calculates angle left to turn
    drive -> getModel() -> tank(-rotate.power, 0); //moves drivetrain according to power value from PID loop (left side is 0 to perform a swing turn to the left)

    if (abs(rotate.error) < 3 //error threshold to break out of PID loop
        && rightFront.getActualVelocity() < 1 && rightBack.getActualVelocity() < 1 //ensure motors are not moving (drive is at complete stop) before completing the movement
        && leftFront.getActualVelocity() < 1 && leftBack.getActualVelocity() < 1)
    {
      drive -> getModel() -> tank(0, 0);
      break;
    }
    pros::delay(10);
  }

  drive -> getModel() -> tank(0, 0);
}
