#include "navigation.h"
#include <xc.h>

void initTimers() {
    //Configure timers
    
    //Timer 1:
    //<7> 16 bit mode
    //<6> device clock not dependent on TMR1
    //<5-4> prescaler 1:1
    //<3> oscillator enable bit
    //<2> external clock sync on
    //<1> select external clock as source
    //<0> enable bit
    T1CON = 0b10000011;
    
    //Timer 0:
    //<7> enable
    //<6> 16bit
    //<5> external clock
    //<4> low to high
    //<3> no prescaler
    //<2-0> prescaler value (not relevant)
    T0CON = 0b10101000;
}

int readTimer1() {
    return ((int) TMR1H << 8) + TMR1L;    
}

int readTimer0() {
    return ((int) TMR0H << 8) + TMR0L;   
}