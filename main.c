#include <xc.h>
#include <string.h>
#include <stdio.h>

#include "anRead.h"
#include "lcd.h"

#pragma config OSC = IRCIO              // Set internal clock

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000              // Set 8MHz clock for delay routines
#endif

//Prototype definition
void delay(){
    for(int i = 0; i< 5; i++) {
        __delay_ms(50);
    }
}

void MoveWithBall();
void Shoot();
void Goal();
void Celebrate();
//Character
char head = 0b01101111;
char standing_body = 0b01111100;
char running_body = 0b11000100;
char kicking_body = 0b10110101;

//Ball
char ground = 0b10100001;
char mid_air = 0b11011111;

//Goal
char goalpost = 0b01111100;

//Display
char topline[16] = "";
char bottomline[16] = "";

//Celebrate
char firework = 0b11101011;
char star = 0b00101010;

void main(void){
    OSCCON = 0x72;
    while(!OSCCONbits.IOFS);                // Wait for OSCON to become stable

    LCD_Init();
    SetLine(1);
    
    
    
    LCD_String("    Football time!!");
    
    for(int i = 0; i < 16; i ++) {
        SendLCD(0b00011000, command);           //Scroll intro
        delay();
    }
    
    //Iterate animation
    while(1){
        MoveWithBall();
        Shoot();
        Goal();
        Celebrate();
    }
        
        
        
}


void FillCanvas() {
    //Fill canvas 
    for(int k = 0; k < 16; k++){
        topline[k] = ' ';
        bottomline[k] = ' ';
    }
}

void Celebrate() {
    SendLCD(0b00000001, command);       //Clear display
    __delay_ms(2);                      //Wait for clear
    FillCanvas(); 
    SetLine(1);
    LCD_String("GOOOOOOOOAAAAAAAL!");
    SetLine(2);
    for(int i = 0; i < 16; i++){
        if(i%2==0){
            bottomline[i] = firework;
        } else {
            bottomline[i] = star;
        }
    }
    LCD_String(bottomline);
    for(int i = 0; i < 16; i ++) {
        SendLCD(0b00011000, command);           //Scroll intro
        delay();
    }
}

void Goal() {
    for (int i = 0; i < 5; i++) {
            //Per frame
        SendLCD(0b00000001, command);       //Clear display
        __delay_ms(2);                      //Wait for clear

        //Fill canvas 
        FillCanvas();

        //Set the frame
        topline[8] = head;
        bottomline[8] = standing_body;
        switch(i) {
            case 0:
                topline[10] = ground;
                break;
            case 1:
                topline[11] = mid_air;
                break;
            case 2:
                topline[12] = mid_air;
                break;
            case 3:
                topline[13] = mid_air;
                break;
            case 4:
                topline[14] = ground;
                break;
        }
        topline[15] = goalpost;
        bottomline[15] = goalpost;

        //Draw top line
        SetLine(1);
        LCD_String(topline);

        //Draw bottom line
        SetLine(2);
        LCD_String(bottomline);

        //Wait to see the frame
        delay();
    }
}

void Shoot() {   
    for (int i = 0; i < 3; i++) {
        //Per frame
    SendLCD(0b00000001, command);       //Clear display
    __delay_ms(2);                      //Wait for clear
            
    //Fill canvas 
    FillCanvas();
            
    //Set the frame
    topline[8] = head;
    switch(i) {
        case 0:
            bottomline[8] = standing_body;
            bottomline[9] = ground;
            break;
        case 1:
            bottomline[8] = kicking_body;
            bottomline[9] = ground;
            break;
        case 2:
            bottomline[8] = running_body;
            bottomline[9] = mid_air;
            break;
    }
    topline[15] = goalpost;
    bottomline[15] = goalpost;

    //Draw top line
    SetLine(1);
    LCD_String(topline);

    //Draw bottom line
    SetLine(2);
    LCD_String(bottomline);

    //Wait to see the frame
    delay();
    }
}

void MoveWithBall() {

    // --- Move with ball
        
        for(int i = 0; i < 8; i++ ){
            //Per frame
            SendLCD(0b00000001, command);       //Clear display
            __delay_ms(2);                      //Wait for clear
            
            //Fill canvas 
            FillCanvas();
            
            //Set the frame
            topline[i] = head;
            if(i%2 == 0){
                bottomline[i] = standing_body;
                bottomline[i+1] = mid_air;
            } else {
                bottomline[i] = running_body;
                bottomline[i+1] = ground;
            }
            
            topline[15] = goalpost;
            bottomline[15] = goalpost;

            //Draw top line
            SetLine(1);
            LCD_String(topline);

            //Draw bottom line
            SetLine(2);
            LCD_String(bottomline);

            //Wait to see the frame
            delay();
        }
        
        
        
    }