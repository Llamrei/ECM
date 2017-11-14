#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "eusart.h"
#include "lcd.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    LCD_Init();
    initEUSART();
    
    char textbuf[60];
    
    while(1){
        readUSART(textbuf, sizeof(buf), 0x02, 0x03);
        LCD_String(buf);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}