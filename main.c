#include <xc.h>
#include <string.h>
#include <stdio.h>

#include "anRead.h"
#include "lcd.h"

#pragma config OSC = IRCIO              // Set internal clock

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif
//TODO: Alias all commands

void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine (char line);
void LCD_String(char *string);

void main(void){
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);                // Wait for OSCON to become stable
    
    ADC_Init();
    LCD_Init();
    SetLine(1);
    
    while(1){
        char buf[40] = "";
        unsigned int int_part = 0;
        unsigned int frac_part = 0;
        int ADC_Result = 0;
        SendLCD(0b00000001, command);
        __delay_ms(2);
        ADC_Result = ADC_Read();
        int_part = (unsigned int) (ADC_Result/204.6);
        frac_part= (unsigned int) (ADC_Result/2.046) - int_part*100;
        sprintf(buf,"%d.%02d",int_part,frac_part);
        LCD_String(buf);
        for(int i = 0; i<4; i++){
           __delay_ms(50);
        }
    }
}
