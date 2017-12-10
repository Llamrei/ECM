//Alex Larionov and Lewis Tran

#ifndef IR_HANDLING_H
#define	IR_HANDLING_H

#define rightIR 1
#define leftIR 3

#define resetEnable 1
#define resetDisable 0

#define debounceTol 100

void initIRCapture(char ICPinNumber, char resetTimerFlag);
int readIRCapture(char ICPinNumber, char* updateFlag, char* errorFlag);


#endif	/* IR_HANDLING_H */
