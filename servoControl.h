//Alex Larionov and Lewis Tran

#ifndef SERVOCONTROL_H
#define	SERVOCONTROL_H

int getTPWM(int T, int OSC, char scaler);   // Function to get value of Tpwm calculations, T in 100ms, OSC in MHz, scaler 
void initServoPWM();                             //Configure PWM0/1 with not special features and 1:64 prescaler
void angle2PWM(int angle);                  //Converts an angle in degrees into PWM signal for servo

#endif	/* SERVOCONTROL_H */

