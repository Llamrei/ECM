/* 
 * File:   ldr.h
 * Author: lt2415
 *
 * Created on 31 October 2017, 16:16
 */
#include <xc.h>

#ifndef LDR_H
#define	LDR_H

void ADC_Init(){
  //Set up ADC
   TRISAbits.RA3 = 1;      //LDR pin to input
   ANSEL0bits.ANS3 = 1;    //Set pin 3 to take analog input
   ADCON0=0b00001101;      //Single shot, ADC port channel 3, enable ADC
   ADCON1=0b00000000;      //Use internal voltage reference
   ADCON2=0b10101011;      //Right justify result and timing settings
}
 
int ADC_Read(){
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    return ADRESL + ((unsigned int) ADRESH << 8);
}

#endif	/* LDR_H */

