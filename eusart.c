#include <xc.h>
#include "eusart.h"

void initEUSART() {
    //set data direction registers
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
       
    // Calculation of baud rate from dependent on set values
    SPBRG=101; //set baud rate to 9600
    SPBRGH=0;
    
    // Basic config
    BAUDCONbits.BRG16=1; //set baud rate scaling to 16 bit mode
    TXSTAbits.BRGH=1; //high baud rate select bit
    TXSTAbits.SYNC = 0; //Set to asynchronous
    
    //Finish configuration and enable
    RCSTAbits.SPEN=1; //enable serial port, other settings default
    TXSTAbits.TXEN=1; //enable transmitter, other settings default
    RCSTAbits.CREN=1; //continuous receive mode
}

char getCharSerial() {
    while(!PIR1bits.RCIF);
    return RCREG;
}

void sendCharSerial(char message) {
    while(!PIR1bits.TXIF);
    TXREG = message;
}

void readUSART(char* buf, int bufSize, char startChar, char endChar) {
    char byteIn = getCharSerial();
    if(byteIn == startChar) {
        char reading = 1;
        int i = 0;
        while(reading) {
            byteIn = getCharSerial();
            if(byteIn == endChar || i == bufSize) {
                reading = 0;
            } else {
                buf[i++] = byteIn;
            }           
        }
    }
    return;
}
