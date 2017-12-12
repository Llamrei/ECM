//Alex Larionov and Lewis Tran
#include <xc.h>
#include "dc_motor.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

// Function to get value of PTPER according to formula given in book
// Period in microseconds, osc in MHz
int getPT(int period, int osc, char scaler){
    return ((period*osc)/(4*scaler) - 1);
}

void initPWM(int PWMperiod){
    PTCON0 = 0b00000000;        // Free running mode
    PTCON0bits.PTCKPS = 00;     // 1:1 prescaler
    PTCON1 = 0b10000000;        // Enable PWM timer
    PWMCON0 = 0b01101111;       // PWM1 & 3 enabled, all independent mode
    PWMCON1 = 0x00;             // Special features, all 0 (default)
    PTPERL = PWMperiod & 0b0000000011111111; // Base PWM period low byte
    PTPERH = PWMperiod >> 8;    // Base PWM period high byte
    
    // For controlling external motor
    TRISBbits.RB2 = 0;
    TRISBbits.RB0 = 0;
}

// Function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
    int PWMduty; // Temporary variable to store PWM duty cycle

    PWMduty = (m->power*m->PWMperiod)/100;  // Calculate duty cycle (value between 0 and PWMperiod)
    
    if (m->direction) // If in the forward direction
    {
        LATB=LATB|(1<<(m->dir_pin));    // Set dir_pin high in LATB
		PWMduty=m->PWMperiod-PWMduty;   // Invert duty cycle as direction is high (100% power is a duty cycle of 0)
    }
    else // If in the reverse direction
    {
        LATB=LATB&(~(1<<(m->dir_pin))); // Set dir_pin low in LATB
    }

	// Write duty cycle value to appropriate registers
    PWMduty = getPT(PWMduty, 8, 1);
    *(m->dutyLowByte)=PWMduty<<2;     
    *(m->dutyHighByte)=PWMduty>>6;
}

// Increases a motor to full power over a period of time
void setMotorFullSpeed(struct DC_motor *m)
{
	for (m->power; (m->power)< 99; m->power++){ // Increase motor power until max
		setMotorPWM(m);	// Pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(5);	// Delay of 10 ms (1000 ms from 0 to 100 full power)
	}
}

// Function to stop a motor gradually 
void stopMotor(struct DC_motor *m)
{
	for (m->power; (m->power) > 0; m->power--){ // Decrease motor power until min
		setMotorPWM(m);	// Pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(5);	// Delay of 10 ms (1000 ms from 0 to 100 full power)
	}
}

// Function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    char minValue = 1;  // Set min value to 1 (not sensitive enough for 0)
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

// Function to make the robot turn left for time in seconds
void turnLeft(struct DC_motor *mL, struct DC_motor *mR, char power)
{
    if(!(mL->direction == 0 && mR->direction == 1)){
        stop(mL,mR);
        mL->direction = 0;
        mR->direction = 1;  
    }
    setSpeedAhead(mL, mR, power);
}

// Function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR, char power)
{
    if(!(mL->direction == 1 && mR->direction == 0)){
        //If not already turning right - set to turn right
        stop(mL,mR);
        mL->direction = 1;
        mR->direction = 0;   
    }    
    setSpeedAhead(mL, mR, power);
}

void forward(struct DC_motor *mL, struct DC_motor *mR, char power) {
    mL->direction = 1;
    mR->direction = 1;
    
    setSpeedAhead(mL, mR, power);
}

void setSpeedAhead(struct DC_motor *mL, struct DC_motor *mR, char power) {
    //Not accurate enough for 100
    if(power > 98){
        power = 98;
    }
    while(mL->power != power || mR->power != power) {
        if(mL->power < power){
            mL->power++;
        } else if(mL->power > power) {
            mL->power--;
        }
        if(mR->power < power){
            mR->power++;
        } else if(mR->power > power) {
            mR->power--;
        }
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
	}
    setMotorPWM(mL);
    setMotorPWM(mR);
}