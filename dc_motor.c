#include <xc.h>
#include "dc_motor.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

// Function to get value of PTPER according to formula given in book
// period in microseconds, osc in MHz
int getPT(int period, int osc, char scaler){
    return ((period*osc)/(4*scaler) - 1);
}

void initPWM(int PWMperiod){
    PTCON0 = 0b00000000;    // free running mode
    PTCON0bits.PTCKPS = 00; // 1:1 prescaler
    PTCON1 = 0b10000000;    // enable PWM timer
    PWMCON0 = 0b01101111;   // PWM1 & 3 enabled, all independent mode
    PWMCON1 = 0x00;         // special features, all 0 (default)
    PTPERL = PWMperiod & 0b0000000011111111; // base PWM period low byte
    PTPERH = PWMperiod >> 8; // base PWM period high byte
    
    //For controlling external motor
    TRISBbits.RB2 = 0;
    TRISBbits.RB0 = 0;
}

// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
    int PWMduty; //tmp variable to store PWM duty cycle

    PWMduty = (m->power*m->PWMperiod)/100;  //calculate duty cycle (value between 0 and PWMperiod)
    
    if (m->direction) //if forward direction
    {
        LATB=LATB|(1<<(m->dir_pin)); //set dir_pin high in LATB
		PWMduty=m->PWMperiod-PWMduty; //need to invert duty cycle as direction is high (100% power is a duty cycle of 0)
    }
    else //if reverse direction
    {
        LATB=LATB&(~(1<<(m->dir_pin))); //set dir_pin low in LATB
    }

	//write duty cycle value to appropriate registers
    PWMduty = getPT(PWMduty, 8, 1);
    *(m->dutyLowByte)=PWMduty<<2;     
    *(m->dutyHighByte)=PWMduty>>6;
}

//increases a motor to full power over a period of time
void setMotorFullSpeed(struct DC_motor *m)
{
	for (m->power; (m->power)< 99; m->power++){ //increase motor power until 100
		setMotorPWM(m);	//pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(5);	//delay of 10 ms (1000 ms from 0 to 100 full power)
	}
}

//function to stop a motor gradually 
void stopMotor(struct DC_motor *m)
{
	for (m->power; (m->power) > 0; m->power--){ //decrease motor power until 100
		setMotorPWM(m);	//pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(5);	//delay of 10 ms (1000 ms from 0 to 100 full power)
	}
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    // Not sensitive enough sensitive for 0
    char minValue = 1;
    while(mL->power > minValue || mR->power > minValue) {
        if(mL->power > minValue){
            mL->power--;
            setMotorPWM(mL);
        }
        if(mR->power > minValue){
            mR->power--;
            setMotorPWM(mR);
        }
        __delay_ms(5);
    }
}

//function to make the robot turn left for time in seconds
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR);
    mL->direction = 0;
    mR->direction = 1;
    fullSpeedAhead(mL, mR);
}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR);
    mL->direction = 1;
    mR->direction = 0;
    fullSpeedAhead(mL, mR);
}

void forward(struct DC_motor *mL, struct DC_motor *mR) {
    mL->direction = 1;
    mR->direction = 1;
    
    fullSpeedAhead(mL, mR);
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
    //Not accurate enough for 100
    char maxValue = 99;
    while(mL->power < maxValue || mR->power < maxValue) {
        if(mL->power < maxValue){
            mL->power++;
            setMotorPWM(mL);
        }
        if(mR->power < maxValue){
            mR->power++;
            setMotorPWM(mR);
        }
        __delay_ms(5);
	}
}