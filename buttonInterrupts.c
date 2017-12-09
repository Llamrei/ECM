#include <xc.h>
#include "buttonInterrupts.h"

// Enables high priority clock interrupt processing on RC3
void initButton() {
    // Enable interrupts
    INTCONbits.GIE = 1;        // Global high interrupt enable bit
        
    // Enable RC3 input + interrupt
    INTCON3bits.INT1E = 1;      // Always high priority      
    TRISC |= 0b00010000;
}