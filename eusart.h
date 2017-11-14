/* 
 * File:   eusart.h
 * Author: al3615
 *
 * Created on 14 November 2017, 11:21
 */

#ifndef EUSART_H
#define	EUSART_H

void initEUSART();                      //Configures RC6/7 to take input from EUSART at 9600 baud
char getCharSerial();                   //Retrieves a char from eusart port by polling
void sendCharSerial(char message);      //Sends a char down eusart port 
void readUSART(char* buf, int bufSize, char startChar, char endChar, char *flag);

#endif	/* EUSART_H */

