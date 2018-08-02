/*
 * CS120B_lab3_part1.c
 *
 * Created: 8/2/2018 12:41:54 PM
 * Author : Keilani Conner (kconn006)
 */ 



#include <avr/io.h>

enum States{INIT, s0, led1, led2, release} state;

unsigned char button = 0x00;
unsigned char tmpB = 0x00;

void Tick() {
	
	button = PINA & 0x01; //PA0
	
	switch(state) { //transitions
		
		case INIT: //automatically go to s0
		state = s0;
		break;
		
		case s0:
		if(button) { //if button is pressed, go to led1
			state = led1;
		}
		else if(!button) { //stay
			state = s0;
		}
		break;
		
		case led1: //2
		if (button) { //stay //pressing button again should change it to 1
			state = led1;
		}
		else if (!button) { //stay upon release
			state = release;
		}
		break;
		
		case release:
		if(button) {
			state = led2; //change to 1
		}
		else if (!button) {
			state = release; //stays 2
		}
		break;
		
		case led2: //1
		state = release;
		break;
		
		
		default:
		break;
	}
	
	switch(state) { //actions
		case INIT:
		tmpB = 0x01; //bit 0, initially on
		break;
		
		case s0:
		tmpB = 0x01; //bit1
		break;
		
		case led1:
		tmpB = 0x02;
		break;
		
		case led2:
		tmpB = 0x01;
		
		case release:
		break;
		
		default:
		break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x01; // Configure port B's 8 pins as outputs
	
	tmpB = 0;
	state = INIT;

	while(1) {
		Tick();
		PORTB = tmpB;
	}
	
	return 0;
}

/*PB0 and PB1 each connect to an LED, and PB0's LED is initially on.
Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED,
staying that way after button release. Pressing the button again turns off PB1's LED and
turns on PB0's LED.*/