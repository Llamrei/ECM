//Alex Larionov and Lewis Tran

#ifndef IR_READER_H
#define	IR_READER_H

// Extracts data from buffer, returns 11bits high if checksum fails
// will return 0xFF in 13th buffer index if error in decoding or checksum invalid
// need to pass a size 14 char buffer
void readRFID(char* buf, int bufSize, char* errorFlag);         
// Verifies data in buffer through XOR and checksum
void checkRFIDSum(char* buf, int bufSize);     

#endif	/* IR_READER_H */