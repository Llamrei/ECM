#include <xc.h>
#include <string.h>
#include "ir_reader.h"

void readRFID(char* bufIn, int bufSize) {  
//    Check checksum works
    checkRFIDSum(bufIn, sizeof(bufIn));
    
    if(bufIn[13] !=  0xFF){
        // Extract first 10 bytes
        for (char i = 10; i < bufSize; i++) {
            bufIn[i] = '\0';
        }
    } 
}
void checkRFIDSum(char* buf, int bufSize) {
    char xorOutput;
    char valid = 0;
    memset(xorOutput, 0, 2);
    
    //ASCII 0 is hex 30
    //Convert input hex into ASCII and then xor
    
    for(char i = 0; i < 5; i++){
        xorOutput ^= buf[2*i] << 4 + buf[2*i + 1];
    }
    xorOutput += 30;
    //Compare to checksum sent - MSBits
    if(buf[10] + == xorOutput[0]) {
        //LSBits
        if(buf[11] == xorOutput[1]){
            //Valid checksum do nothing
            valid = 1;
        }
    }    
    
    if(!valid) {
            buf[13] = 0xFF;
    }
}
