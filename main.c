#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "eusart.h"
#include "lcd.h"
#include "anRead.h"

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
    initEUSART(9600, 0);
    initADC();
        
    char textbuf[16];
    char updated = 0;
    
    setLine(1);
    char debuggingBaud[10];
    sprintf(debuggingBaud, "SPBRG %d", SPBRG);
    sendStrLCD(debuggingBaud);
        
    while(1){
        readUSART(textbuf, sizeof(textbuf), 0x02, 0x03, &updated);
        if(updated) {
          clearLCD();
          updated = 0;
        }
        setLine(2);
        sendStrLCD(textbuf);
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}