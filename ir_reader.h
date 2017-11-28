/* 
 * File:   ir_reader.h
 * Author: lt2415
 *
 * Created on 28 November 2017, 10:42
 */

#ifndef IR_READER_H
#define	IR_READER_H

// Extracts data from buffer, returns 11bits high if checksum fails
// will return 0xFF in 13th buffer index if error in decoding or checksum invalid
void readRFID(char* buf, int bufSize);         
// Verifies data in buffer through XOR and checksum
void checkRFIDSum(char* buf, int bufSize);     

#endif	/* IR_READER_H */