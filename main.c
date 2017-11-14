#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <stdio.h>
#include <xc.h>

#include "dc_motor.h"
#include "lcd.h"

#define PWMcycle 100        //us

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

void main(void){

    struct DC_motor motorL, motorR; //declare 2 motor structures

    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable

    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz

    LCD_Init();
    initPWM(PTPER);  //setup PWM registers

    //some code to set initial values of each structure
    motorL.PWMperiod    = PWMcycle; //us
    motorL.dir_pin      = 2;   //Left servo is on pin B2
    motorL.direction    = 1;   //Forward
    motorL.dutyHighByte = (unsigned char *) &PDC1H;  //Address of PDC1H
    motorL.dutyLowByte  = (unsigned char *) &PDC1L;  //Address of PDC1L
    motorL.power        =  99; //Power out of 100

    motorR.PWMperiod    = PWMcycle; //us
    motorR.dir_pin      = 0;   //Right servo is on pin B0
    motorR.direction    = 1;   //Forward
    motorR.dutyHighByte = (unsigned char *) &PDC0H;  //Address of PDC0H
    motorR.dutyLowByte  = (unsigned char *) &PDC0L;  //Address of PDC0L
    motorR.power        =  99; //Power out of 100
   setMotorPWM(&motorR);
   setMotorPWM(&motorL);     
   while(1){
        char buf[16];
      
        LCD_clear();
        LCD_String("Forward");
        forward(&motorL,&motorR);
        delay_s(1);
       
        LCD_clear();
        SetLine(1);
        LCD_String("Left");
        turnLeft(&motorL,&motorR);
        delay_s(1);
        
        LCD_clear();
        SetLine(1);
        LCD_String("Right");
        turnRight(&motorL,&motorR);
        delay_s(1);

        LCD_clear();
        LCD_String("Stop");
        stop(&motorL, &motorR);
        delay_s(1);
   }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}