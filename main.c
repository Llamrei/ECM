#include <xc.h>
#include <string.h>

#pragma config OSC = IRCIO              // Set internal clock

#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines

#define dispEnableL LATCbits.LC0        // Alias pins for use in display
#define dispDataL LATAbits.LATA6
#define disp7L LATDbits.LD1             // LD1 vs latd1
#define disp6L LATDbits.LD0
#define disp5L LATCbits.LC2
#define disp4L LATCbits.LC1
#define command 0                       // Alias commands for readability
#define data 1
//TODO: Alias all commands

void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine (char line);
void LCD_String(char *string);

void main(void){
    char buf[16];                           // Buffer for characters for LCD
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);                // Wait for OSCON to become stable
    
    LCD_Init();
    SetLine(1);                              // Set to line 1
      
    strcpy(buf,"Hello World!");
    LCD_String(buf);
    while(1);
}

// Function to toggle data read
void E_TOG(void){
   dispEnableL = 1;
__delay_us(5);                              // Delay for toggle to register
   dispEnableL = 0; 
}

// Function to send four bits to the LCD
void LCDout(unsigned char number){
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
void SendLCD(unsigned char Byte, char type){
 // Set RS pin whether it is a Command (0) or Data/Char (1)
 // using type as the argument
    dispDataL = type;
 // Send high nibble of Byte using LCDout function
    LCDout(Byte >> 4);
 __delay_us(10);                            // 10us delay to allow it to register
 // Send low nibble of Byte using LCDout function
    LCDout(Byte & 0b00001111);
}
void LCD_Init(void){
 // Set initial relevant TRIS registers to output
    TRISAbits.RA6 = 0;
    TRISC &= 0b11111000;
    TRISD &= 0b11111100;
 // Set initial LAT output values
    dispEnableL = 0;
    dispDataL = 0;
    disp7L = 0;
    disp6L = 0;
    disp5L = 0;
    disp4L = 0;
    
// Initialisation sequence code - see the data sheet
   LCDout(0b0011); 
   __delay_us(5000);
   LCDout(0b0011);
   __delay_us(200);
    LCDout(0b0011);
   __delay_us(50);
   
   LCDout(0b0010);                      //Set to 4 bit mode
   __delay_us(50);
   
   SendLCD(0b00001000,command);         //Switch off display
   __delay_us(50);
   
   SendLCD(0b00000001,command);         //Clear display
   __delay_us(1700);

   SendLCD(0b00000110,command);         //Set entry mode
   __delay_us(50);
   
   SendLCD(0b00001100, command);        //Switch display on
   __delay_us(50);
}

// Function to put cursor to start of line
void SetLine (char line){
// Send 0x80 to set line to 1 (0x00 ddram address)
// Send 0xC0 to set line to 2 (0x40 ddram address)
    if(line == 1) {
        SendLCD(0x80,command);
    } else if (line == 2) {
        SendLCD(0xC0,command);
    } else {
        SendLCD(0x80,command);           // If other line given set to 1
    }
 __delay_us(50); // 50us delay
}

void LCD_String(char *string){
    while(*string != 0){                // Increment until null (0x00) reached
        SendLCD(*string++,data);        // Send each character
        __delay_ms(50);                 // Short delay to print each character
    }
}