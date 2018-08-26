/*
 * Lab_LED_part2.c
 *
 * Created: 8/24/2018 10:19:10 AM
 * Author : Keilani
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
typedef unsigned char uc; 
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

/*enum MoveBall_States{MB_START, MB_STAY_TOP, MB_SHIFT_UP, MB_SHIFT_DOWN, MB_STAY_DOWN} mb_state;
	uc up_button= ~PINC & 0x01; // up = C0
	uc down_button= ~PINC & 0x02; // down = C1
	uc top = 0; // Top of matrix = 0
	uc bottom = 7; // Bottom of matrix = 7
	uc current_row = 0;

void MoveBall_Tick(){
	//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*A0 0 |  |  |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |  |  |
	  A2 0 |  |  |  |  |  |  |  |  |  |  |
	  A3 0 |  |  |  |  |  |  |  |  |  |  |
	  A4 0 |  |  |  |  |  |  |  |  |  |  |
	  A5 0 |  |  |  |  |  |  |  |  |  |  |
	  A6 0 |  |  |  |  |  |  |  |  |  |  |
	  A7 0 |  |  |  |  |  |  |  |  |  |  |
  			0  0  0  0  0  0  0  0  0  0
		   // B0  B1  B2  B3  B4  B5  B6  B7 
	//Start with ball in Left Top corner
		// === Local Variables ===
		static uc turn_on = 0x01; // sets the pattern displayed on columns
		static uc column_num = 0x00; // grounds column to display pattern
		//Remember Columns have to be 0 first in order to turn on
		//Row value (turn_on) has to 1 to turn on a grounded (0) column
		
		switch (mb_state)
		{
			//State Transitions 
			case MB_START:
				if (up_button&& (current_row!=top) )
				{
					mb_state = MB_SHIFT_UP;
					break;
				}
				if (down_button&& current_row!=bottom)
				{
					mb_state = MB_SHIFT_DOWN;
					break;
				}
				else{
					mb_state = MB_START;
					break;
				}
			break;
			case MB_SHIFT_UP:
				if (up_button&& (current_row!=top) )
				{
					mb_state = MB_SHIFT_UP;
					break;
				}
				if (down_button&& (current_row!=bottom) )
				{
					mb_state = MB_SHIFT_DOWN;
					break;
				}
				else{
					break;
				}
			case MB_SHIFT_DOWN:
			if (up_button&& (current_row!=top) )
			{
				mb_state = MB_SHIFT_UP;
				break;
			}
			if (down_button&& (current_row!=bottom) )
			{
				mb_state = MB_SHIFT_DOWN;
				break;
			}
			else{
				break;
			}
			default:
				break;
		}

		switch (mb_state)
		{
			//State Actions 
			case MB_START: 
				turn_on = 0x01;
				column_num = 0x00; // so whole first row should be on
				current_row = 0;
			break;
			case MB_SHIFT_UP:
				if (up_button&&current_row!=top)
				{
					for (int i=0; i<=current_row; i++)
					{
						turn_on +=1;
						current_row+=1;
					}
					break;
				}
				if (up_button current_row ==top)
				{
					turn_on = turn_on;
					current_row = current_row;
				}
					
		}
		 
}*/

enum MakeSquare_States{MS_START, MS_SIDES, MS_TOPandBOT} ms_state;
void MakeSquare_Tick(){
	//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*A0 0 |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |
	  A2 1 |  |  | @| @| @| @|  |  |
	  A3 1 |  |  | @|  |  | @|  |  |
	  A4 1 |  |  | @|  |  | @|  |  |
	  A5 1 |  |  | @| @| @| @|  |  |
	  A6 0 |  |  |  |  |  |  |  |  |
	  A7 0 |  |  |  |  |  |  |  |  |
  			0  0  0  0  0  0  0  0  
		    B0 B1 B2 B3 B4 B5 B6  B7 */
	//Start with ball in Left Top corner
		// === Local Variables ===
		static uc turn_on = 0x00; // sets the pattern displayed on columns
		static uc column_num = 0xFF; // grounds column to display pattern
		//Remember Columns have to be 0 first in order to turn on
		//Row value (turn_on) has to 1 to turn on a grounded (0) column
		
		switch (ms_state)
		{
			//State Transitions 
			case MS_START:
				ms_state = MS_SIDES;
			break;
			case MS_SIDES:
				ms_state = MS_TOPandBOT;
			break;
			case MS_TOPandBOT:
				ms_state = MS_SIDES;
			break;

			default:
				break;
		}

		switch (ms_state)
		{
			//State Actions 
			case MS_START: 
			break;
			case MS_SIDES:
				//sides
				turn_on = 0x3C;
				column_num = ~0x24;
			break;
			case MS_TOPandBOT:
				//top+bottom
				turn_on = 0x24;
				column_num = ~0x3C;
			break;
			default:
			break;
		}
		
		PORTA = turn_on;
		PORTB = column_num;
		
		 
}
int main(void)
{
	/* Replace with your application code */
	DDRA = 0xFF; //set port A to output
	PORTA = 0x00; //init port A to 0's
	DDRB = 0xFF; //set port B to output
	PORTB = 0x00; //init port B to 0's
	DDRC = 0x00; //set port C to intput
	PORTC = 0xFF; //init port C to 1's
	
	
	const unsigned long timerPeriod =1;

	TimerSet(timerPeriod);
	TimerOn();	
	ms_state = MS_START;
	
	while (1)
	{
		//SM1_Tick(sm1_display);
		MakeSquare_Tick();
		while(!TimerFlag); //wait 1 sec
		TimerFlag = 0;
	}
	return 0;
}



/*Design a system where a 4x4 hollow square is displayed in the center
 of the LED matrix like the image below:

Hints:
Since only one column pattern can be displayed on the LED matrix at a time, to display the square, different patterns need to be displayed at different times
Every one ms, update the pattern to be displayed and the column that the pattern will be displayed on.
Use two arrays to store relevant data for the square. One array stores the patterns for each column. The other array stores the column that displays the pattern.
Ground only one column at a time.

*/