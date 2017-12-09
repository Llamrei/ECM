#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
#include "anRead.h"
#include "ir_handling.h"
#include "dc_motor.h"
#include "buttonInterrupts.h"
#include "rfid_reader.h"
#include "eusart.h"
        
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif
#define PWMcycle 100 
#define bufSize 16
#define turnLeftThresh 35
#define turnRightThresh 35
#define forwardThresh 35

void delay_s(int time);
char abs(char input);
    
//Declare motors
struct DC_motor motorL, motorR; //declare 2 motor structures     

//Declare planned globals
volatile char on = 1;           //1 for debugging, should be 0 for production
volatile char checkSumTrue = 0;
volatile char bombID[bufSize];
char returning = 0;
char atSource = 0;
char direction;
int displacementL = 0;
int displacementR = 0;

void main(void){
    // <editor-fold defaultstate="collapsed" desc="Initialising hardware and motor">
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
        
    //Initialise hardware
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable);
    initEUSART(9600, 0);
    initPWM(PTPER);                 //setup PWM registers
    initButtonHigh();
    
    //Initialise motors
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
    // </editor-fold>
    
    int ir_r = 0; 
    int ir_l = 0;
    
    //Debugging flags
    char updateFlag = 0;
    char errorFlag = 0;
    
    sendStrLCD("Test message");
    while(1){
//        if(on) {
//           if(!returning){
//               if(!atSource) {
//                   ir_r = readIRCapture(rightIR,&updateFlag,&errorFlag);
//                   ir_l = readIRCapture(leftIR,&updateFlag,&errorFlag);
//                   
//                   if(ir_r - ir_l > turnRightThresh) {
//                       if(direction != 'F') {
//                           switch(direction) {
//                               case 'L':
//                                   displacementL -=                                        
//                           }
//                       }
//                   }
//                   
//               } else  {
//                   
//               }
//           } else {
//               //Returning algorithm
//           }
//        }
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

char abs(char input) {
    return input * ((input>0) - (input<0));
}

void interrupt InterruptHandlerHigh() {
    if(INTCONbits.INT0IF){
        INTCONbits.INT0F = 0;
    }
    
    if(PIR3bits.IC1IF) {
        PIR3bits.IC1IF = 0;
    }
    
    if(PIR3bits.IC3DRIF) {
        PIR3bits.IC3DRIF = 0;
    }
    
    if (PIR1bits.RCIF) {
    }
}