/*
 * LED_Mat_part1.c
 *
 * Created: 8/23/2018 10:53:09 PM
 * Author : Keilani
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

// ====================
// SM1: DEMO LED matrix
// ====================
enum SM1_States {sm1_display};
int SM1_Tick(int state) {

	// === Local Variables ===
	static unsigned char column_val = 0x01; // sets the pattern displayed on columns
	static unsigned char column_sel = 0x7F; // grounds column to display pattern
	
	// === Transitions ===
	switch (state) {
		case sm1_display:    break;
		default:   	        state = sm1_display;
		break;
	}
	
	// === Actions ===
	switch (state) {
		case sm1_display:   // If illuminated LED in bottom right corner
		if (column_sel == 0xFE && column_val == 0x80) {
			column_sel = 0x7F; // display far left column
			column_val = 0x01; // pattern illuminates top row
		}
		// else if far right column was last to display (grounded)
		else if (column_sel == 0xFE) {
			column_sel = 0x7F; // resets display column to far left column
			column_val = column_val << 1; // shift down illuminated LED one row
		}
		// else Shift displayed column one to the right
		else {
			column_sel = (column_sel >> 1) | 0x80;
		}
		break;
		default:   	        break;
	}
	
	PORTA = column_val; // PORTA displays column pattern
	PORTB = column_sel; // PORTB selects column to display pattern

	return state;
};

int main(void)
{
	/* Replace with your application code */
	DDRA = 0xFF; //set port A to output
	PORTA = 0x00; //init port A to 0's
	DDRB = 0xFF; //set port B to output
	PORTB = 0x00; //init port B to 0's
	TimerSet(500);
	TimerOn();
	//unsigned char tmpB = 0x00;
	
	
	while (1)
	{

		
		SM1_Tick(sm1_display);
		
		while(!TimerFlag); //wait 1 sec
		TimerFlag = 0;
	}
}
