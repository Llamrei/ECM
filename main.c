#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
<<<<<<< Updated upstream
#include "anRead.h"
=======
#include "ir_handling.h"
#include "dc_motor.h"
#include "buttonInterrupts.h"
#include "rfid_reader.h"
#include "eusart.h"
>>>>>>> Stashed changes

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);
<<<<<<< Updated upstream
=======
volatile char on = 0;
volatile char cardPresent = 0;
volatile char CAP1update = 1;
volatile char CAP3update = 1;
char searching = 1;
    
//Declare motors
struct DC_motor motorL, motorR; //declare 2 motor structures     
>>>>>>> Stashed changes

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
<<<<<<< Updated upstream
    //Enable interrupts
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;
    
    //Initialise hardware
    initLCD();
    initADC();
        
    char textbuf[16];
    char updated = 0;
            
    while(1){
        if(updated) {
          clearLCD();
          updated = 0;
          setLine(1);
        }
        sendStrLCD(textbuf);
        __delay_ms(50);
=======
    //Construct PWM signal
    int PTPER = getPT(PWMcycle, 8, 1);   //Get pwm cycle length for 10kHz
    initPWM(PTPER);  //setup PWM registers
    initButtonHigh();
    initEUSART(9600, 0); 
        
    //Initialise hardware
    initLCD();
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable); 
    
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
  
    char textbuf[16];
    char updated = 0;
    unsigned int IRvalueL = 0, IRvalueR = 0;
    char bombCode[16] = "z";
            
    while(1){
        while(bombCode[0] = 'z'){
            CLRWDT();
            if(cardPresent){
                readUSART(bombCode, 16, 0x02, 0x03, 0);
                readRFID(bombCode, 16);  
            }                       
            
            char leftError = 0, rightError = 0;
            IRvalueL = readIRCapture(leftIR, &updated, &leftError);  
            IRvalueR = readIRCapture(rightIR, &updated, &rightError);            

            if(updated) {
                clearLCD();
                updated = 0;
            }
            setLine(1);
            sprintf(textbuf, "L %d R %d", leftError ? leftError : IRvalueL, rightError ? rightError : IRvalueR);
            sendStrLCD(textbuf);

            if(searching) {

                if(IRvalueL - IRvalueR > 0 ? IRvalueL - IRvalueR < 100 : IRvalueL - IRvalueR < 100) {
                    forward(&motorL, &motorR, 50);
                    setLine(2);
                    sendStrLCD("Forward");
                } else if(IRvalueR > IRvalueL) {
                    turnRight(&motorL, &motorR, 50);
                    setLine(2);
                    sendStrLCD("Right");
                } else if (IRvalueR < IRvalueL) {
                    turnLeft(&motorL, &motorR, 50);
                    setLine(2);
                    sendStrLCD("Left");
                }
            }

            __delay_ms(50);
        }
>>>>>>> Stashed changes
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt InterruptHandlerHigh() {
    if(INTCONbits.INT0IF){
        if(on) {
            on = 0;
            stop(&motorL,&motorR);
        } else {
            on = 1;
        }
        INTCONbits.INT0F = 0;
    }
    
    if(PIR3bits.IC1IF) {
        CAP1update = 1;
        PIR3bits.IC1IF = 0;
    }
    
    if(PIR3bits.IC3DRIF) {
        CAP3update = 1;
        PIR3bits.IC3DRIF = 0;
    }
    
    if (PIR1bits.RCIF) {
        cardPresent = 1;
    }
}