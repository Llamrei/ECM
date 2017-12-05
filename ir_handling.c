#include <xc.h>
#include "ir_handling.h"

//TODO: add prescaler as function argument
void initIRCapture() {
       TRISC |= 1 << 1;   //Set relevant TRISC register to input mode
       TRISC |= 1 << 2;
       INTCONbits.GIE = 1;
       PIE1bits.CCP1IE = 1;
       PIE2bits.CCP2IE = 1;
               
       // T5CON is configured by:
       //See datasheet, configured to 2 8 bit operations and 1:8 prescaler with internal clock source
       T1CON = 0b01111001;
       //Prescaler choice dictated by the time unit desired, timer cycle = prescaler/(Fosc/4) 
       setEdgeCapture(1, falling);
       setEdgeCapture(2, falling);
}

void setEdgeCapture(char CCPselect, char edgeType) {
    if(CCPselect > 0 && CCPselect < 3) {       
       //Changing CCPxCon sets capture interrupt flag so we want to disable during change
       PIE1bits.CCP1IE = 0;
       PIE2bits.CCP2IE = 0;
       int registerToAddress = 0xFBD - (CCPselect-1)*3;
       char* CCPxCON = registerToAddress;
       if(edgeType = falling) {
           CCPxCON = 0b00000100;
       } else if (edgeType = rising) {
           CCPxCON = 0b00000101;
       } else {
           //Default to falling
           CCPxCON = 0b00000100;
       }
       //re-enable after changing mode and clear any changes made
       PIR1bits.CCP1IF = 0;
       PIR2bits.CCP2IF = 0;
       PIE1bits.CCP1IE = 1;
       PIE2bits.CCP2IE = 1;
    } 
}

int readIRCapture(char CCPselect, char* updateFlag) {
   
   //Only mess with memory if we are given a valid pin number - 1:3
    if(CCPselect > 0 && CCPselect < 3) {       
        if(CCPselect == 1) {
           while(!readFlag1);
           readFlag1 = 0;
           return time1;
        } else if (CCPselect == 2) {
           while(!readFlag2);
           readFlag2 = 0;
           return time2;
        }
    }
    return -7;
}