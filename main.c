#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "lcd.h"
#include "ir_handling.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Enable interrupts
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;
    
    //Initialise hardware
    initLCD();
    initIRCapture();

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
        sprintf(textbuf, "PWL %u *4 us", IRvalueL);
        sendStrLCD(textbuf);
        setLine(2);
        sprintf(textbuf, "PWR %u *4 us", IRvalueR);
        sendStrLCD(textbuf);
        
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt InterruptHandler() {
    if(PIR1bits.CCP1IF) {   
        if(CCP1CON = 0b00000100) {
            // If configured for falling edge
            //Set to read for rising edge
            setEdgeCapture(1,rising);
            TMR1 = 0;
        } else if(CCP1CON = 0b00000100) {
            // Else if configured for rising edge set to falling and save info
            setEdgeCapture(1,falling);
            time1 = ((unsigned int) CCPR1H << 8) + CCPR1H;
            readFlag1 = 1;
        }
        PIR1bits.CCP1IF = 0;
    }
    if(PIR2bits.CCP2IF) {
        if(CCP2CON = 0b00000100) {
            // If configured for falling edge
            //Set to read for rising edge
            setEdgeCapture(2,rising);
            TMR1 = 0;
        } else if(CCP2CON = 0b00000100) {
            // Else if configured for rising edge set to falling and save info
            setEdgeCapture(2,falling);
            time1 = ((unsigned int) CCPR1H << 8) + CCPR1H;
            readFlag2 = 1;
        }
        PIR2bits.CCP2IF = 0;
    }
}