/* 
 * File:   eusart.h
 * Author: al3615
 *
 * Created on 14 November 2017, 11:21
 */

#ifndef EUSART_H
#define	EUSART_H

void initEUSART();
char getCharSerial();
void sendCharSerial(char message);
void readUSART(char* buf, int bufSize, char startChar, char endChar, char *flag);

#endif	/* EUSART_H */

