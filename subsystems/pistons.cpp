#include "main.h"

using namespace okapi;

// A piston pushes disk into flywheel  (piston) macro retracts waits a second and goes back out
// up another piston expansion at the end

pros::ADIDigitalOut pistonDisk(pistonDiskPort); //indexer
pros::ADIDigitalOut pistonExtension(pistonExtensionPort);
pros::ADIDigitalOut pistonExtensionBlocker(pistonExtensionBlockerPort);
pros::ADIDigitalOut pistonBlooper(pistonBlooperPort);

int blooperState;


void updatePistons(){
  if (controller.getDigital(ControllerDigital::A) == 1)
   {
     pistonDisk.set_value(true);
     pros::delay(200);
     pistonDisk.set_value(false);
   }

   if(controller.getDigital(ControllerDigital::up) == 1)
   {
     pistonExtension.set_value(false);
   }

   if(controller.getDigital(ControllerDigital::down) == 1)
   {
     pistonExtension.set_value(true);
   }

   if (controller.getDigital(ControllerDigital::left) == 1)
   {
     pistonExtensionBlocker.set_value(true);
   }

   if (controller.getDigital(ControllerDigital::right) == 1)
   {
     pistonExtensionBlocker.set_value(false);
   }

   if (controller.getDigital(ControllerDigital::X) == 1)
   {
     blooperState = 2;
     pistonBlooper.set_value(false);
   }

   if (controller.getDigital(ControllerDigital::B) == 1)
   {
     blooperState = 1;
     pistonBlooper.set_value(true);
   }
}
