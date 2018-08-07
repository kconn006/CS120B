/*
 * kmccu003_lab4_part3.c
 *
 * Created: 8/6/2018 12:16:27 PM
 * Author : Kiana & Keilani 
 */ 

#include <avr/io.h>

enum States{START, INIT, WAIT, ODD, EVEN, PRESS, RELEASE, PRESS_2, RELEASE_2} state; //5 4 and A A

unsigned char button = 0x00;
unsigned char cnt = 0x00; //button presses
unsigned char tmpC;

void Tick() {
	
	button = PINA & 0x01; //input PA0

	switch(state) { //transitions
		
		case START: 
			state = INIT;
			break;
		
		case INIT:
			
			if(button) { //button is pressed
				state = PRESS;
			}

			else if(!button){ //stay
				state = INIT;
			}
			break;
			
			case PRESS:
				if(button) {
					state = PRESS;
				}
				else if(!button) {
					state = RELEASE;
				}
				
			case RELEASE:
				state = EVEN;
			break;
				
			case ODD:
			if(button) {
				state = PRESS;
			}
			
			else if(!button) {
				state = ODD;
			}
			
			case PRESS_2:
			if(!button) {
				state = RELEASE_2;
			}
			else if(button) {
				state = PRESS_2;
			}
			break;
			
			case RELEASE_2:
				state = ODD;
			break;
			
			case EVEN:
			if(!button) {
				state = EVEN;
			}
			
			else if(button) {
				state = PRESS_2;
			}
		
			default:
				break;
			
			
	} //end transitions
	
	
	switch(state) { //actions
			
		case INIT:
			break;
			
		case PRESS:
			break;
			
		case ODD:
			tmpC = 0xAA;
			break;
		
		case EVEN:
			tmpC = 0x55;
			break;
		
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	
	state = INIT;

	while(1) {
		Tick();
		PORTC = tmpC;0
	}
	
	return 0;
}

/*(Challenge) Create your own festive lights display with 6 LEDs connected to port PB5..PB0,
lighting in some attractive sequence. Pressing the button on PA0 changes the lights to the next configuration
in the sequence.  Use a state machine (not synchronous) captured in C.*/