#include <xc.h>
#include <string.h>
#include "rfid_reader.h"

void readRFID(char* bufIn, int bufSize) {  
    // Check checksum works
    checkRFIDSum(bufIn, sizeof(bufIn));
    
    if(bufIn[13] !=  0xFF){
        // Extract first 10 bytes
        for (char i = 10; i < bufSize; i++) {
            bufIn[i] = '\0';
        }
    } 
}

void checkRFIDSum(char* buf, int bufSize) {
    char xorOutput = 0;
    char translatedASCII[12];
    char valid = 0;
    char failedDecode = 0;
    memset(translatedASCII, 0, 12);
    
    // ASCII 0 is hex 30
    // Convert input hex into ASCII and then XOR
    
    // Convert into local 'ASCII' represented by hex
    for (char i = 0; i< 12; i++) {
        if(buf[i] > 'F') {
           failedDecode = 1;
        } else if(buf[i] >= 'A') {
            translatedASCII[i] = buf[i] - 'A' + 10;
        } else if(buf[i] >= '0') {
            translatedASCII[i] = buf[i] - '0';
        } else {
            failedDecode = 1;
        } 
    }
    
    // Pair pseudo ASCII and XOR
    for (char i = 0; i< 5; i++) {
        xorOutput ^= (translatedASCII[2*i] << 4) + translatedASCII[2*i+1];
    }
    
    // Pseudo ASCII of signal compared against pseudo ASCII checksum
    
    // Combine pseudo ASCII checksum nibbles
    char ASCIIChecksum = (translatedASCII[10] << 4) + (translatedASCII[11] & 0xF);
    
    if(ASCIIChecksum == xorOutput) {
        valid = 1;
    }
    
    if(!valid || failedDecode) {
        buf[13] = 0xFF;
    }
}
