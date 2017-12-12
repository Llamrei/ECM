//Alex Larionov and Lewis Tran
#include <xc.h>
#include "binled.h"

// Set TRIS registers D2-7 and C4-7 to output
void initLED() { 
    TRISC &= 0b00001111; // Set registers 4-7 to 0 (output)
    TRISD &= 0b00000011; // Set registers 2-7 to 0 (output) 
}

// Sends out a 10 bit number to 10bit LED array on D2-7 C4-7
void sendLED(int number)
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
