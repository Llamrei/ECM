#include <xc.h>
#include <string.h>
#include <stdio.h>

#include "anRead.h"
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
    
    ADC_Init();
    LCD_Init();
    SetLine(1);
    
    while(1){
        char buf[40] = "";
        SendLCD(0b00000001, command);
        __delay_ms(2);
        sprintf(buf,"%d",ADC_Read());
        LCD_String(buf);
        for(int i = 0; i<4; i++){
           __delay_ms(50);
        }
    }
}
