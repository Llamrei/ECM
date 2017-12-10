//Alex Larionov and Lewis Tran
#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

//Functions to handle motor control

struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100 - affects speed
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyLowByte; //PWM duty low byte address
    unsigned char *dutyHighByte; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB - controls motor - 2 for right, 0 for left
    int PWMperiod; //base period of PWM cycle
};

//function prototypes
int getPT(int period, int osc, char scaler);            //Determines PTPER required for osc in MHz and period in us
void forward(struct DC_motor *mL, struct DC_motor *mR, char power); //Moves both motors at 100 power forward
void initPWM(int PWMperiod);                            //Configure PWM 1 and 3 to output PWM with no special features, motors to go on RB2 and RB0
void setMotorPWM(struct DC_motor *m);                   //Send out PWM relating to given motor config - takes direction into account
void setMotorFullSpeed(struct DC_motor *m);             //Sets a single motor to full power incrementally
void stopMotor(struct DC_motor *m);                     //Stops a single motor incrementally
void stop(struct DC_motor *mL, struct DC_motor *mR);    //Decrementally stop both motors in parallel
void turnLeft(struct DC_motor *mL, struct DC_motor *mR, char power);//Turns left by stopping, then sending turn signal + fullspeed ahead
void turnRight(struct DC_motor *mL, struct DC_motor *mR, char power);//Turns right by stopping, then sending turn signal + fullspeed ahead
void setSpeedAhead(struct DC_motor *mL, struct DC_motor *mR, char power); //Sets both motor to given power whichever direction the face in parallel
#endif
