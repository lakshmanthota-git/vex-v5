#include "v5.h"
#include "v5_vcs.h"
//#include "robotconfig.h"
#include <string>
#include <future>
#include <thread>
#include <chrono>
#pragma once
class drivetrain_pranav
{
    private:
        motor_group leftdrive;
        motor_group rightdrive;
        // inertial InertialSensor;
    public:
        drivetrain_pranav(motor_group leftside, motor_group rightside)
        {
                    /// leftdrive(leftside), rightdrive(rightside), InertialSensor(Inertial) 
                    leftdrive = leftside;
                    rightdrive = rightside;
                    //  InertialSensor =Inertial;
        };

    public:
        void LateralPD(double distance)
        {
            double kP = 0.1;
            double kD = 0;
            
            double motorposition = (leftdrive.position(degrees) + rightdrive.position(degrees)) / (double)2;
            double prevmotorposition = motorposition;
            double startposition = (leftdrive.position(degrees) + rightdrive.position(degrees)) / (double)2;

            double distancetraveled = 0;

            double error = distance;
            double preverror = distance; 

            double derivativeerror = 0;

            double margin = 0.3;
            double output = 0;
            while(true)
            {
                prevmotorposition = motorposition;
                motorposition = (leftdrive.position(degrees) + rightdrive.position(degrees)) / (double)2;

                distancetraveled += ((motorposition - prevmotorposition) / (double)360) * (double)10.21;

                preverror = error;
                error = distance - distancetraveled;

                derivativeerror = preverror - error;
                output = kP * error + kD * derivativeerror;
                leftdrive.spin(forward, output, percent);
                rightdrive.spin(forward, output, percent);

                if(error < margin && error > -margin)
                {
                    leftdrive.stop();
                    rightdrive.stop();
                    break;
                }
                wait(20, msec);
            }

        }
        void TurnPD(double degrees)
        {
            InertialSensor.resetHeading();
            double kP = 0.1;
            double kD = 0;

            double currheading = InertialSensor.heading();
            double prevheading = currheading;

            double error = degrees - currheading;
            double preverror = error;

            double derivativeerror = 0;
            double margin = 0.3;
            double output = 0;
            while(true)
            {
                prevheading = currheading;
                currheading = InertialSensor.heading();
                
                preverror = error;
                error = degrees - currheading;

                derivativeerror = preverror - error;
                output = kP * error + kD * derivativeerror;
                leftdrive.spin(forward, output, percent);
                rightdrive.spin(forward, output, percent);
                if(error < margin && error > -margin)
                {
                    leftdrive.stop();
                    rightdrive.stop();
                    break;
                }
                wait(20, msec);
            }

        }
        void SwingTurnPD(double degrees, std::string direction)
        {
            InertialSensor.resetHeading();
            double kP = 0.1;
            double kD = 0;

            double currheading = InertialSensor.heading();
            double prevheading = currheading;

            double error = degrees - currheading;
            double preverror = error;

            double derivativeerror = 0;
            double margin = 0.3;
            double output = 0;
            while(true)
            {
                prevheading = currheading;
                currheading = InertialSensor.heading();
                
                preverror = error;
                error = degrees - currheading;

                derivativeerror = preverror - error;
                output = kP * error + kD * derivativeerror;
                if(direction == "right")
                {
                    leftdrive.spin(forward, output, percent);
                }
                else if(direction == "left")
                {
                    rightdrive.spin(forward, output, percent);
                }
                if(error < margin && error > -margin)
                {
                    leftdrive.stop();
                    rightdrive.stop();
                    break;
                }
                wait(20, msec);
            }
        }
};


class Arm
{
    private:
        motor arm;
    Arm(motor armMotor) : arm(armMotor) {};
    public:
        void LiftArm(double height)
        {
            double DegtoHeightRat = 5;
            double targetDegrees = height * DegtoHeightRat;
            arm.resetPosition();
            arm.spinToPosition(targetDegrees, degrees);

        }
};

class claw
{
    private:
        motor clawopener;
        motor clawflywheel;
    public:
        claw(motor clawOpener, motor clawFlyWheel) : clawopener(clawOpener), clawflywheel(clawFlyWheel) {};
    public:
        void SpinFlyWheel()
        {
            // TODO: Do we need while here,
            clawflywheel.spin(forward, 100, percent);
            wait(1500, msec);
            clawflywheel.stop();
        }
   
        static int32_t SpinFlyWheelTask(void* arg)
        {
            static_cast<claw*>(arg)->SpinFlyWheel();
            return 0;
        }
       
        void OpenClaw()
        {
            double position = clawopener.position(degrees);
            clawopener.spinToPosition(position + 75, degrees);
        }

        void CloseClaw(void* arg)
        {
            //  void (* callback)(void)
            double position = clawopener.position(degrees);
            //std::future<void> &result = std::async(std::launch::async, SpinFlyWheel);
            vex::thread flywheelthread(SpinFlyWheel);
            clawopener.spinToPosition(position - 75, degrees);
            //result.get();
            flywheelthread.join();
            
        }   

};