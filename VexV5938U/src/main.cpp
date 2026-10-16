#include "main.h"
#include "lemlib/api.hpp"

using namespace std;
using namespace pros;
using namespace lemlib;

Controller controller(E_CONTROLLER_MASTER);
Motor motor_11w_1 (1, MotorGearset::blue);
Motor motor_11w_2 (2, MotorGearset::blue);
Motor motor_5w_1 (3);
Motor motor_11w_3 (-8, MotorGearset::blue);
Motor motor_11w_4 (-9, MotorGearset::blue);
Motor motor_5w_2 (-10);

Motor arm_11w_1 (11, MotorGearset::green);
Motor arm_11w_2 (20, MotorGearset::green);

Motor claw_5w_1 (6);
Motor claw_5w_2 (7);

MotorGroup claw({6, 7});
MotorGroup arm({11, 20});
MotorGroup motor_group_left({1, 2, 3});
MotorGroup motor_group_right({-8, -9, -10});
MotorGroup DriveTrain({1, 2, 3, -8, -9, -10});

Rotation rotation_parallel(11);
Rotation rotation_perpendicular(12);

Imu imu(13);

Drivetrain drivetrain(&motor_group_left, &motor_group_right, 12, lemlib::Omniwheel::NEW_325, 360, 2);

TrackingWheel horizantalwheel(&rotation_parallel, Omniwheel::NEW_2, 0, 1);
TrackingWheel perpendicularwheel(&rotation_perpendicular, Omniwheel::NEW_2, 0, 1);

OdomSensors sensors(&horizantalwheel, nullptr, &perpendicularwheel, nullptr, &imu);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0// maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(80, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              8, // derivative gain (kD)
                                              0, // anti windup
                                              5, // small error range, in degrees
                                              50, // small error range timeout, in milliseconds
                                              20, // large error range, in degrees
                                              200, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

ExpoDriveCurve expo_steer(3, 8, 1.019);
ExpoDriveCurve expo_drive(3, 8, 1.019);
Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &expo_steer, &expo_drive);

void OpenClaw()
{}

void CloseClaw()
{}

void SpinFlywheel()
{}

void LiftArm()
{}

 
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}


void initialize() {
	chassis.calibrate();
	chassis.setPose(72, 0, 0);
	arm_11w_1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	arm_11w_2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}


void disabled() {}
void competition_initialize() {}


void autonomous() {
	/*Toggle Change*/
	
	for(int i = 0; i < 2; i++) {
		chassis.moveToPoint(72, 12, 2000);
		chassis.waitUntilDone();
		chassis.moveToPoint(72, 0, 0);
	}
	
	
	chassis.moveToPose(72, 12, 90, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(84, 12, 90, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(72, 12, 45, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(84, 24, 45, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(84, 12, 270, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(84, 24, 315, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(96, 12, 315, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(84, 12, 180, 2000);
	chassis.waitUntilDone();
	
    // turn to face heading 90 with a very long timeout
    //chassis.turnToHeading(90, 100000);

	chassis.moveToPoint(72, -12, 2000);
	chassis.waitUntilDone();
	chassis.moveToPoint(72, 0, 2000);
	chassis.waitUntilDone();
}

void opcontrol() {
	bool clawOpen = false;
	while (true) {
		int left_y = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int right_x = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		bool pressedUp = controller.get_digital(E_CONTROLLER_DIGITAL_R1);
		bool pressedDown = controller.get_digital(E_CONTROLLER_DIGITAL_R2);
		bool clawOpen = controller.get_digital(E_CONTROLLER_DIGITAL_L1);
		bool clawClose = controller.get_digital(E_CONTROLLER_DIGITAL_L2);
		bool flywheel = controller.get_digital(E_CONTROLLER_DIGITAL_A);
	
		if(pressedUp == true)
		{
			arm.move(127);
		}
		else if(pressedDown == true)
		{
			arm.move(-127);
		}
		else
		{
			arm.move(0);
		}

		chassis.arcade(left_y, -1 * right_x, 0.5);
		
		if(clawOpen == true)
		{
			claw_5w_1.move(127);
		}
		else if(clawClose == true)
		{
			claw_5w_1.move(-127);
		}
		else
		{
			claw_5w_1.move(0);
		}

		if(flywheel == true)
		{
			if(clawOpen == true)
			{
				claw_5w_2.move(-127);
				clawOpen = false;
			}
			else
			{
				claw_5w_2.move(0);
				clawOpen = true;
			}
		}
	delay(20);	
	}
}