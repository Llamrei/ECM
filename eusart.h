/* 
 * File:   eusart.h
 * Author: al3615
 *
 * Created on 14 November 2017, 11:21
 */

#ifndef EUSART_H
#define	EUSART_H

void initEUSART(int baudRate, char error);                      //Configures RC6/7 to take input from EUSART at 9600 baud
char getCharSerial();                   //Retrieves a char from eusart port by polling
void sendCharSerial(char message);      //Sends a char down eusart port 
//Puts any content between a start char and end char inside the buf provided and alters flag passed to let caller know it has updated buffer
void readUSART(char* buf, int bufSize, char startChar, char endChar, char *flag);   

#endif	/* EUSART_H */

