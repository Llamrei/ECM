//Alex Larionov and Lewis Tran
#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
#include "navigation.h"
#include "buttonInterrupts.h"
#include "ir_handling.h"
#include "dc_motor.h"
#include "rfid_reader.h"
#include "eusart.h"

        
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif
#define PWMcycle 100 
#define bufSize 20

//Configuring navigation
#define turnLeftThresh 30
#define turnRightThresh 30
#define spinThresh 500

//Configuring return
#define tolL 10
#define tolR 0

//For dead reckoning - not implemented
#define M_PI 3.14159265358979323846

void delay_s(int time);
    
//Declare motors
struct DC_motor motorL, motorR; //declare 2 motor structures     

//Declare planned globals
volatile char on = 1;           //1 for debugging, should be 0 for production
volatile char checkSumError = 0;
volatile char bombID[bufSize];
volatile char atSource = 0;
char returning = 0;
char direction = 'i';
char directions[100];
unsigned int rotationL[50];
unsigned int rotationR[50];
char directionIndex = 0;

//Debugging flags/vars
char updateFlag = 0;
char errorFlag = 0;
char irValues[20];
char dispValues[20];

void main(void){
    // <editor-fold defaultstate="collapsed" desc="Initialising hardware and motor">
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    INTCONbits.GIEH = 1;      //enable interrupts
    
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
        
    //Initialise hardware
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable);
    initEUSART(9600, 1);
    initPWM(PTPER);                 //setup PWM registers
    initButton();
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
           CLRWDT();
           if(!returning){
               // <editor-fold defaultstate="collapsed" desc="Navigation routine">
               if(!atSource) {
                   CLRWDT();
                   //Searching for source
                   ir_r = readIRCapture(rightIR,&updateFlag,&errorFlag);
                   ir_l = readIRCapture(leftIR,&updateFlag,&errorFlag);
                   
                   // <editor-fold defaultstate="collapsed" desc="Update screen">
                   if(updateFlag) {
                       clearLCD();
                       setLine(1);
                       sprintf(irValues, "L %d R %d", ir_r, ir_l);
                       sendStrLCD(irValues);
                       setLine(2);
                       sprintf(dispValues, "%c", direction);
                       sendStrLCD(dispValues);
                       __delay_ms(10);
                   }
                   // </editor-fold>
                   if(!(directionIndex > 49)){
                        if(ir_r < spinThresh && ir_l < spinThresh) {
                            // No signal found
                        if(direction != 'L') {
                             rotationL[directionIndex] = readTimer1();
                             rotationR[directionIndex++] = readTimer0();
                             resetTimers();
                        }
                        turnLeft(&motorL, &motorR, 50);        //Could implement proportional control
                        direction = 'L';
                        directions[directionIndex] = direction;
                        } else {
                             if(ir_r - ir_l > turnRightThresh) {
                                if(direction != 'R') {
                                    rotationL[directionIndex] = readTimer1();
                                    rotationR[directionIndex++] = readTimer0(); 
                                    resetTimers();
                                }
                                turnRight(&motorL, &motorR, 50);        //Could implement proportional control
                                direction = 'R';
                                directions[directionIndex] = direction;
                            } else if(ir_l - ir_r > turnLeftThresh) {
                                if(direction != 'L') {
                                     rotationL[directionIndex] = readTimer1();
                                     rotationR[directionIndex++] = readTimer0();
                                     resetTimers();
                                }
                                turnLeft(&motorL, &motorR, 50);        //Could implement proportional control
                                direction = 'L';
                                directions[directionIndex] = direction;
                            } else {
                                if(direction != 'F') {
                                     rotationL[directionIndex] = readTimer1();
                                     rotationR[directionIndex++] = readTimer0();
                                     resetTimers();
                                }
                                forward(&motorL, &motorR, 80);        //Could implement proportional control
                                direction = 'F';
                                directions[directionIndex] = direction;
                            }   
                        }                    
                   }
                 // </editor-fold>     
               } else  {
               // <editor-fold defaultstate="collapsed" desc="Final storing of wheel pos and disp ID">
                    // At source
                    CLRWDT();
                    stop(&motorL, &motorR);
                    rotationL[directionIndex] = readTimer1();
                    rotationR[directionIndex] = readTimer0();
                    resetTimers();
                    returning = 1;
                    clearLCD();
                    sendStrLCD(bombID);
                    setLine(2);
                    if(checkSumError) {
                        sendStrLCD("Checksum failed");
                    } else {
                        sendStrLCD("Checksum success");
                    }
                    delay_s(1);
                    // </editor-fold>     
               }
           } else {
               // <editor-fold defaultstate="collapsed" desc="Returning home">
               for(directionIndex; directionIndex > 0; directionIndex--){
                   while(readTimer1() < rotationL[directionIndex] + tolL && readTimer0() < rotationR[directionIndex] + tolR){
                       clearLCD();
                       setLine(1);
                       sprintf(irValues, "L %d R %d", readTimer1()*100/rotationL[directionIndex], readTimer0()*100/rotationR[directionIndex]);
                       sendStrLCD(irValues);
                       setLine(2);
                       sprintf(dispValues, "%c %d", directions[directionIndex], directionIndex);
                       sendStrLCD(dispValues);
                       __delay_ms(10);
                       switch(directions[directionIndex]) {
                           case 'F':
                               motorL.direction = 0;
                               motorR.direction = 0;
                               setSpeedAhead(&motorL, &motorR, 80);
                               break;
                           case 'L':
                               turnRight(&motorL, &motorR, 50);
                               break;
                           case 'R':
                               turnLeft(&motorL,&motorR,50);
                               break;
                       }
                   }
                   resetTimers();
                   CLRWDT();
                   
               }
               stop(&motorL, &motorR);
               clearLCD();
                    sendStrLCD(bombID);
                    setLine(2);
                    if(checkSumError) {
                        sendStrLCD("Checksum failed");
                    } else {
                        sendStrLCD("Checksum success");
                    }
               //</editor-fold>
           }
        }
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt InterruptHandlerHigh() {
    if(PIR1bits.RCIF) {
        char tmp;
        readUSART(&bombID, bufSize, 0x02, 0x03, &tmp);
        readRFID(&bombID, bufSize, &checkSumError);
        atSource = 1;
        PIR1bits.TXIF = 0;
    }
    
    if(INTCON3bits.INT1IF) {
        RESET();
        INTCON3bits.INT1IF = 0;
    }
}