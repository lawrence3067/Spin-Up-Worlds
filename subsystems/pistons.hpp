//Defines all pneumatic-related electronics and functions

#include "main.h"

using namespace okapi;

//Indexer Solenoid
extern pros::ADIDigitalOut pistonDisk;
//Expansion Solenoid
extern pros::ADIDigitalOut pistonExtension;
//Expansion Blocker Solenoid
extern pros::ADIDigitalOut pistonExtensionBlocker;
//Blooper Solenoid
extern pros::ADIDigitalOut pistonBlooper;

extern int blooperState;

//Op-Control Movements
void updatePistons();
