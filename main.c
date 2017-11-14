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

volatile char inputBuffer[60];
volatile char index=0;
volatile char updatingBuffer=0;
volatile char byteRead = 0;
volatile char allowedToRead = 0;

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Enable interrupts
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;
    
    //Initialise hardware
    LCD_Init();
    initEUSART();

    while(1){
        if (updatingBuffer){
            LCD_clear();
            LCD_String(&inputBuffer);       // Display text array between 0x02 and 0x03
            if (!allowedToRead){
                index = 0;                  // Reset index after finishing text array
                memset(inputBuffer,0, sizeof(inputBuffer)); // Reset input buffer after finishing text array
                updatingBuffer = 0;
             }
            __delay_ms(50);             
        }
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt low_priority InterrupHandlerHigh() {
    if(PIR1bits.RCIF) {
        byteRead = RCREG;
        if (byteRead == 0x02){          // Check for first bit = 0x02
            allowedToRead = 1;          // Start reading text array
        }
        else if (byteRead == 0x03){     // Check for last bit = 0x03
            allowedToRead = 0;          // End reading text array
        }
        else if (allowedToRead){
            inputBuffer[index++] = byteRead;    //Read value from reg
            updatingBuffer = 1;                 //Let LCD know stuff has changed   
        }
        RCSTAbits.CREN = 0; //clear error (if any)
        RCSTAbits.CREN = 1; //Enables Receiver      
    }
}