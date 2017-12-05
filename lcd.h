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

#define dispEnableL LATCbits.LC0        // Alias pins for use in display
#define dispDataL LATAbits.LATA6
#define disp7L LATDbits.LD1             // LD1 vs latd1
#define disp6L LATDbits.LD0
#define disp5L LATEbits.LE2
#define disp4L LATEbits.LE1
#define command 0                       // Alias commands for readability
#define data 1

void E_TOG(void);                           // Function to toggle data read - used internally
void sendNibLCD(unsigned char number);      // Function to send four bits to the LCD - used internally
void sendLCD(unsigned char Byte, char type);// Function to send data/commands over a 4bit interface
void initLCD(void);                         // Function to initialise LCD screen
void setLine (char line);                   // Function to put cursor to start of given line
void sendStrLCD(char *string);              // Function to display string on LCD
void clearLCD();                            // Clears LCD

#endif	/* LCD_H */

