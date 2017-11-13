#include <xc.h>
#include "dc_motor.h"

// Function to get value of PTPER according to formula given in book
// period in microseconds, osc in MHz
int calcPTPER(int period, int osc, char scaler) {
    return ((period*osc)/(4*scaler) - 1);
}

void initPWM(int PWMperiod){
    PTCON0 = 0b00001100;    // free running mode, 1:64 prescaler = 32 us
    PTCON1 = 0b10000000;    // enable PWM timer
    PWMCON0 = 0b00111111;   // PWM0/1/2/3 enabled, all independent mode
    PWMCON1 = 0x00;         // special features, all 0 (default)
    PTPERL = PWMperiod & 0b0000000011111111; // base PWM period low byte
    PTPERH = PWMperiod >> 8; // base PWM period high byte
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
    *(m->dutyLowByte)=PWMduty<<2;
    *(m->dutyHighByte)=PWMduty>>6;
}

//increases a motor to full power over a period of time
void setMotorFullSpeed(struct DC_motor *m)
{
	for (m->power; (m->power)<=100; m->power++){ //increase motor power until 100
		setMotorPWM(m);	//pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(1);	//delay of 1 ms (100 ms from 0 to 100 full power)
	}
}

//function to stop a motor gradually 
void stopMotor(struct DC_motor *m)
{
	// a loop to slow the motor power down to zero
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
	// a loop to slow both motors down to zero power
}

//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
	//remember to change the power gradually
}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
	//remember to change the power gradually
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
	//remember to change the power gradually
}
