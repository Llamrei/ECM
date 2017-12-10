//Alex Larionov and Lewis Tran
#include <xc.h>
#include "lcd.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif



// Function to toggle data read
void E_TOG(void){
   dispEnableL = 1;
__delay_us(5);                              // Delay for toggle to register
   dispEnableL = 0; 
}

// Function to send four bits to the LCD
void sendNibLCD(unsigned char number){
// Set data pins using the four bits from number
  disp7L =  number >> 3 & 0b00000001;
  disp6L = (number >> 2) & 0b00000001;
  disp5L = (number >> 1) & 0b00000001;
  disp4L = number & 0b00000001;
 // Toggle the enable bit to send data
  E_TOG();
__delay_us(5);                              // 5us delay to allow it to register
}

// Function to send data/commands over a 4bit interface
void sendLCD(unsigned char Byte, char type){
 // Set RS pin whether it is a Command (0) or Data/Char (1)
 // using type as the argument
    dispDataL = type;
 // Send high nibble of Byte using LCDout function
    sendNibLCD(Byte >> 4);
 __delay_us(10);                            // 10us delay to allow it to register
 // Send low nibble of Byte using LCDout function
    sendNibLCD(Byte & 0b00001111);
}

// Function to initialise LCD screen
void initLCD(void){
 // Set initial relevant TRIS registers to output
    TRISAbits.RA6 = 0;
    TRISC &= 0b11111001;
    TRISD &= 0b11111100;
    TRISE &= 0b11111110;
 // Set initial LAT output values
    dispEnableL = 0;
    dispDataL = 0;
    disp7L = 0;
    disp6L = 0;
    disp5L = 0;
    disp4L = 0;
    
// Initialisation sequence code - see the data sheet
   sendNibLCD(0b0011); 
   __delay_us(5000);
   sendNibLCD(0b0011);
   __delay_us(200);
    sendNibLCD(0b0011);
   __delay_us(50);
   
   sendNibLCD(0b0010);                      //Set to 4 bit mode
   __delay_us(50);
   
   sendLCD(0b00001000,command);         //Switch off display
   __delay_us(50);
   
   sendLCD(0b00000001,command);         //Clear display
   __delay_us(1700);

   sendLCD(0b00000110,command);         //Set entry mode (Display is shifted LSB = 0)
   __delay_us(50);
   
   sendLCD(0b00001110, command);        //Switch display on
   __delay_us(50);
}

// Function to put cursor to start of line
void setLine (char line){
// Send 0x80 to set line to 1 (0x00 ddram address)
// Send 0xC0 to set line to 2 (0x40 ddram address)
    if(line == 1) {
        sendLCD(0x80,command);
    } else if (line == 2) {
        sendLCD(0xC0,command);
    } else {
        sendLCD(0x80,command);           // If other line given set to 1
    }
 __delay_us(50); // 50us delay
}

// Function to display text on LCD
void sendStrLCD(char *string){
    while(*string != 0){                // Increment until null (0x00) reached
        sendLCD(*string++,data);        // Send each character as data
    }
}

void clearLCD() {
   sendLCD(0b00000001,command);         //Clear display
   __delay_us(1700);
}
