/*
 * File:   main.c
 * Author: Alex
 *
 * Created on 24 October 2017, 22:44
 */


#include <xc.h>
#pragma config OSC = IRCIO, WDTEN = OFF //internal oscillator, WDT off  

void main(void) {
    OSCCON = 0x72;
//    while(!OSCCONbits.IOFS);  //Doesnt work with debug
    
    INTCONbits.GIEH = 1;        //Global enable
    INTCONbits.TMR0IE = 1;      //Enable
    INTCON2bits.TMR0IP = 1;     //High
    
    T0CONbits.TMR0ON = 0;       //off
    T0CONbits.T016BIT = 1;      //8 bit
    T0CONbits.T0CS = 0;         //Fosc/4
    T0CONbits.PSA = 0;          //Enable
    T0CONbits.T0PS = 0b100;     //1:32

    TRISC = 0x00;
    LATC = 0x00;
    
    T0CONbits.TMR0ON = 1;       //on
    while(1) {
    }
    return;
}

void interrupt InterruptHandlerHigh() {
    if(INTCONbits.TMR0IF) {
        if(LATC) {
            LATC = 0x00;
        } else {
            LATC = 0xFF;
        }
        INTCONbits.TMR0IF = 0;
    }
}
