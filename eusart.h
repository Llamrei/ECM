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
void sendCharSerial(char charToSend);
void sendStrSerial(char *string);

#endif	/* EUSART_H */

