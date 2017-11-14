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

volatile char inputBuffer[60];
volatile char index=0;

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Enable interrupts
//    INTCONbits.GIEH = 1;        
//    INTCONbits.PEIE = 1;
    
    LCD_Init();
    initEUSART();
    while(1){
//        LCD_String(&inputBuffer);
        char input = getCharSerial();
        SendLCD(input, data);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt InterrupHandlerHigh() {
    if(PIR1bits.RCIF) {
        inputBuffer[index] = RCREG;
        index++;
    }
}