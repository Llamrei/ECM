//Alex Larionov and Lewis Tran

#ifndef BINLED_H
#define	BINLED_H

void initLED();               // Set TRIS registers D2-7 and C4-7 to output
void sendLED(int number);     // Sends out a 10 bit number to 10bit LED array on D2-7 C4-7

#endif	/* BINLED_H */

