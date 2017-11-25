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
    initEUSART();
    initADC();
        
    char textbuf[60];
    char updated = 0;
    
    while(1){
        sendCharSerial('L');
        readUSART(textbuf, sizeof(textbuf), 0x02, 0x03, &updated);
        if(updated) {
          clearLCD();
          sendCharSerial('C');
          updated = 0;
        }
        sendStrLCD(textbuf);
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}