/* 
 * File:   servoControl.h
 * Author: al3615
 *
 * Created on 13 November 2017, 14:45
 */

#ifndef SERVOCONTROL_H
#define	SERVOCONTROL_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

// Function to get value of Tpwm calculations, T in 100ms, OSC in MHz, scaler 
int getTPWM(int T, int OSC, char scaler) {
    return ((T*10*(OSC))/(4*scaler) - 1);
}

void initPWM() {
    PTCON0 = 0b00001100;    // free running mode, 1:64 prescaler = 32 us
    PTCON1 = 0b10000000;    // enable PWM timer
    PWMCON0 = 0b00101111;   // PWM0/1 enabled, all independent mode
    PWMCON1 = 0x00;         // special features, all 0 (default)
    int PTPER_aim = 624;
    PTPERL = PTPER_aim & 0b0000000011111111; // base PWM period low byte
    PTPERH = PTPER_aim >> 8; // base PWM period high byte
}

void angle2PWM(int angle) {
    int T_aim = ((200*angle)/225 + 130);    // Apply angle range to PWM range
    int PDC0_aim = getPT(T_aim, 8, 64);     //
    //note: the two LSBs in PDC0L have a different function;
    PDC0L = (PDC0_aim & 0x3F) << 2;         // PDC0L bits 7:2 are on period bits 5:0 - take bottom 5 and shift to fit
    PDC0H = (PDC0_aim & 0xFC0) >> 6;        // PDC0H bits 5:0 are on period bits 11:6
}



#endif	/* SERVOCONTROL_H */

