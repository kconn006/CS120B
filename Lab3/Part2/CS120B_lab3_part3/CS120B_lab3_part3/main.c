/*
 * CS120B_lab3_part3_kc.c
 *
 * Created: 8/2/2018 12:49:41 PM
 * Author : Keilani Conner (kconn006)
 */ 
#include <avr/io.h>

enum States{INIT, output7, button_pressed, button_released, reset, decrement, increment} state;
unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
unsigned char tempC = 0x00;

void Tick() {
	
	button0 = PINA & 0x01; //PA0
	button1 = PINA & 0x02; //PA1
	
	switch(state) 
	{ //transitions
		case INIT: //automatically go to output7
			state = output7;
			break;
		case output7:
			if (button0 && button1) {
				state = reset;
				break;
			}
			if (button0)
			{
				state = increment;
				break;
			}
			if (button1)
			{
				state = decrement;
				break;
			}
		case reset:
			state = INIT;
			break;
		case increment:
			if (button0 && button1) 
				{
					state = reset;
					break;
				}
			if (button0 && !button1)
			{
				if (tempC < 9)
				{
					state = increment;
					break;
				}	
			}
			if (!button0 && button1)
			{
				state = decrement;
				break;
			}
			state = INIT;
			break;
		case decrement:
			if (button0 && button1) 
				{
					state = reset;
					break;
				}
			if (button1 && !button0)
				{
					if (tempC > 0)
					{
						state = decrement;
						break;
					}
					
				}
			if (!button1 && button0)
			{
				state = increment;
			}
			state = INIT;
			break;
		default:
			break;
	}

	switch(state)
	{//State actions 
		case INIT:
			break;
		case output7:
			tempC = 0x07;
			break;
		case reset:
			tempC = 0x00;
			break;
		case increment:
			if (button0)
				{
					tempC = tempC+1;
				}
			break;
		case decrement:
			if (button1)
				{
					tempC = tempC-1;
				}
			break;
			
		default:
			break;
				
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x01; // Configure port C's 8 pins as outputs
	

	tempC = 0;
	state = INIT;

	while(1) {
		Tick();
		PORTC = tempC;
	}
	
	return 0;
}
/*Buttons are connected to PA0 and PA1. 
Output for PORTC is initially 7.
Pressing PA0 increments PORTC once (stopping at 9).
Pressing PA1 decrements PORTC once (stopping at 0).
If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
*/