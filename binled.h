/* 
 * File:   binled.h
 * Author: al3615
 * Controls 10 bit 
 * Created on 24 October 2017, 10:12
 */

#ifndef BINLED_H
#define	BINLED_H

void LEDconfig() { 
    // Configure relevant registers
    TRISC &= 0b00001111; // Set registers 4-7 to 0 (output)
    TRISD &= 0b00000011; // Set registers 2-7 to 0 (output) 
}

void BinLEDout(int number)
{       
    // For C extract middle 4 and shift them to start from bit position 4 (instead of 2)
    LATC = (number & 0b00111100) << 2;
 
    // For D extract 2 most and least significant bits and shift them
    // Least needs moved to bit position 2 and 3
    // Most needs moved to bit position 4 and 5
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2);
    
    if (number < 1024 && number > 256) //for 10 bit
    {
        LATD |= (number & 0b1100000000) >> 2;
    }
}

#endif	/* BINLED_H */

