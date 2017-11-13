#include <xc.h>
#include "anRead.h"
#include "buttonInterrupts.h"
#include "servoControl.h"

#pragma config OSC = IRCIO              // Set internal clock

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

//Prototype definition
void delay_s(char seconds); // function to delay in seconds

volatile int threshold = 512;

void main(void){   
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);    // Wait for OSCON to become stable
    
    initADC();                  //Set up analogue to digital convertor pins
    initButtonHigh();           //Set up interrupt flags and configure them to listen on button
    initPWM();                  //setup PWM registers
    
    while(1){
        //If darker than threshold we are in night
        if(readADC() > threshold) {
            angle2PWM(-90);     //So move window to -90: 'closed'
        }else { 
            angle2PWM(90);      //Else it must be day so move to 90: 'open'
        }
    }
}

//delay_s abstracts repeated calls to _delay_ms to surpass limit of 87ms
void delay_s(char seconds) {
    for (int i = 0; i < (seconds*1000)/50; i++) {
        __delay_ms(50);
    }
}

//InterruptHandlerHigh() enables handling of high priority interrupt signals)
void interrupt InterruptHandlerHigh() {
    if(INTCONbits.INT0IF) {
        threshold = readADC();
        INTCONbits.INT0F = 0;
    }    
}