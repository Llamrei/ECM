#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
#include "ir_handling.h"
#include "dc_motor.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif
#define PWMcycle 100 

void delay_s(int time);

     

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Declare motors
    struct DC_motor motorL, motorR; //declare 2 motor structures
    
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
    initPWM(PTPER);  //setup PWM registers
    
    //some code to set initial values of each structure
    motorL.PWMperiod    = PWMcycle; //us
    motorL.dir_pin      = 2;   //Left servo is on pin B2
    motorL.direction    = 1;   //Forward
    motorL.dutyHighByte = (unsigned char *) &PDC1H;  //Address of PDC1H
    motorL.dutyLowByte  = (unsigned char *) &PDC1L;  //Address of PDC1L
    motorL.power        =  99; //Power out of 100
    motorL.power        =  0; //Power out of 100
 
    motorR.PWMperiod    = PWMcycle; //us
    motorR.dir_pin      = 0;   //Right servo is on pin B0
    motorR.direction    = 1;   //Forward
    motorR.dutyHighByte = (unsigned char *) &PDC0H;  //Address of PDC0H
    motorR.dutyLowByte  = (unsigned char *) &PDC0L;  //Address of PDC0L
    motorR.power        =  99; //Power out of 100
        
    //Initialise hardware
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable);
    setMotorPWM(&motorR);
    setMotorPWM(&motorL);
    
        
    char textbuf[16];
    char updated = 0;
    int IRvalueL, IRvalueR = 0;
            
    while(1){
        IRvalueL = readIRCapture(leftIR, &updated);
        IRvalueR = readIRCapture(rightIR, &updated);
        
        if(updated) {
          clearLCD();
          updated = 0;
        }
        
        setLine(1);
        sprintf(textbuf, "L %u R %u", IRvalueL, IRvalueR);
        sendStrLCD(textbuf);
        if(IRvalueR - IRvalueL < 200) {
            forward(&motorL, &motorR);
            setLine(2);
            sendStrLCD("Forward");
        } else if(IRvalueR > IRvalueL) {
            turnRight(&motorL, &motorR);
            setLine(2);
            sendStrLCD("Right");
        } else if (IRvalueR < IRvalueL) {
            turnLeft(&motorL, &motorR);
            setLine(2);
            sendStrLCD("Left");
        }
        
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}