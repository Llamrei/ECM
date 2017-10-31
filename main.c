/*
 * File:   main.c
 * Author: Alex
 *
 * Created on 24 October 2017, 22:44
 */

#include <xc.h>
#define _XTAL_FREQ 8000000          //Set 8MHz clock 
#define dispWrite LATCbits.LC0      //Alias pins for use in display
#define dispReset LATAbits.RA6

void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine (char line);
void initialiseLED();

void main(void) {

    initialiseLED();
    
}

void initialiseLED() {
    
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
   dispWrite = 1;
__delay_us(46); // 46us delay ? remember to define _XTAL_FREQ
   dispWrite = 0; 
}
//function to send four bits to the LCD
void LCDout(unsigned char number){
//set data pins using the four bits from number
    
 //toggle the enable bit to send data
__delay_us(5); // 5us delay
}
//function to send data/commands over a 4bit interface
void SendLCD(unsigned char Byte, char type){
 // set RS pin whether it is a Command (0) or Data/Char (1)
 // using type as the argument
 // send high bits of Byte using LCDout function
 __delay_us(10); // 10us delay
 // send low bits of Byte using LCDout function
}
void LCD_Init(void){
 // set initial LAT output values (they start up in a random state)
 // set LCD pins as output (TRIS registers)
 // Initialisation sequence code - see the data sheet
 //delay 15mS
 //send 0b0011 using LCDout
 //delay 5ms
 //send 0b0011 using LCDout
 //delay 200us
 //send 0b0011 using LCDout
 //delay 50us
 //send 0b0010 using LCDout set to four bit mode
 // now use SendLCD to send whole bytes ? send function set, clear
 // screen, set entry mode, display on etc to finish initialisation
}
//function to put cursor to start of line
void SetLine (char line){
//Send 0x80 to set line to 1 (0x00 ddram address)
//Send 0xC0 to set line to 2 (0x40 ddram address)
 __delay_us(50); // 50us delay
}
