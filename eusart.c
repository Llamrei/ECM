#include <xc.h>
#include "eusart.h"

void initEUSART() {
    //set data direction registers
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
       
    // Calculation of baud rate from dependent on set values
    SPBRG=97; //set baud rate to 19200 - for interrupts and handles echoing
    SPBRGH=0; 
    
    // Basic config
    BAUDCONbits.BRG16=1; //set baud rate scaling to 16 bit mode
    TXSTAbits.BRGH=1; //high baud rate select bit
    TXSTAbits.SYNC = 0; //Set to asynchronous
    
    //Finish configuration and enable
    RCSTAbits.SPEN=1; //enable serial port, other settings default
    PIE1bits.RCIE =1; // enable receive interrupts
    TXSTAbits.TXEN=1; //enable transmitter, other settings default
    RCSTAbits.CREN=1; //continuous receive mode
}

char getCharSerial() {
    while(!PIR1bits.RCIF);
    return RCREG;
}

void sendCharSerial(char charToSend) {
    while(!PIR1bits.TXIF);      //While waiting for transmit register to clear - set to 1
    TXREG = charToSend;         //Once clear - sned char
}
