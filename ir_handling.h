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

#define falling 0
#define rising 1

#define resetEnable 1
#define resetDisable 0

char readFlag1 = 0, readFlag2 = 0;
int time1 = 0, time2 = 0;

void initIRCapture();
void setEdgeCapture(char CCPselect, char edgeType);
int readIRCapture(char CCPselect, char* updateFlag);


#endif	/* IR_HANDLING_H */

