#include <xc.h>
#include "ir_handling.h"

void initIRCapture(char ICPinNumber, char resetTimerFlag) {
    //Only mess with memory if we are given a valid pin number - 1:3
    if(ICPinNumber > 0 && ICPinNumber < 4) {
       TRISA |= 1 << ICPinNumber + 1;   //Set relevant TRISA register to input mode
       int registerToAddress = 0xF63 - (ICPinNumber-1);
       char* CAPxCON = registerToAddress;
       // CAPxCON is configured by 
       // CAPxCON <7> - Irrelevant
       // CAPxCON <6> - Time base reset enable, 1 for enabled
       // CAPxCON <5:4> - Irrelevant
       // CAPxCON <3:0> - Various modes, 0110  - falling to rising, 0111 rising to falling, 0101 period measurement
       if(resetTimerFlag){
           *CAPxCON = 0b01000101;
       } else {
           *CAPxCON = 0b00000101;
       }
    } 
}

unsigned int readIRCapture(char ICPinNumber, char* updateFlag) {
   
   //Only mess with memory if we are given a valid pin number - 1:3
    if( (ICPinNumber > 0) && (ICPinNumber < 4) ) {
       int registerToAddressH = 0xF69 - (2*(ICPinNumber-1));
       int registerToAddressL = 0xF69 - (2*(ICPinNumber-1) + 1);
       char* CAPxBUFH = registerToAddressH;
       char* CAPxBUFL = registerToAddressL;
       unsigned int toReturn = ((unsigned int) *CAPxBUFH << 8) + *CAPxBUFL; //TODO: Done in 2 lines to help debug - change later
       *updateFlag = 1;
       return toReturn;
    }
    
    return -7;
}