#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "eusart.h"
#include "lcd.h"
#include "dc_motor.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    char PWMPeriod = 100;    //Length of overall pwm cycle in us
    
    // Initialise hardware registers
    LCD_Init();
    initEUSART();
    initPWM(getPT(PWMPeriod, 8, 1));
    
    struct DC_motor mL, mR;
    
    //some code to set initial values of each structure
    mL.PWMperiod    = PWMPeriod; //us
    mL.dir_pin      = 2;   //Left servo is on pin B2
    mL.direction    = 1;   //Forward
    mL.dutyHighByte = (unsigned char *) &PDC1H;  //Address of PDC1H
    mL.dutyLowByte  = (unsigned char *) &PDC1L;  //Address of PDC1L
    mL.power        =  0; //Power out of 100

    mR.PWMperiod    = PWMPeriod; //us
    mR.dir_pin      = 0;   //Right servo is on pin B0
    mR.direction    = 1;   //Forward
    mR.dutyHighByte = (unsigned char *) &PDC0H;  //Address of PDC0H
    mR.dutyLowByte  = (unsigned char *) &PDC0L;  //Address of PDC0L
    mR.power = 0; //Power out of 100
    
    char cmdbuf[60];
    char updated = 0;
    
    while(1){
        sendCharSerial('L');
        readUSART(cmdbuf, sizeof(cmdbuf), 0x01, 0x04, &updated);
        for(unsigned int i = 0; i < sizeof(cmdbuf); i++){
            switch (cmdbuf[i]){
                case 0xF0:
                    forward(&mL, &mR);
                    break;
                case 0xF1:
                    turnLeft(&mL, &mR);
                    break;
                case 0xF2:
                    turnRight(&mL, &mR);
                    break;
                case 0xF3:
                    stop(&mL, &mR);
                    break;
            }
        }
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}