#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
#include "navigation.h"
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
#define turnLeftThresh 100
#define turnRightThresh 100
#define forwardThresh 100

void delay_s(int time);
char abs(char input);
    
//Declare motors
struct DC_motor motorL, motorR; //declare 2 motor structures     

//Declare planned globals
volatile char on = 1;           //1 for debugging, should be 0 for production
volatile char checkSumError = 0;
volatile char bombID[bufSize];
volatile char atSource = 0;
char returning = 0;
char direction = 'i';
int displacementL = 0;
int displacementR = 0;

//Debugging flags
char updateFlag = 0;
char errorFlag = 0;

void main(void){
    // <editor-fold defaultstate="collapsed" desc="Initialising hardware and motor">
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    INTCONbits.GIE = 0;      //enable interrupts
    
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
        
    //Initialise hardware
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable);
    initEUSART(9600, 0);
    initPWM(PTPER);                 //setup PWM registers
//    initButtonHigh();
    initTimers();                   //Left wheel encoder on T1, right on T0
    
    //Ansel bugs?
    ANSEL0 = 0;
    ANSEL1 = 0;
    
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
        
    while(1){
        if(on) {
           if(!returning){
               if(!atSource) {
                   CLRWDT();
                   //Searching for source
                   ir_r = readIRCapture(rightIR,&updateFlag,&errorFlag);
                   ir_l = readIRCapture(leftIR,&updateFlag,&errorFlag);
                   
                   if(updateFlag) {
                       char irValues[16];
                       clearLCD();
                       setLine(1);
                       sprintf(irValues, "L %d R %d", ir_r, ir_l);
                       sendStrLCD(irValues);
                       setLine(2);
                       sendStrLCD(&direction);
                       __delay_ms(10);
                   }
                   
                   if(ir_r - ir_l > turnRightThresh) {
                       if(direction != 'R') {
                           switch(direction) {
                               case 'L':
                                   displacementL -= readTimer1();
                                   displacementR += readTimer0();
                               case 'F':
                                   displacementL += readTimer1();
                                   displacementR += readTimer0();  
                           }
                           resetTimers();
                       }
                       turnRight(&motorL, &motorR, 50);        //Could implement proportional control
                       direction = 'R';
                   } else if(ir_l - ir_r > turnLeftThresh) {
                       if(direction != 'L') {
                           switch(direction) {
                               case 'R':
                                   displacementL += readTimer1();
                                   displacementR -= readTimer0();
                               case 'F':
                                   displacementL += readTimer1();
                                   displacementR += readTimer0();  
                           }
                           resetTimers();
                       }
                       turnLeft(&motorL, &motorR, 50);        //Could implement proportional control
                       direction = 'L';
                   } else {
                       if(direction != 'F') {
                           switch(direction) {
                               case 'R':
                                   displacementL += readTimer1();
                                   displacementR -= readTimer0();
                               case 'L':
                                   displacementL -= readTimer1();
                                   displacementR += readTimer0();  
                           }
                           resetTimers();
                       }
                       forward(&motorL, &motorR, 50);        //Could implement proportional control
                       direction = 'F';
                   }
                   
               } else  {
                    // At source
                   stop(&motorL, &motorR);
                   switch(direction) {
                        case 'R':
                            displacementL += readTimer1();
                            displacementR -= readTimer0();
                        case 'L':
                            displacementL -= readTimer1();
                            displacementR += readTimer0(); 
                        case 'F':
                            displacementL += readTimer1();
                            displacementR += readTimer0(); 
                    }
                    resetTimers();
                    returning = 1;
                    sendStrLCD(bombID);
                    setLine(2);
                    if(errorFlag) {
                        sendStrLCD("Checksum failed");
                    } else {
                        sendStrLCD("Checksum success");
                    }
               }
           } else {
               //Returning algorithm
           }
        }
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

//void interrupt InterruptHandlerHigh() {
////    if(PIR1bits.TXIF) {
////        char tmp;
//////        readUSART(&bombID, bufSize, 0x02, 0x03, &tmp);
//////        readRFID(&bombID, bufSize, &checkSumError);
//////        atSource = 1;
////        //PIR1bits.TXIF = 0;
////    }
//}