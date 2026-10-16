#include "v5.h"
#include "v5_vcs.h"
using namespace vex;
//Claw side is considered front

motor frontleft = motor(1, ratio6_1, true);
motor middleleft = motor(2, ratio6_1, true);
motor backleft = motor(3, ratio6_1, true);

motor frontright = motor(8, ratio6_1, false);
motor middleright = motor(9, ratio6_1, false);
motor backright = motor(10, ratio6_1, false);

motor armleft = motor(11, ratio18_1, false);
//motor armright = motor(20, ratio18_1, true);

motor clawopener = motor(5, false);
motor clawflywheel = motor(6, true);

motor_group leftdrive = motor_group(frontleft, middleleft, backleft);
motor_group rightdrive = motor_group(frontright, middleright, backright);

motor_group drivetrain = motor_group(frontleft, middleleft, backleft, frontright, middleright, backright);

//motor_group arms = motor_group(armleft, armright);

controller Controller = controller();

inertial InertialSensor = inertial(12); 

