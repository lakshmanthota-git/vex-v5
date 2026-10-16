#include "Move.h"


void Pranav3pin(drivetrain_pranav Drivetrain, claw Claw, Arm Arm)
{
    for(int i = 0; i < 2; i ++)
    {
        Drivetrain.LateralPD(2);
        Drivetrain.LateralPD(-2);
    }
    Drivetrain.LateralPD(12);
    Drivetrain.TurnPD(90);
    Drivetrain.LateralPD(12);
    Drivetrain.LateralPD(-12);
    Drivetrain.TurnPD(-45);
    Drivetrain.LateralPD(16.97);
    Drivetrain.TurnPD(135);
    Drivetrain.LateralPD(12);
    Drivetrain.LateralPD(-12);
    Drivetrain.TurnPD(45);
    Drivetrain.LateralPD(16.97);
    Drivetrain.TurnPD(135);
    Drivetrain.LateralPD(12);

}