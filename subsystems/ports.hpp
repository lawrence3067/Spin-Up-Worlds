//Defines all used ports

/**

-------8 Motors Total-------

Drivetrain:   6
Flywheel:     1
Intake:       1

-------2 Sensors Total-------

Inertial Sensor
Optical Shaft Encoder

-------2 Solenoids Total-------

Indexer:      1
Expansion:    1

**/

#include "main.h"

//Drivetrain Left Motor Ports
const int leftFrontPort = 19;
const int leftMiddlePort = 21;
const int leftBackPort = 9;

//Drivetrain Right Motor Ports
const int rightFrontPort = 5;
const int rightMiddlePort = 17;
const int rightBackPort = 4;

//Drivetrain Sensor Ports
const int inertialPort = 20;
const char encoderTopPort = 'C';
const char encoderBottomPort = 'D';

//Flywheel Motor Port
const int flywheelPort1 = 6;

//Intake Motor Port
const int intakePort1 = 10;

//Solenoid Ports
const char pistonDiskPort = 'A'; //indexer
const char pistonExtensionPort = 'G'; //expansion
const char pistonExtensionBlockerPort = 'F';
const char pistonBlooperPort = 'H';
