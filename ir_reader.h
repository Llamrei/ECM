/* 
 * File:   ir_reader.h
 * Author: lt2415
 *
 * Created on 28 November 2017, 10:42
 */

#ifndef IR_READER_H
#define	IR_READER_H

void readRFID(char* buf, int bufSize);         // Extracts data from buffer, returns 11bits high if checksum fails
void checkRFIDSum(char* buf, int bufSize);     // Verifies data in buffer through XOR and checksum

#endif	/* IR_READER_H */