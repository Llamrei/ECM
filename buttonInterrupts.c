#include <xc.h>
#include "buttonInterrupts.h"

// Enables high priority clock interrupt processing on RC3
void initButtonHigh() {
    // Enable interrupts
    RCONbits.IPEN = 1;          // Enable priorities
    INTCONbits.GIEH = 1;        // Global high interrupt enable bit
    
    // Configure  timer 0 interrupts
    T0CONbits.TMR0ON = 1;       // Turn on timer0
    T0CONbits.T016BIT = 1;      // 8 bit mode
    T0CONbits.T0CS = 1;         // Use pin instead of internal clock
    T0CONbits.PSA = 1;          // Disable prescaler
    TMR0L = 0;                  // Start from 0
    
    // Enable RC3 input + interrupt
    INTCONbits.INT0IE = 1;      // Always high priority      
    TRISC |= 0b00001000;
}