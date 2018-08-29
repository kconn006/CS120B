#include <avr/io.h>
#include <avr/interrupt.h>
#include "matrix.h"
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"
#include "lcd_io.c"
#include "ball_control.h"
enum Try_States {try_display, try_move_up, try_release, try_move_down,try_stay};
int Try_Tick(int try_state) {

	// === Local Variables ===
	static unsigned char try_turn_on = 0x38; // sets the pattern displayed on columns
	static unsigned char try_column_num = 0x7F; // grounds column to display pattern
	uc button = ~PINC &0x02;
	uc button2 = ~PINC & 0x04;
	uc top = 0x07;
	uc bottom = 0xE0;
	//uc max_turn_on =
	// === Transitions ===
	switch (try_state) {
		case try_display: 
			if (button)
			{
				try_state = try_move_up;
			}
			else if (button2)
			{
				try_state = try_move_down;
			}
			else {try_state = try_display;}
		break;
		case try_move_up:
			if (!button)
			{try_state = try_stay;}
			
			if (button)
			{try_state = try_move_up;}
		break;
		case try_move_down:
			if (!button2)
			{
				try_state = try_stay;
			}
		break;
		case try_stay:
			try_state = try_stay;
			break;
		break;
		default:   	        
		try_state = try_stay;
		break;
	}
	
	// === Actions ===
	switch (try_state) {
		case try_display:   // illuminate LED in First col
			try_turn_on = try_turn_on; // display far left column
			try_column_num = try_column_num; // pattern illuminates top row
		break;
		case try_move_up: 
		if (try_turn_on > top){
			try_turn_on = (try_turn_on >> 1); }
			
			try_turn_on = try_turn_on;
			try_column_num = try_column_num;
			break;
		case try_move_down:
		if (try_turn_on < bottom){
			try_turn_on = (try_turn_on << 1);}
			
			try_turn_on =try_turn_on;
			try_column_num = try_column_num;
		break;
		case try_stay:
			try_turn_on = try_turn_on;
			try_column_num = try_column_num;
			break;
		// else if far right column was last to display (grounded)
		//else if (column_sel == 0xFE) {
			//column_sel = 0x7F; // resets display column to far left column
			//column_val = column_val << 1; // shift down illuminated LED one row
		//}
		// else Shift displayed column one to the right
		//else {
		//	column_sel = (column_sel >> 1) | 0x80;
		//}
		
		default: 
		break;
	}
	matrix_display(try_turn_on,try_column_num);
	//PORTA = try_turn_on; // PORTA displays column pattern
	//PORTB = try_column_num; // PORTB selects column to display pattern

	return try_state;
}
//Advanced
enum MakeSquare_States{ms_start, ms_sides, ms_top} ms_state;
void MakeSquare_Tick(){
	//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*A0 0 |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |
	  A2 0 |  |  |  |  |  |  |  |  |
	  A3 1 |  |  | @| @| @| @|  |  |
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
			case ms_start:
				ms_state = ms_sides;
			break;
			case ms_sides:
				ms_state = ms_top;
			break;
			case ms_top:
				ms_state = ms_sides;
			break;

			default:
				break;
		}

		switch (ms_state)
		{
			//State Actions 
			case ms_start: 
			break;
			case ms_sides:
				//sides
				turn_on = 0x3C;
				column_num = ~0x24;
			break;
			case ms_top:
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
enum MakeBall_States{ball_display, ball_right, ball_left} ball_state;
void MakeBall_Tick(){
//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*A0 0 |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |
	  A2 0 |  |  |  |  |  |  |  |  |
	  A3 1 |  |<-|--|-@|  |  |  |  |
	  A4 0 |  |  |  |  |  |  |  |  |
	  A5 0 |  |  |  |  |  |  |  |  |
	  A6 0 |  |  |  |  |  |  |  |  |
	  A7 0 |  |  |  |  |  |  |  |  |
  			0  0  0  1  0  0  0  0  
		    B0 B1 B2 B3 B4 B5 B6  B7 */
	//Start with ball in Left Top corner
		// === Local Variables ===
		static uc bally = 0x00; // sets the pattern displayed on columns PORTA
		static uc ballx = 0xFF; // grounds column to display pattern PORTB
		//Remember Columns have to be 0 first in order to turn on
		//Row value (turn_on) has to 1 to turn on a grounded (0) column
		uc button = ~PINC &0x01; //C0
		uc button2 = ~PINC &0x01; //C0
		
		uc p1_middle = 0x7F;
		uc p2_middle = 0xFE;
		
		uc LeftWall = 0x7F;
		uc RightWall = 0xFE;
	///Original just goes left ugh	
		/*switch (ball_state)
		{
			//State Transitions 
			case ball_display:
			if (button)
			{ball_state =ball_left;}
			else if (button2)
			{ball_state =ball_right;}
			else{
				ball_state = ball_display;}
			break;
			
			case ball_left:
			if (button)
			{ball_state = ball_left;}
				
			else{ball_state=ball_display;}
			break;
			
			case ball_right:
				ball_state = ball_right;
			break;

			default:
				break;
		}

		switch (ball_state)
		{
			//State Actions 
			case ball_display: 
				bally = 0x08;
				ballx = ~(0x10); 
			break;
			case ball_left:
				//y = PortA
				//x=PortB
			if (ballx > LeftWall)
			{ballx = MoveXLeft(ballx);}
				ballx = ballx;
				bally = bally;
			break;
			case ball_right:
				//y = PortA
				//x=PortB
				if (ballx < RightWall)
				{ballx = MoveXRight(ballx);}
				ballx = ballx;
				bally = bally;
			break;
			default:
			break;
		}*
		
		
	//Matrix_display( turn_on, column_num)	
	//Matrix_display( PORTA, PORTB);
	//Matrix_display( y-axis, x-axis)


	matrix_display(bally,ballx);
}
int main(void)
{
	/* Replace with your application code */
	
	switch (ball_state)
		{
			//State Transitions 
			case ball_display:
				bally = 0x08;
				ballx = ~(0x10);
				//if (button)//Button will start ball to left
				{ball_state =ball_left;}
				
				//if(!button)
				//{ball_state = ball_display;}
			break;
			
			case ball_left:
				if (ballx > p1_middle)
				{ball_state = ball_left;}
				
				if(ballx == p1_middle)
				{ball_state=ball_right;}
					
				if (button)
				{bally = 0x08;
				ballx = ~(0x10);}
			break;
			
			case ball_right:
				if (ballx < p2_middle)
				{ball_state = ball_right;}

				if(ballx == p2_middle)
				{ball_state=ball_left;}
						
				if (button)
				{bally = 0x08;
				ballx = ~(0x10);
				ball_state = ball_display;}
			break;

			default:
				break;
		}

		switch (ball_state)
		{
			//State Actions 
			case ball_display: 
 
			break;
			case ball_left:
				//y = PortA
				//x=PortB
			{ballx = MoveXLeft(ballx);}
				ballx = ballx;
				bally = bally;
			break;
			case ball_right:
				//y = PortA
				//x=PortB
				{ballx = MoveXRight(ballx);}
				ballx = ballx;
				bally = bally;
			break;
			default:
			break;
		}
		//Matrix_display( turn_on, column_num)
		//Matrix_display( PORTA, PORTB);
		//Matrix_display( y-axis, x-axis)


		matrix_display(bally,ballx);
}
int main (void){
	DDRA = 0xFF; //set port A to output
	PORTA = 0x00; //init port A to 0's
	DDRB = 0xFF; //set port B to output
	PORTB = 0x00; //init port B to 0's
	DDRC = 0x00; //set port C to intput
	PORTC = 0xFF; //init port C to 1's
	
	
	const unsigned long timerPeriod =200;

	TimerSet(timerPeriod);
	TimerOn();	
	//ball_state = ball_display;
	
	while (1)
	{
		//SM1_Tick(sm1_display);
		//MakeSquare_Tick();
		//Try_Tick(try_display);
		MakeBall_Tick();
		while(!TimerFlag); //wait 1 sec
		TimerFlag = 0;
	}
	return 0;
}
