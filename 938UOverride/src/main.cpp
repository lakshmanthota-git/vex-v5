/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kasta                                                     */
/*    Created:      10/3/2026, 9:50:14 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  Controller.rumble("..--..");

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  bool flywheeldir = true;
  while (true) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    int forward = Controller.Axis3.position();
    int turn = Controller.Axis1.position();
    bool armup = Controller.ButtonL1.pressing();
    bool armdown = Controller.ButtonL2.pressing();
    bool clawopen = Controller.ButtonR1.pressing();
    bool clawclose = Controller.ButtonR2.pressing();
    bool flywheel = Controller.ButtonA.pressing();


    leftdrive.spin(vex::forward, forward + turn, percent);
    rightdrive.spin(vex::forward, forward - turn, percent);

    if(armup == true && armdown == true)
      armleft.stop();
    else if(armup == true)
      armleft.spin(vex::forward, 100, percent);
    else if(armdown == true)
      armleft.spin(vex::forward, -100, percent);
    else
      armleft.stop();

    if(clawopen == true && clawclose == true)
      clawopener.stop();
    else if(clawopen == true)
      clawopener.spin(vex::forward, 100, percent);
    else if(clawclose == true)
      clawopener.spin(vex::forward, -100, percent);
    else
      clawopener.stop();
    
    if(flywheel == true)
    {
      if(flywheeldir == true){
        clawflywheel.spin(vex::forward, 100, percent);
        flywheeldir = false;
      }
      else {
        clawflywheel.spin(vex::forward, -100, percent);
        flywheeldir = true;
      }
    }
    else
      clawflywheel.stop();
    wait(20, msec); // Sleep the task for a short amount of time to                   // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
