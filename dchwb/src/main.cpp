/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mua30                                                     */
/*    Created:      7/11/2025, 11:47:18 AM                                    */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

vex::brain Brain;
vex::controller Controller;
motor leftLift = motor(PORT6, true);
motor rightLift = motor(PORT12, false);
motor leftDrive = motor(PORT11, true);
motor rightDrive = motor(PORT5, true);
pneumatic pnumatic = pneumatic(PORT1);
distance dist1 = distance(PORT2);
distance dist2 = distance(PORT7);
inertial imu = inertial();

void setup(){
    pnumatic.pumpOn();
    pnumatic.retract(cylinder1);
    pnumatic.retract(cylinder2);
    leftLift.spin(vex::forward);
    rightLift.spin(vex::forward);
    leftLift.setVelocity(100, percent);
    rightLift.setVelocity(100, percent);
    leftLift.setMaxTorque(100, percent);
    rightLift.setMaxTorque(100, percent);
    leftLift.setTimeout(2, seconds);
    rightLift.setTimeout(2, seconds);
    wait(0.3,seconds);
    while(rightLift.velocity(percent) > 20)
        wait(20, msec);
    rightLift.spinFor(vex::reverse, 0.3, turns, false);
    leftLift.spinFor(vex::reverse, 0.3, turns);
    leftLift.stop();
    rightLift.stop();
}

int liftControl(){
    bool up = false;
    while(true) {
        if(dist1.objectDistance(mm) < 45 || dist2.objectDistance(mm) < 45){
            if(up){
                leftLift.spin(vex::forward);
                rightLift.spin(vex::forward);
                wait(0.35, seconds);
                if(dist1.objectDistance(mm) < 60)
                    pnumatic.retract(cylinder2);
                if(dist2.objectDistance(mm) < 60)
                    pnumatic.retract(cylinder1);
                while(leftLift.velocity(percent) > 30)
                    wait(20, msec);
            } else{
                leftLift.spin(vex::forward);
                rightLift.spin(vex::forward);
                wait(0.3,seconds);
                while((dist2.objectDistance(mm) > 35 || dist2.objectDistance(mm) > 35) && rightLift.velocity(percent) > 20)
                    wait(20, msec);
                leftLift.stop();
                rightLift.stop();
            }
            if(dist1.objectDistance(mm) < 60)
                pnumatic.extend(cylinder2);
            if(dist2.objectDistance(mm) < 60)
                pnumatic.extend(cylinder1);
            rightLift.spinFor(vex::reverse, 1.7, turns, false);
            leftLift.spinFor(vex::reverse, 1.7, turns);
            up = true;
        }

        if(Controller.ButtonEUp.pressing()){
            leftLift.spin(vex::forward);
            rightLift.spin(vex::forward);
            wait(0.3,seconds);
            while((dist2.objectDistance(mm) > 35 || dist2.objectDistance(mm) > 35) && rightLift.velocity(percent) > 20)
                wait(20, msec);
            rightLift.spinFor(vex::reverse, 0.7, turns, false);
            leftLift.spinFor(vex::reverse, 0.7, turns);
            while(Controller.ButtonEUp.pressing()){
                wait(20, msec);
            }
            pnumatic.retract(cylinder1);
            pnumatic.retract(cylinder2);

            while((dist2.objectDistance(mm) < 45 || dist2.objectDistance(mm) < 45))
                wait(20, msec);
            rightLift.spinFor(vex::forward, 0.4, turns, false);
            leftLift.spinFor(vex::forward, 0.4, turns);
            up = false;
        }
    }
    return 0;
}

void driveControl(float driveVelocity, float turnVelocity){
    Brain.Screen.print("hi");
    while(true){
        leftDrive.setVelocity(Controller.AxisA.position() * (driveVelocity/100) + Controller.AxisC.position() * (turnVelocity/100), percent);
        rightDrive.setVelocity(Controller.AxisA.position() * (driveVelocity/100) - Controller.AxisC.position() * (turnVelocity/100), percent);
    }
}

int main() {
	setup();
    vex::task ws1(liftControl);
    Brain.Screen.print("hi");
    driveControl(100,100);
}