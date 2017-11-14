/* 
 * File:   lcd.h
 * Author: lt2415
 *
 * Created on 31 October 2017, 15:32
 * Requires:
 *  #pragma config OSC = IRCIO       
    #define _XTAL_FREQ 8000000 
 */

#ifndef LCD_H
#define	LCD_H

void E_TOG(void);                           // Function to toggle data read - used internally
void sendNibLCD(unsigned char number);      // Function to send four bits to the LCD - used internally
void sendLCD(unsigned char Byte, char type);// Function to send data/commands over a 4bit interface
void initLCD(void);                         // Function to initialise LCD screen
void setLine (char line);                   // Function to put cursor to start of given line
void sendStrLCD(char *string);              // Function to display string on LCD
void clearLCD();                            // Clears LCD

#endif	/* LCD_H */

