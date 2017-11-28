/* 
 * File:   ldr.h
 * Author: lt2415
 *
 * Created on 31 October 2017, 16:16
 */
#include <xc.h>

#ifndef LDR_H
#define	LDR_H

void initADC();         // Sets TRIS and ADCON registers needed to use ADC on RA3
int readADC();          // Read RA3 voltage and convert to 10 bit value, returns int

#endif	/* LDR_H */

