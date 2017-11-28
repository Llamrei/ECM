#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "eusart.h"

void initEUSART(int baudRate, char error) {
    //set data direction registers
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
       
    // Calculation of baud rate from dependent on set values
    SPBRG = (8000000/(baudRate)/4) - 1 + error; //set baud rate to 9600 - for interrupts and handles echoing as close to good as we can expect
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

void readUSART(char *buf, int bufSize, char startChar, char endChar, char *flag) {
    char byteIn = getCharSerial();
    sendCharSerial(byteIn);     //Debugging
    memset(buf, 0, bufSize);
    if(byteIn == startChar) {
        char reading = 1;
        int i = 0;
        while(reading) {
            //Will discard 1st byte read - assuming it was right
            byteIn = getCharSerial();
            sendCharSerial(byteIn); //Debugging

            //Stop reading if we see an end char or we fill the buffer
            if(byteIn == endChar || i == bufSize) {
                reading = 0;
            } else {
                buf[i++] = byteIn;
            }           
        }
        *flag = 1;
    }
    return;
}

void sendStrSerial(char *string){
    while(*string != 0){                // Increment until null (0x00) reached
        while(!PIR1bits.TXIF);
        TXREG = *string++;        // Send each character as data
    }
}
