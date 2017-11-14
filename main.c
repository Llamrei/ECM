#pragma config OSC = IRCIO  // internal oscillator

#include <string.h>
#include <xc.h>
#include <stdio.h>

#include "eusart.h"
#include "lcd.h"
#include "anRead.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

void delay_s(int time);

volatile char inputBuffer[60];
volatile char index=0;
volatile char updatingBuffer=0;

void main(void){
    
    OSCCON = 0x72; //8MHz clock
    while(!OSCCONbits.IOFS); //wait until stable
    
    //Enable interrupts
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;
    
    //Initialise hardware
    LCD_Init();
    initEUSART();
    initADC();
    
    int digitalReading;
    int int_part;
    int frac_part;
    
    while(1){
        char voltage[60] = "";
        digitalReading = readADC();
+       int_part = (int) (digitalReading/204.5);
+       frac_part= (int) (digitalReading/2.045) - int_part*100;
+       sprintf(voltage,"%d.%02d V",int_part,frac_part);
        sendStrSerial(voltage);
        LCD_clear();
        LCD_String(voltage);
        __delay_ms(50);
    }
}

void delay_s(int time) {
    for(int i = 0; i < time*20; i++){
           __delay_ms(50);
    }
}

void interrupt low_priority InterrupHandlerHigh() {
    if(PIR1bits.RCIF) {
        inputBuffer[index] = RCREG;   //Read value from reg
        updatingBuffer = 1;           //Let LCD know stuff has changed   
        RCSTAbits.CREN = 0; //clear error (if any)
        RCSTAbits.CREN = 1; //Enables Receiver
        
    }
}