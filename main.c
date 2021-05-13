/*	Author: Dylan Riffel
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;


void TimerOn(){
TCCR1B = 0x0B;
OCR1A = 125;
TIMSK1 = 0x02;
TCNT1 = 0;
_avr_timer_cntcurr = _avr_timer_M;
SREG |= 0x80;

}	

void TimerOff(){
TCCR1B = 0x00;
}	
void TimerISR(){
TimerFlag = 1;
}	
ISR(TIMER1_COMPA_vect){

    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0){
	TimerISR();
	_avr_timer_cntcurr = _avr_timer_M;
    
    }	    
}



void TimerSet(unsigned long M){
       _avr_timer_M = M;
       _avr_timer_cntcurr = _avr_timer_M;
}

unsigned int count = 0;
unsigned int count1 = 0;


enum SM_STATES { SM_Start, SM_Middle,SM_End} SM_STATE;

void Tick(){
switch(SM_STATE){
	case SM_Start:
	if( count == 1000){
	count = 0;	
	SM_STATE = SM_Middle;
	}
	else {
	SM_STATE = SM_Start;
	}
        break;

        case SM_Middle:
        if( count == 1000){
        count = 0;
        SM_STATE = SM_End;
        }
        else {
	SM_STATE = SM_Middle;
        }
        break;

        case SM_End:
        if( count == 1000){
        count = 0;
        SM_STATE = SM_Start;
        }
        else {
	SM_STATE = SM_End;

        }
        break;


}
switch(SM_STATE){
	case SM_Start:
	PORTB = 0b001;
	break;

	case SM_Middle:
	PORTB = 0b010;
	break;

	case SM_End:
	PORTB = 0b100;
	break;
}	
}


enum S_STATES { S_Off, S_On} S_STATE;

void On(){
switch(S_STATE){

	case S_Off:
	if( count1 == 1000){
        count1 = 0;
        S_STATE = S_On;
        }
        else{
	S_STATE = S_Off;
        }
        break;

        case S_On:
        if( count1 == 1000){
        count1 = 0;
        S_STATE = S_Off;
        }
        else {
	S_STATE = S_On;
        }
        break;

	
}
switch(S_STATE){
	case S_Off:
	PORTB = PORTB & 0b0111;
	break;
	case S_On:
	PORTB = PORTB | 0b1000;
	break;

}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; 
    PORTA = 0xFF; DDRB = 0xFF; 
    DDRD = 0xFF;  PORTD = 0x00;
    
    TimerSet(1);
    TimerOn();

    SM_STATE = SM_Start;
    S_STATE = S_Off;
    /* Insert your solution below */
    while (1) {
    Tick();
    On();
    ++count;
    ++count1;

    while(!TimerFlag);
    TimerFlag = 0;
    }
    
}
