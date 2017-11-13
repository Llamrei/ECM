#include "dc_motor.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#pragma config OSC = IRCIO  // internal oscillator
#define PWMcycle 100        //us

int calcPTPER(int period, int osc, char scaler);

void main(void){

    struct DC_motor motorL, motorR; //declare 2 motor structures

    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable

    int PTPER = calcPTPER(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz

    LCD_Init();

    initPWM(PTPER);  //setup PWM registers

    //some code to set initial values of each structure
    motorL.PWMperiod    = PWMcycle; //us
    motorL.dir_pin      = 2;   //Left servo is on pin B2
    motorL.direction    = 1;   //Forward
    motorL.dutyHighByte = &PDC1H;  //Address of PDC1H
    motorL.dutyLowByte  = &PDC1L;  //Address of PDC1L
    motorL.power        =  50; //Power out of 100

//    motorR.PWMperiod    = PWMcycle; //us
//    motorR.dir_pin      = 0;   //Right servo is on pin B0
//    motorR.direction    = 1;   //Forward
//    motorR.dutyHighByte = &PDC0H;  //Address of PDC0H
//    motorR.dutyLowByte  = &PDC0L;  //Address of PDC0L
//    motorR.power        =  50; //Power out of 100

    setMotorPWM(&motorL);
//    setMotorPWM(&motorR);
    char buf[16];
    sprintf(buf, "%x %x %x", PDC1L, PDC1H, PTPER);
    LCD_String(buf);
    SetLine(2);
    char buf2[16];
    sprintf(buf2, "%x %x %x", PDC1L, PDC1H, PTPER);
    LCD_String(buf2);
       
   while(1){
        //call your control functions, i.e. fullSpeedAhead(&motorL,&motorR);
        setMotorFullSpeed(&motorL);
   }

}

// Function to get value of PTPER according to formula given in book
// period in microseconds, osc in MHz
int calcPTPER(int period, int osc, char scaler) {
    return ((period*osc)/(4*scaler) - 1);
}