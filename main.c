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
    initIRCapture(leftIR, resetEnable);
    initIRCapture(rightIR, resetEnable);
        
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