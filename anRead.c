//Alex Larionov and Lewis Tran

#include <xc.h>
#include "anRead.h"

// Sets TRIS and ADCON registers needed to use ADC on RA3
void initADC(){
  //Set up ADC
   TRISAbits.RA3 = 1;      //LDR pin to input
   ANSEL0bits.ANS3 = 1;    //Set pin 3 to take analog input
   ADCON0=0b00001101;      //Single shot, ADC port channel 3, enable ADC
   ADCON1=0b00000000;      //Use internal voltage reference
   ADCON2=0b10101011;      //Right justify result and timing settings
}

// Read RA3 voltage and convert to 10 bit value, returns int
int readADC(){
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    return ADRESL + ((unsigned int) ADRESH << 8);
}
