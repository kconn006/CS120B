#ifndef PLAYER1_PADDLE_H
#define PLAYER1_PADDLE_H
#include "bit.h"
#include "matrix.h"
////////////////////////////////////////////////////////////////////////////////
//Player1 Names and all that
/*
enum P1_Paddle_States{P1P_START, P1P_PRESS, P1P_RELEASE, P1P_UP, P1P_DOWN, P1P_RESET} p1p_state;
void p1_paddle_Tick(){
	//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*int a[3][4] = {
		{0, 1, 2, 3} ,     initializers for row indexed by 0 
		{4, 5, 6, 7} ,     initializers for row indexed by 1 
		{8, 9, 10, 11}     initializers for row indexed by 2 
	*/
	/*A0 0 |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |
	  A2 1 |  |  |  |  |  |  |  |  |
	  A3 1 |  |  |  |  |  |  |  |  |
	  A4 1 |  |  |  |  |  |  |  |  |
	  A5 1 | @|  |  |  |  |  |  |  |
	  A6 0 | @|  |  |  |  |  |  |  |
	  A7 0 | @|  |  |  |  |  |  |  |
  			1  1  0  0  0  0  1  1  
		    B0 B1 B2 B3 B4 B5 B6  B7 */
	//add ms delay at end of each for loop
	  /*uc matrix[8][8]= { {0,0,0,0,0,1,1,1},//Row A0
						   {0,0,0,0,0,0,0,0},//Row A1
						   {0,0,0,0,0,0,0,0},//Row A2
						   {0,0,0,0,0,0,0,0},//Row A3
						   {0,0,0,0,0,0,0,0},//Row A4
						   {0,0,0,0,0,0,0,0},//Row A5
						   {0,0,0,0,0,0,0,0},//Row A6
						   {0,0,0,0,0,0,0,0} };//Row A7
						//  0 1 2 3 4 5 6 7 //Matrix will print 
uc up_button = ~PINC & 0x01; //C0
uc down_button = ~PINC & 0x02;//C1
uc reset_button = ~PINC & 0x04;//C2
uc p1_turn_on = 0x00;
uc p1_column_num = 0x00;

	switch(p1p_state)
	{
		case P1P_START:
			if (!up_button && !down_button)
			{
				p1p_state = P1P_START; //Stay in start if no button pressed
			}
			else if (up_button && !down_button)
			{
				p1p_state = P1P_UP;//C0 = press
			}
			else if (!up_button && down_button) {
				p1p_state = P1P_DOWN;//C1 = press
			}
			else if (reset_button)
			{
				p1p_state = P1P_RESET;//C2 =press start player1 over
			}
		break;
		case P1P_UP: 
			if (reset_button)
			{
				p1p_state = P1P_RESET;//C2 =press start player1 over
			}
			else 
			{//Move up then read release
				p1p_state = P1P_RELEASE; 
			}
		break;
		case P1P_DOWN:
			if (reset_button)
			{
				p1p_state = P1P_RESET;//C2 =press start player1 over
			}
			else
			{//Move down then read release
				p1p_state = P1P_RELEASE;}
		break;
		case P1P_RELEASE:
			if (!up_button && !down_button )
			{//buttons released
				p1p_state = P1P_PRESS;
			}
			else if (reset_button)
			{
				p1p_state = P1P_RESET;
			}
			else{p1p_state = P1P_RELEASE;}
		break;
		case P1P_PRESS:
			if (up_button && !down_button)
			{
				//Move up 
				p1p_state = P1P_UP;
			}
			else if (!up_button && down_button)
			{
				p1p_state = P1P_DOWN;
			}
			else if (reset_button)
			{
				p1p_state = P1P_RESET;
			}
			else {p1p_state=P1P_PRESS;}
			break;
		case P1P_RESET:
			p1p_state = P1P_START;
			break;
		default:
		break;
	}
	
	switch(p1p_state)
	{//State Actions
		case P1P_START:
			//call function that displays player1
			//paddle in bottom right corner PORTA or y-comp=0xE0
			p1_turn_on = 0x38;
			p1_column_num = 0x7E;
			matrix_display();
		break;
		case P1P_UP:
		//Call function that moves ball up
		//Function will increase ball y-comp, shift (PORTA) to LEFT 
		break;
		case P1P_DOWN:
		//Call function that moves ball down
		//Function will decrease ball y-comp, shift (PORTA) to RIGHT
		break;
		case P1P_RELEASE:
		break;
		case P1P_PRESS:
		break;
		case P1P_RESET:
			reset = 1;
			//Handle reset outside this state machine
		break;
		default:
		break;
	}
	
	//Will handle output here
	//PORTA = row_on; //Turns on row Ax
	//PORTB = column_grnd; //Grounds column Bx
}
*/

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

#endif //PLAYER1_PADDLE_H


