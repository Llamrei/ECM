#include <xc.h>
#include <string.h>
#include <stdio.h>

#include "lcd.h"

#pragma config OSC = IRCIO              // Set internal clock

#ifndef _XTAL_FREQ 8000000
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif
//TODO: Alias all commands

void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine (char line);
void LCD_String(char *string);

volatile int btnCount = 0;
volatile char buf[40] = 0;

void main(void){
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);                // Wait for OSCON to become stable

    // Enable interrupt handling by button
    TRISCbits.RC3 = 1;       //Set button to input
    INTCONbits.GIEH = 1;     //Globally enable interrupts
    INTCONbits.INT0IE = 1;   //Enable external interrupt on INT0
    
    LCD_Init();
    SetLine(1);
    
    while(1){
    }
}
    
    void interrupt InterruptHandlerHigh() {
        if(INTCONbits.INT0IF) {
            btnCount++;
            SendLCD(0b00000001, command); //Clear display
            __delay_ms(2);                //Let display update
            sprintf(buf,"%d", btnCount);
            LCD_String(buf);
            INTCONbits.INT0IF = 0;
        }
    }