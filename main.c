#include "dc_motor.h"

#pragma config OSC = IRCIO  // internal oscillator
#define PWMcycle 100        //us

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif


int calcPTPER(int period, int osc, char scaler);
void delay_new(char time);

void main(void){

    struct DC_motor motorL, motorR; //declare 2 motor structures

    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable

    int PTPER = calcPTPER(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz

    initPWM(PTPER);  //setup PWM registers

    //some code to set initial values of each structure
    motorL.PWMperiod    = PWMcycle; //us
    motorL.dir_pin      = 2;   //Left servo is on pin B2
    motorL.direction    = 1;   //Forward
    motorL.dutyHighByte = (unsigned char *) &PDC1H;  //Address of PDC1H
    motorL.dutyLowByte  = (unsigned char *) &PDC1L;  //Address of PDC1L
    motorL.power        =  50; //Power out of 100

    motorR.PWMperiod    = PWMcycle; //us
    motorR.dir_pin      = 0;   //Right servo is on pin B0
    motorR.direction    = 1;   //Forward
    motorR.dutyHighByte = (unsigned char *) &PDC0H;  //Address of PDC0H
    motorR.dutyLowByte  = (unsigned char *) &PDC0L;  //Address of PDC0L
    motorR.power        =  50; //Power out of 100
       
   while(1){
        //call your control functions, i.e. fullSpeedAhead(&motorL,&motorR);
       fullSpeedAhead(&motorL, &motorR);
       delay_new(5);
       turnLeft(&motorL, &motorR, 1);
       turnRight(&motorL, &motorR, 1);
       stop(&motorL, &motorR);
       delay_new(5);
   }

}

// Function to get value of PTPER according to formula given in book
// period in microseconds, osc in MHz
int calcPTPER(int period, int osc, char scaler) {
    return ((period*osc)/(4*scaler) - 1);
}

void delay_new(char time) {
    for(int i; i < time*20; i++){
        __delay_ms(50);
    }
}