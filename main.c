#include <xc.h>

#pragma config OSC = IRCIO              // Set internal clock

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

//Prototype definition
void initPWM(void); // function to setup PWM
void angle2PWM(int angle); // output correct PWM output for given angle
void delay_s(char seconds); // function to delay in seconds
int getPT(int T, int OSC, char scaler);
//__delay_ms() is limited to a maximum delay of 89ms with an 8Mhz
//clock so you need to write a function to make longer delays

void main(void){    
    OSCCON = 0x72;
   // while(!OSCCONbits.IOFS);                // Wait for OSCON to become stable
    
    initPWM(); //setup PWM registers
    while(1){
        angle2PWM(-90); //set servo to -90 deg
        delay_s(1);//wait for 1s
        angle2PWM(90); //set servo to 90 deg
        delay_s(1);//wait for 1s
    }
}

void initPWM() {
    PTCON0 = 0b00001100; // free running mode, 1:64 prescaler = 32 us
    PTCON1 = 0b10000000; // enable PWM timer
    PWMCON0 = 0b00101111; // PWM0/1 enabled, all independent mode
    PWMCON1 = 0x00; // special features, all 0 (default)
    int PTPER_aim = 624;
    PTPERL = PTPER_aim & 0b0000000011111111; // base PWM period low byte
    PTPERH = PTPER_aim >> 8; // base PWM period high byte
}

void angle2PWM(int angle) {
    int T_aim = ((200*angle)/225 + 130); //This doesnt work
    int PDC0_aim = getPT(T_aim, 8, 64);  //
    //note: the two LSBs in PDC0L have a different function;
    PDC0L = (PDC0_aim & 0x3F) << 2; // PDC0L bits 7:2 are on period bits 5:0 - take bottom 5 and shift to fit
    PDC0H = (PDC0_aim & 0xFC0) >> 6; // PDC0H bits 5:0 are on period bits 11:6
}

// Function to get value of Tpwm calculations, T in 100ms, OSC in MHz, scaler 
int getPT(int T, int OSC, char scaler) {
    return ((T*10*(OSC))/(4*scaler) - 1);
}

void delay_s(char seconds) {
    for (int i = 0; i < (seconds*1000)/50; i++) {
        __delay_ms(50);
    }
}