#ifndef PLAYER1_PADDLE_H
#define PLAYER1_PADDLE_H
#include "bit.h"
#include "shared_variables.h"
#include "matrix.h"
////////////////////////////////////////////////////////////////////////////////
//Player1 Names and all that
/*
enum P1_Paddle_States{P1P_START, P1P_PRESS, P1P_RELEASE, P1P_UP, P1P_DOWN, P1P_RESET} p1p_state;
void p1_paddle_Tick(){
	*/

enum P1_Paddle_States {p1_display, p1_move_up, p1_release, p1_move_down,p1_stay};
int p1_paddle_Tick(int p1_state) {

	// === Local Variables ===
	static unsigned char p1_turn_on = 0x38; // sets the pattern displayed on columns
	static unsigned char p1_column_num = 0x7F; // grounds column to display pattern
	uc p1_up = ~PINC &0x02;
	uc p1_down = ~PINC & 0x04;
	uc p1_top = 0x0E;
	uc p1_bottom = 0xE0;
	//uc max_turn_on =
	// === Transitions ===
	switch (p1_state) {
		case p1_display:
		if (p1_up)
		{
			p1_state = p1_move_up;
		}
		else if (p1_down)
		{
			p1_state = p1_move_down;
		}
		else {p1_state = p1_display;}
		break;
		case p1_move_up:
		if (!p1_up)
		{p1_state = p1_stay;}
		
		if (p1_up)
		{p1_state = p1_move_up;}
		break;
		case p1_move_down:
		if (!p1_down)
		{
			p1_state = p1_stay;
		}
		break;
		case p1_stay:
		p1_state = p1_stay;
		break;
		break;
		default:
		p1_state = p1_stay;
		break;
	}
	
	// === Actions ===
	switch (p1_state) {
		case p1_display:   // illuminate LED in First col
		p1_turn_on = p1_turn_on; // display far left column
		p1_column_num = p1_column_num; // pattern illuminates top row
		break;
		case p1_move_up:
		if (p1_turn_on > p1_top){
		p1_turn_on = (p1_turn_on >> 1); }
		
		p1_turn_on = p1_turn_on;
		p1_column_num = p1_column_num;
		break;
		case p1_move_down:
		if (p1_turn_on < p1_bottom){
		p1_turn_on = (p1_turn_on << 1);}
		
		p1_turn_on =p1_turn_on;
		p1_column_num = p1_column_num;
		break;
		case p1_stay:
		p1_turn_on = p1_turn_on;
		p1_column_num = p1_column_num;
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
	p1MatrixRow= p1_turn_on;
	p1MatrixColumn = p1_column_num;
	//matrix_display(p1_turn_on,p1_column_num);
	//PORTA = try_turn_on; // PORTA displays column pattern
	//PORTB = try_column_num; // PORTB selects column to display pattern

	return p1_state;
}

#endif //PLAYER1_PADDLE_H


