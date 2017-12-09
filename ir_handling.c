#include <xc.h>
#include "ir_handling.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

//TODO: add prescaler as function argument
void initIRCapture(char ICPinNumber, char resetTimerFlag) {
    //Only mess with memory if we are given a valid pin number - 1:3
    if(ICPinNumber > 0 && ICPinNumber < 4) {
       TRISA |= 1 << ICPinNumber + 1;   //Set relevant TRISA register to input mode
       
       // Resetting analogue (removes 'bug')
       ANSEL0 = 0;
       ANSEL1 = 0;
       // T5CON is configured by:
       // T5CON <7-5> - Irrelevant
       // T5CON <4-3> - Prescale bits: 11 (1:8), 10 (1:4), 01 (1:2), 00 (1:1)
       // T5CON <2> - 1 = not synchronized with external clock input
       // T5CON <1> - 1 = clock source from pin, 0 = internal clock source
       // T5CON <0> - 1 = enable Timer5
       T5CON = 0b00011101;
       //Choice dictated by the time unit desired, 
       
       int registerToAddress = 0xF63 - (ICPinNumber-1);
       char* CAPxCON = registerToAddress;
       // CAPxCON is configured by:
       // CAPxCON <7> - Irrelevant
       // CAPxCON <6> - Time base reset enable, 1 for enabled
       // CAPxCON <5:4> - Irrelevant
       // CAPxCON <3:0> - Various modes, 0110  - falling to rising, 0111 rising to falling, 0101 period measurement
       if(resetTimerFlag){
           *CAPxCON = 0b01000110;
       } else {
           *CAPxCON = 0b00000110;
       }
    } 
}

int readIRCapture(char ICPinNumber, char* updateFlag, char* errorFlag) {
   
   //Only mess with memory if we are given a valid pin number - 1:3
    if( (ICPinNumber > 0) && (ICPinNumber < 4) ) {
       int registerToAddressH = 0xF69 - (2*(ICPinNumber-1));
       int registerToAddressL = 0xF69 - (2*(ICPinNumber-1) + 1);
       //wait for reading
       //while(! (PIR3 & 0b1 < 1 + ICPinNumber));
       char* CAPxBUFH = registerToAddressH;
       char* CAPxBUFL = registerToAddressL;
       char debouncing = 1;
       int toReturnOld = 0;
       
       //Software debouncing of IR signal
       while(1) {
       toReturnOld = ((int) *CAPxBUFH << 8) + *CAPxBUFL;
            __delay_us(100);
            int delayedDiff = ((int) *CAPxBUFH << 8) + *CAPxBUFL - toReturnOld;
            if(delayedDiff * ((delayedDiff>0) - (delayedDiff<0)) <  100){
                //Valid debounced signal
                break;
            }          
       }
       
       *updateFlag = 1;
       if(toReturnOld > 130000){
           toReturnOld = 0;
           *errorFlag = 1;
       }
       return toReturnOld;
    }
    *errorFlag = 2;
return 0;
}