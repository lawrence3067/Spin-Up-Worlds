//Routes for all autonomous functions

#include "main.h"

using namespace okapi;

void closeFiveDisc()
{
  /***

  Close side autonomous (35 points):

  1. Turn roller
  2. Turn to shoot 2 preloads at high goal
  3. Turn to and intake 3-stack of discs
  4. Turn to high goal and shoot all 3 discs

  ***/
  flywheelSpeed = 70;
  pros::Task startFlywheel(flywheelBangBang);

  translatePID(-5, 500);
  intake.tarePosition();
  while (abs(intake.getPosition()) < 400)
  {
    intake.moveVelocity(-600);
    pros::delay(10);
  }
  intake.moveVelocity(0);
  translatePID(5, 500);
  rotatePID(90);

  //pros::delay(1000);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(750);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  flywheelSpeed = 95;
  rotatePID(-133);
  translatePID(-35, 3000);
  intake.moveVelocity(600);
  driveSpeed = 0.5;
  translatePID(-30, 3000);
  rotatePID(-37);
  translatePID(11, 1500);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

}


void farFiveDisc()
{
  /***

  Far side autonomous (35 points):

  1. Moves towards roller and rolls it to correct color
  2. Turn to shoot 2 preloads at high goal
  3. Turn to and intake line of 3 discs
  4. Turn to high goal and shoot all 3 discs

  ***/

  flywheelSpeed = 115;
  pros::Task startFlywheel(flywheelBangBang);

  intake.moveVelocity(600);
  driveSpeed = 0.8;
  translatePID(-50, 5000);
  rotatePID(-130);
  translatePID(7, 700);
  driveSpeed = 1;
  pros::delay(500);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  translatePID(-7, 700);
  rotatePID(135);
  translatePID(-75, 7000);
  intake.moveVelocity(0);
  rotatePID(180);
  translatePID(-10, 700);
  flywheelSpeed = 70;

  intake.tarePosition();
  while (abs(intake.getPosition()) < 300)
  {
    intake.moveVelocity(-600);
    pros::delay(10);
  }
  intake.moveVelocity(0);
  translatePID(5, 500);
  rotatePID(90);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(500);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
}

void soloAWP()
{
  /***

  Start on Near Side (__ points):

  1. Turn roller
  2. Turn to shoot 2 preloads at high goal
  3. Turn to and intake 3-stack of discs
  4. Turn to high goal and shoot all 3 discs
  5. Turn to and intake line of 3 discs
  7. Translate to roller and swing turn into the roller
  8. Turn roller
  9. Turn to high goal and shoot all 3 discs

  ***/

  flywheelSpeed = 130;
  pros::Task startFlywheel(flywheelBangBang);

  translatePID(-5, 150);
  intake.tarePosition();
  while (abs(intake.getPosition()) < 300)
  {
    intake.moveVelocity(-600);
    pros::delay(10);
  }
  intake.moveVelocity(0);

  translatePID(5, 500);
  rotatePID(-133);
  translatePID(-35, 3000);
  intake.moveVelocity(600);
  driveSpeed = 0.5;
  translatePID(-30, 3000);
  rotatePID(-34);
  translatePID(11, 1500);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  translatePID(-11, 1500);
  rotatePID(-135);
  driveSpeed = 0.8;
  translatePID(-100, 10000);
  rotatePID(-90);
  translatePID(-10, 750);

  intake.tarePosition();
  while (abs(intake.getPosition()) < 300)
  {
    intake.moveVelocity(-600);
    pros::delay(10);
  }
  intake.moveVelocity(0);
}

void test()
{
  translatePID(50, 2000000);
  //swingLeftTranslate(24, -90, 4000);
  //translateSwingLeft(24, -90, 4000);
  //translateSwingRight(24, 90, 4000);
  //rotatePID(90);
}

/**
void skills()
{  //check that you used the correct translate  //change driving over disks to translate4
  flywheelSpeed = 75;
  pros::Task startFlywheel(flywheelBangBang);

  intakeLeft.tarePosition();
  intakeRight.tarePosition();
  inertialSensor.reset();

  pistonPTO.set_value(false);

  translatePID(-5,true,400);

  while(intakeRight.getPosition() <= 320){ //100
    intakeLeft.moveVelocity(600);
    intakeRight.moveVelocity(600);
  }
  translatePID(3,true,500); //pick up disc

  rotatePID(150);
  pistonPTO.set_value(false); //up


  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  translate4(-18,1000);
  pros::delay(250); //was 1000 700

  rotatePID(90);

  // pros::lcd::set_text(6,"hi");

  translatePID(-16.5,true,890); //drive toward rollers

  pistonPTO.set_value(false);

  intakeLeft.tarePosition();
  intakeRight.tarePosition();

  while(intakeRight.getPosition() <= 330){ //100
    intakeLeft.moveVelocity(600);
    intakeRight.moveVelocity(600);
    pros::delay(10);
  }

  translatePID(3,true,1500);

  //flywheel.moveVelocity(370);
  //flywheel2.moveVelocity(370);

  rotatePID(6);


  translatePID(62,true,5000);//51
  rotatePID(20);
  pistonPTO.set_value(false);


  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  //shoots first set
  pistonDisk.set_value(true);
  pros::delay(200); //was 400
  pistonDisk.set_value(false);
  pros::delay(220);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(220);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0);

  // rotatePID(-3);
  //
  // translatePID(-6,true,2000);//300

  rotatePID(-35);

  pistonPTO.set_value(false);
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  translate4(-39,4000); //drive toward 3 line

  rotatePID(-135);

  pistonPTO.set_value(false);
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);
  flywheelSpeed = 80;

  //translateJank(-40,5000);
  translate4(-40, 5000);

  // flywheel.moveVelocity(390); //387
  // flywheel2.moveVelocity(390);
  rotate4(-47);//-45

  translate4(10,800); //drive close to basket
  pros::delay(1000);


  //shoots second set
  pistonDisk.set_value(true);
  pros::delay(200);//changed from 400
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(365);
  // flywheel2.moveVelocity(365);
  pros::delay(250); //500
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(365);
  // flywheel2.moveVelocity(365);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0);
  translate4(-10,1000); //drive back from flywheel

  rotatePID(-135);

  pistonPTO.set_value(false);

  translate4(-33, 4000); //35

  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  translateJank(-28, 3800); //pick up 3 disks //was 28

  rotatePID(-88); //face flywheel
  flywheelSpeed = 75;

  // flywheel.moveVelocity(380);
  // flywheel2.moveVelocity(380);


  translatePID(50,true,3200);
  rotatePID(-97);
  pistonPTO.set_value(false); //up
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  //shoots third set
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0);

  rotatePID(-86);
  pistonPTO.set_value(false); //up
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);
  translate4(-55, 5000);

  rotateJank(-180, 0);
  translatePID(-12, true, 1000);

  // translate4(-60,3600); //drive back
  //
  // rotateJank(-180,0);
  //
  //
  // translatePID(-7,true,800); //drive toward rollers
  //
  pistonPTO.set_value(false);

  intakeLeft.tarePosition();
  intakeRight.tarePosition();

  while(intakeRight.getPosition() <= 300){ //100
    intakeLeft.moveVelocity(600);
    intakeRight.moveVelocity(600);
    pros::delay(10);
  }

  translatePID(3,true,500);
  rotatePID(-40);
  pistonPTO.set_value(false); //up
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);
  translate4(-12, 1000);
  rotatePID(-135);
  pistonPTO.set_value(false); //up
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);
  translate4(-20, 1500);
  translate4(21, 1500);
  rotatePID(-90);
  translate4(-20, 1500);

  // rotatePID(-90);
  //
  // pistonPTO.set_value(false); //up
  // intakeLeft.moveVelocity(-600);
  // intakeRight.moveVelocity(-600);
  //
  // translate4(-13,2000);
  //
  // //rotatePID(-135);
  //
  // // translate4(-15,1200); //get corner disk
  // // pros::delay(200);
  //
  // // translatePID(20,true,2000);
  //
  // rotatePID(0);
  // pistonPTO.set_value(false); //up
  // intakeLeft.moveVelocity(-600);
  // intakeRight.moveVelocity(-600);
  // translate4(-17, 2000);
  // rotatePID(-90);
  //
  // translate4(-12,700); //drive toward second roller

  pistonPTO.set_value(false);

  intakeLeft.tarePosition();
  intakeRight.tarePosition();

  while(intakeRight.getPosition() <= 300){ //100
    intakeLeft.moveVelocity(600);
    intakeRight.moveVelocity(600);
    pros::delay(10);
  }
  intakeLeft.moveVelocity(0);
  intakeRight.moveVelocity(0);





  //MIRROR

  translatePID(3,true,1500);

  // flywheel.moveVelocity(380);
  // flywheel2.moveVelocity(380);

  rotateJank(-173,0);

  translatePID(57,true,4000);
  rotateJank(-165,0);
  pistonPTO.set_value(false);




  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  //shoots fourth set
  pistonDisk.set_value(true);
  pros::delay(200); //was 400
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(380); //387
  // flywheel2.moveVelocity(380);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(380); //395
  // flywheel2.moveVelocity(380);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0);

  // rotateJank(-180, 0);
  //
  // translatePID(-8,true,1000);



  rotateJank(140, 0);

  pistonPTO.set_value(false);
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  translate4(-37, 3000); //drive toward 3 line //31

  rotatePID(45);

  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0); //stop flywheel


  pistonPTO.set_value(false);
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);

  // translateJank(-34,4000);
  translate4(-34,4000);

  rotatePID(0);
  pistonPTO.set_value(false);
  intakeLeft.moveVelocity(-600);
  intakeRight.moveVelocity(-600);
  translate4(-43, 3500);
  // flywheel.moveVelocity(380);
  // flywheel2.moveVelocity(380);
  rotatePID(90);
  translatePID(15, true, 3000);
  rotatePID(80);

  // rotatePID(135);
  //
  // pistonPTO.set_value(false);
  // intakeLeft.moveVelocity(-600);
  // intakeRight.moveVelocity(-600);
  //
  // translate4(7,600); //drive close to basket

  //shoots fifth set
  pros::delay(500);
  pistonDisk.set_value(true);
  pros::delay(200);//changed from 400
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(387);
  // flywheel2.moveVelocity(387);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  // flywheel.moveVelocity(380);
  // flywheel2.moveVelocity(380);
  pros::delay(250);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  // flywheel.moveVelocity(0);
  // flywheel2.moveVelocity(0);

  flywheelSpeed = 0;

  rotatePID(90);
  translatePID(-80, true, 7000);
  rotatePID(-135);
  translatePID(-10, true, 1000);
  pistonExtension.set_value(true);

  // translate4(-7,1000); //drive back from flywheel
  //
  // rotatePID(45);
  //
  // translatePID(-33,true,3200); //35
  //
  // pistonPTO.set_value(false);
  // intakeLeft.moveVelocity(-600);
  // intakeRight.moveVelocity(-600);
  //
  // translateJank(-32,4000); //pick up 3 disks
  //
  // rotatePID(90); //face basket
  //
  // flywheel.moveVelocity(350);
  // flywheel2.moveVelocity(350);
  //
  //
  // translatePID(50,true,4000);
  // rotatePID(83);
  // pistonPTO.set_value(false);
  //
  // intakeLeft.moveVelocity(-600);
  // intakeRight.moveVelocity(-600);
  //
  // //shoots sixth set
  // pistonDisk.set_value(true);
  // pros::delay(200);
  // pistonDisk.set_value(false);
  // flywheel.moveVelocity(350);
  // flywheel2.moveVelocity(350);
  // pros::delay(500);
  // pistonDisk.set_value(true);
  // pros::delay(200);
  // pistonDisk.set_value(false);
  // flywheel.moveVelocity(350);
  // flywheel2.moveVelocity(350);
  // pros::delay(500);
  // pistonDisk.set_value(true);
  // pros::delay(200);
  // pistonDisk.set_value(false);
}
**/

void skills()
{
  flywheelSpeed = 70;
  pros::Task startFlywheel(flywheelBangBang);

  translatePID(-5, 500);
  intake.tarePosition();
  while (abs(intake.getPosition()) < 800)
  {
    intake.moveVelocity(-600);
    pros::delay(10);
  }
  intake.moveVelocity(0);
  translatePID(5, 500);
  rotatePID(90);

  //pros::delay(1000);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(750);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  flywheelSpeed = 95;
  rotatePID(-133);
  translatePID(-35, 3000);
  intake.moveVelocity(600);
  driveSpeed = 0.5;
  translatePID(-30, 3000);
  rotatePID(-39);
  translatePID(11, 1500);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(1000);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  driveSpeed = 1;

  translatePID(10, 1000);
  rotatePID(45);
  translatePID(-40, 4000);

  pistonExtension.set_value(true);
}

void skills2()
{
  flywheelSpeed = 71;
  pros::Task startFlywheel(flywheelBangBang);

  intake.moveVelocity(600);
  translatePID(-25, 2000);
  translatePID(25, 2000);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  flywheelSpeed = 90;

  rotatePID(45);
  driveSpeed = 0.4;
  translatePID(-52, 6500);
  driveSpeed = 1;
  translatePID(13, 1700);
  rotatePID(23);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  rotatePID(135);
  driveSpeed = 0.7;
  translatePID(-75, 8000);
  rotatePID(86);

  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);
  pros::delay(200);
  pistonDisk.set_value(true);
  pros::delay(200);
  pistonDisk.set_value(false);

  driveSpeed = 0.4;
  intake.moveVelocity(500);
  translatePID(-50, 6500);
  intake.moveVelocity(0);
  rotatePID(90);
  translatePID(-15, 1000);

  intake.tarePosition();
  while (abs(intake.getPosition()) < 1000)
  {
    intake.moveVelocity(600);
    pros::delay(10);
  }
  intake.moveVelocity(0);
}
