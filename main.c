/*
 * File:   main.c
 * Author: Alex
 *
 * Created on 24 October 2017, 22:44
 */

#include <xc.h>

#pragma config OSC = IRCIO          // Set internal clock

#define _XTAL_FREQ 8000000          // Set 8MHz clock for delay routines

#define dispEnableL LATCbits.LC0      //Alias pins for use in display
#define dispDataL LATAbits.LA6
#define disp7L LATDbits.LD1
#define disp6L LATDbits.LD0
#define disp5L LATCbits.LC2
#define disp4L LATCbits.LC1
#define command 0                     //Alias commands for readability
#define data 1
//TODO: Alias all commands


void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine (char line);

void main(void) {
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);
    
    LCD_Init();
    
    SetLine(1);
    SendLCD(0x48,1); //H
    SendLCD(0x65,1); //E
    SendLCD(0x6C,1); //L
    SendLCD(0x6C,1); //L
    SendLCD(0x6F,1); //O
    SendLCD(0x20,1); //(space)
    SendLCD(0x57,1); //W
    SendLCD(0x6F,1); //O
    SendLCD(0x72,1); //R
    SendLCD(0x6C,1); //L
    SendLCD(0x64,1); //D
    SendLCD(0x21,1); //!
    while(1);
    
    
}

// don?t forget to put function prototypes at the start of your program
// precise delays can be produced using the __delay_us() and __delay_ms()
// macro functions. For these to work you must write the line
// #define _XTAL_FREQ 8000000 //i.e. for an 8MHz clock frequency
// near the top of your main.c file so the functions can calculate how
// long to make the delay for different clock frequencies.
//function to toggle enable bit on then off
void E_TOG(void){
//don?t forget to put a delay between the on and off
//commands! 5us will be plenty.
   dispEnableL = 1;
__delay_us(5); // 5us delay ? remember to define _XTAL_FREQ
   dispEnableL = 0; 
}
//function to send four bits to the LCD
void LCDout(unsigned char number){
//set data pins using the four bits from number
  disp7L =  number >> 3 & 0b00000001;
  disp6L = (number >> 2) & 0b00000001;
  disp5L = (number >> 1) & 0b00000001;
  disp4L = number & 0b00000001;
 //toggle the enable bit to send data
  E_TOG();
__delay_us(5); // 5us delay
}
//function to send data/commands over a 4bit interface
void SendLCD(unsigned char Byte, char type){
 // set RS pin whether it is a Command (0) or Data/Char (1)
 // using type as the argument
    dispDataL = type;
 // send high bits of Byte using LCDout function
    LCDout(Byte >> 4);
 __delay_us(10); // 10us delay
 // send low bits of Byte using LCDout function
    LCDout(Byte & 0b00001111);
}
void LCD_Init(void){
    //set initial relevant TRIS registers to output
    TRISAbits.RA6 = 0;
    TRISC &= 0b11111000;
    TRISD &= 0b11111100;
    // set initial LAT output values (they start up in a random state)
    dispEnableL = 0;
    dispDataL = 0;
    disp7L = 0;
    disp6L = 0;
    disp5L = 0;
    disp4L = 0;
    
 // Initialisation sequence code - see the data sheet
   LCDout(0b0011);  // Initialise by instruction
   __delay_us(5000);
   LCDout(0b0011);
   __delay_us(200);
    LCDout(0b0011);
   __delay_us(50);
 //send 0b0010 using LCDout set to four bit mode
   LCDout(0b0010);
   __delay_us(50);
   
   SendLCD(0b00001000,command);         //switch off display
   __delay_us(50);
   
   SendLCD(0b00000001,command);       //Clear display
   __delay_us(1700);

   SendLCD(0b00000110,command);       //Set entry mode
   __delay_us(50);
   
   SendLCD(0b00001100, command);
   __delay_us(50);
 // now use SendLCD to send whole bytes ? send function set, clear
 // screen, set entry mode, display on etc to finish initialisation
}
//function to put cursor to start of line
void SetLine (char line){
//Send 0x80 to set line to 1 (0x00 ddram address)
//Send 0xC0 to set line to 2 (0x40 ddram address)
    if(line == 1) {
        SendLCD(0x80,command);
    } else if (line == 2) {
        SendLCD(0xC0,command);
    } else {
        SendLCD(0x80,command);
    }
 __delay_us(50); // 50us delay
}
