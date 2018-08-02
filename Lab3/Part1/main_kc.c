/*
 * CS120B_lab3_part3_kc.c
 *
 * Created: 8/2/2018 12:49:41 PM
 * Author : ucrcse
 */ 
#include <avr/io.h>

enum States{INIT, led0, led1} state;
unsigned char button = 0x00;
unsigned char tempB = 0x00;

void Tick() {
	
	button = PINA & 0x01; //PA0
	
	switch(state) 
	{ //transitions
		case INIT: //automatically go to led0
			state = led0;
			break;
		case led0:
			state = button ? led1 : led0;
			break;
		case led1:
			state = button ? led0 : led1;
			break;
		default:
			break;
	}

	switch(state)
	{//State actions 
		case INIT:
			break;
		case led0:
			tempB = 0x01;
			break;
		case led1:
			tempB = 0x02;
			break;
		default:
			break;
				
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x01; // Configure port B's 8 pins as outputs
	

	tempB = 0;
	state = INIT;

	while(1) {
		Tick();
		PORTB = tempB;
	}
	
	return 0;
}