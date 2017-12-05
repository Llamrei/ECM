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

int abs(int arg) {
    return arg * ((arg>0) - (arg<0));
}
    
//Declare motors
struct DC_motor motorL, motorR; //declare 2 motor structures     

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Initialise hardware   
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
    initPWM(PTPER);  //setup PWM registers
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable); 
    
    //Initialise motor structs
    motorL.PWMperiod    = PWMcycle; //us
    motorL.dir_pin      = 2;   //Left servo is on pin B2
    motorL.direction    = 1;   //Forward
    motorL.dutyHighByte = (unsigned char *) &PDC1H;  //Address of PDC1H
    motorL.dutyLowByte  = (unsigned char *) &PDC1L;  //Address of PDC1L
    motorL.power        =  0; //Power out of 100
    motorR.PWMperiod    = PWMcycle; //us
    motorR.dir_pin      = 0;   //Right servo is on pin B0
    motorR.direction    = 1;   //Forward
    motorR.dutyHighByte = (unsigned char *) &PDC0H;  //Address of PDC0H
    motorR.dutyLowByte  = (unsigned char *) &PDC0L;  //Address of PDC0L
    motorR.power        =  0; //Power out of 100 
    setMotorPWM(&motorR);
    setMotorPWM(&motorL);
  
    char textbuf[16];
    char updated = 0;
    unsigned int IRvalueL = 0, IRvalueR = 0;
            
    while(1){
        CLRWDT();                      

        char leftError = 0, rightError = 0;
        IRvalueL = readIRCapture(leftIR, &updated, &leftError);  
        IRvalueR = readIRCapture(rightIR, &updated, &rightError);            

        if(updated) {
            clearLCD();
            updated = 0;
        }
        setLine(1);
        sprintf(textbuf, "L - R = %d", (signed int) IRvalueL - IRvalueR);
        sendStrLCD(textbuf);
        int diff = (signed int)IRvalueL - (signed int) IRvalueR;
                
        if(IRvalueL < 1000 && IRvalueR < 1000){
        // If both sensors drop signal we want to turn back, or at least turn instead of moving forward
            if (motorR.dir_pin != motorL.dir_pin) {
                motorR.dir_pin = ~motorR.dir_pin;
                motorL.dir_pin = ~motorL.dir_pin;
                setSpeedAhead(&motorL, &motorR, 50);
            } else {
                turnLeft(&motorL, &motorR, 50);
            }
        }
        else if(abs(diff) < 35) {
        //If we have a difference within a threshold we can assume that to be just noise and go forward
            forward(&motorL, &motorR, 30);
            setLine(2);
            sendStrLCD("Forward");
        } else if(IRvalueR > IRvalueL) {
        //Otherwise turn to stronger signals
            turnRight(&motorL, &motorR, 50);
            setLine(2);
            sendStrLCD("Right");
        } else if (IRvalueR < IRvalueL) {
            turnLeft(&motorL, &motorR, 50);
            setLine(2);
            sendStrLCD("Left");
        }
        
        __delay_ms(50);
    }
}

void interrupt InterruptHandlerHigh() {
    if(INTCONbits.INT0IF){
        
        INTCONbits.INT0F = 0;
    }
    
    
    if(PIR3bits.IC3DRIF) {

        PIR3bits.IC3DRIF = 0;
    }
    
    if (PIR1bits.RCIF) {
        
    }
}