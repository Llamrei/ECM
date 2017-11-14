#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "eusart.h"
#include "lcd.h"
#include "anRead.h"
#include "dc_motor.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

volatile char inputBuffer[60];
volatile char index=0;
volatile char updatingBuffer=0;
volatile char byteRead = 0;
volatile char allowedToRead = 0;
volatile char allowedToCmd = 0;
volatile char cmdBuffer[60];
volatile char cmdIndex = 0;
volatile char executingBuffer = 0;

void main(void){
    
    
    struct DC_motor mL, mR;
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Enable interrupts
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;
    
    //Initialise hardware
    LCD_Init();
    initEUSART();
    int PWMCycle = 100;     //us
    initPWM(getPT(PWMCycle, 8, 1));
    
    // Initialise motors
    mL.PWMperiod = PWMCycle;
    mL.dir_pin = 2;
    mL.direction = 1;
    mL.dutyHighByte = (unsigned char*) &PDC1H;
    mL.dutyLowByte = (unsigned char*) &PDC1L;
    mL.power = 0;
    
    mR = mL;
    
    mR.dir_pin = 0;
    mR.dutyHighByte = (unsigned char*) &PDC0H;
    mR.dutyLowByte = (unsigned char*) &PDC0L;
    char executingIndex = 0;
    
    while(1){
        if (updatingBuffer){
            sendCharSerial('l');
            LCD_clear();
            LCD_String(&inputBuffer);       // Display text array between 0x02 and 0x03
            if (!allowedToRead){
                index = 0;                  // Reset index after finishing text array
                memset(inputBuffer,0, sizeof(inputBuffer)); // Reset input buffer after finishing text array
                updatingBuffer = 0;
                sendCharSerial('e');
             }
            __delay_ms(50);             
        }
        if (executingBuffer){
            switch (cmdBuffer[executingIndex++]){
                case 0xF0:
                    stop(&mL, &mR);
                    break;
            }
            if(!allowedToCmd){
                cmdIndex = 0;
                executingBuffer = 0;
            }
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
        sendCharSerial(byteRead);
        if (byteRead == 'a'){          // Check for first bit = 0x02
            allowedToRead = 1;          // Start reading text array
        }
        else if (byteRead == 'z'){     // Check for last bit = 0x03
            allowedToRead = 0;          // End reading text array
        }
        // Motor commands
        else if (byteRead == 0x01){     // Check for first bit = 0x01
            allowedToCmd = 1;           // Start reading first command
        }
        else if (byteRead == 0x04){     // Check for last bit = 0x04
            allowedToCmd = 0;           // End reading text array
        }
        else if (allowedToRead){
            inputBuffer[index++] = byteRead;    //Read value from reg
            updatingBuffer = 1;                 //Let LCD know stuff has changed
            sendCharSerial('r');
        }
        else if (allowedToCmd){
            cmdBuffer[cmdIndex++] = byteRead;
            executingBuffer = 1;
        }
        
        RCSTAbits.CREN = 0; //clear error (if any)
        RCSTAbits.CREN = 1; //Enables Receiver      
    }
}