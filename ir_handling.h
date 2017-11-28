/* 
 * File:   ir_handling.h
 * Author: lt2415
 *
 * Created on 28 November 2017, 18:30
 */

#ifndef IR_HANDLING_H
#define	IR_HANDLING_H

#define rightIR 1
#define leftIR 3

#define resetEnable 1
#define resetDisable 0

void initIRCapture(char ICPinNumber, char resetTimerFlag);
unsigned int readIRCapture(char ICPinNumber, char* updateFlag);


#endif	/* IR_HANDLING_H */

