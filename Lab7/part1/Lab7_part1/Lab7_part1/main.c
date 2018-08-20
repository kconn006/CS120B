/*
 * Lab7_part1.c
 *
 * Created: 8/13/2018 12:25:36 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() { //timer/counter controller reg
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



unsigned char tempB = 0x00;
unsigned char temp = 0x00;
unsigned char cnt = 0x00;
unsigned char threeLEDs;
unsigned char blinkingLED;

enum BL_States { BL_SMStart, BL_LEDOff, BL_LEDOn } bl_state;
void BlinkLED_Tick(){
	
	switch(bl_state){
	//Transitions
		case BL_SMStart:
			bl_state = BL_LEDOff;
			break;
		case BL_LEDOff:
			bl_state = BL_LEDOn;
			break;
		case BL_LEDOn:
			bl_state = BL_LEDOff;
		default:
		break;
	}
	
	
	switch(bl_state){
	//State Actions
		case BL_SMStart:
		break;
		case BL_LEDOff:
			tempB = 0x00;
			break;
		case BL_LEDOn:
			tempB = 0x01;
			break;
		default:
		break;
	}
	blinkingLED = tempB;
}

enum TL_States { TL_SMStart, TL_Seq0, TL_Seq1, TL_Seq2 } tl_state;
void ThreeLED_Tick(){
	
	switch(tl_state){
	//Transitions 
		case TL_SMStart:
			tl_state = TL_Seq0;
		break;
		case TL_Seq0:
			tl_state = TL_Seq1;
		break;
		case TL_Seq1:
			tl_state = TL_Seq2;
		break;
		case TL_Seq2:
			tl_state = TL_Seq0;
		break;
		default:
		break;
	}
	
	switch(tl_state){
		//State actions
		case TL_SMStart:
		break;
		case TL_Seq0:
			tempB = 0x02;
		break;
		case TL_Seq1:
		tempB = 0x04;
		break;
		case TL_Seq2:
		tempB = 0x08;
		break;
		default:
		break;

	}
	//if (cnt <= 3)
	//{
		threeLEDs = tempB;
		//cnt+=1;
	//}
	/*//S*if (cnt==4)
	{
		cnt = 0;
	*/	
}

enum CM_STATES{ CM_SMStart, CM_ON } cm_state;
void COMBO_Tick(){
  
  
  switch(cm_state){
	  case CM_SMStart:
		cm_state = CM_ON;
		break;
	case CM_ON:
		cm_state = CM_ON;
		break;
	default:
	break;
  }
  
  switch(cm_state){
	  case CM_SMStart:
	  break;
	  case CM_ON:
		BlinkLED_Tick();
		ThreeLED_Tick();
		tempB= threeLEDs | blinkingLED;
		{
		  PORTB = tempB;
		 }
	  break;
	 }
}


int main(void) {
	DDRB = 0xFF; //set port B to output
	PORTB = 0x00; //init port B to 0's
	TimerSet(1000);
	TimerOn();
	cm_state = CM_SMStart;
	while(1) {
		COMBO_Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
	
}
