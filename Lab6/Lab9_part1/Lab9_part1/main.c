/*
 * Lab9_part1.c
 *
 * Created: 8/19/2018 10:11:36 PM
 * Author : Keilani
 */ 

#include <avr/io.h>
// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


typedef unsigned char uc;
typedef double db;

enum Speaker_States{SP_SMSTART, INIT, C4_FREQ, D4_FREQ, E4_FREQ} sp_state;
void Speaker_Tick(){
	db C4 = 261.63; // f = 261.63
	db D4 = 293.66; // f = 293.66
	db E4 = 329.63; // f = 329.63
	unsigned char button0; // A0
	unsigned char button1; // A1
	unsigned char button2; // A2
	button0 = PINA & 0x01; // A0
	button1 = PINA & 0x02; // A1
	button2 = PINA & 0x04; // A2

	uc button_check0=0;
	uc button_check1=0;
	uc button_check2=0;

	if ( (button0&&!button1&&!button2))
	{
		button_check0 = 1;
	}

	if ( (!button0&&button1&&!button2))
	{
		button_check1 = 1;
	}

	if ( (!button0&&!button1&&button2))
	{
		button_check2 = 1;
	}
	
	switch(sp_state){
		case SP_SMSTART:
		sp_state = INIT;
		break;
		case INIT:
			if (button_check0)
			{
				sp_state = C4_FREQ;
				break;
			}
			if (button_check1)
			{
				sp_state = D4_FREQ;
				break;
			}
			if (button_check2)
			{
				sp_state = E4_FREQ;
				break;
			}
			sp_state = INIT;
			break;
		
		case C4_FREQ:
			if (button_check0)
			{
				sp_state = C4_FREQ;
				break;
			}
			else {sp_state = INIT;}
		break;
		case  D4_FREQ:
			if (button_check0)
			{
				sp_state = D4_FREQ;
				break;
			}
			else {sp_state = INIT;}
			break;
		case  E4_FREQ:
			if (button_check0)
			{
				sp_state = E4_FREQ;
				break;
			}
			else {sp_state = INIT;}
		break;
		default:
		break;
	}
	
	switch(sp_state){
		//State Actions
		case SP_SMSTART:
		break;
		case INIT:
			set_PWM(0);
			PWM_on();
		break;
		case C4_FREQ:
			set_PWM(C4);
			PWM_on();
		break;
			PWM_on();
		break;
		case  E4_FREQ:
			set_PWM(E4);
			PWM_on();
		break;
		default:
		break;
	}

	
};
int main(void)
{
	DDRB = 0xFF; //set port B to output
	PORTB = 0x00; //init port B to 0's
	DDRA = 0x00;
	PORTA = 0xFF; // Configure port A's 8 pins as inputs
		sp_state = SP_SMSTART;

	/* Replace with your application code */
	while (1)
	{
		Speaker_Tick();
	}
}
