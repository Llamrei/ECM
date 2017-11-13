#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#define _XTAL_FREQ 8000000

#include <xc.h>

struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyLowByte; //PWM duty low byte address
    unsigned char *dutyHighByte; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int PWMperiod; //base period of PWM cycle
};

//function prototypes
int getTPWM(int T, int OSC, char scaler);
void initPWM(int PWMperiod); // function to setup PWM
void setMotorPWM(struct DC_motor *m);
void setMotorFullSpeed(struct DC_motor *m);
void stopMotor(struct DC_motor *m);
void stop(struct DC_motor *mL, struct DC_motor *mR);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR);
void turnRight(struct DC_motor *mL, struct DC_motor *mR);
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR);

#endif
