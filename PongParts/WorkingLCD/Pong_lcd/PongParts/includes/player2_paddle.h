#ifndef PLAYER2_PADDLE_H
#define PLAYER2_PADDLE_H
#include "bit.h"
#include "shared_variables.h"
#include "matrix.h"
////////////////////////////////////////////////////////////////////////////////
//Player2 Names and all that
/*
enum P2_Paddle_States{P2P_START, P2P_PRESS, P2P_RELEASE, P2P_UP, P2P_DOWN, P2P_RESET} p2p_state;
void p2_paddle_Tick(){
	*/
enum P2_Paddle_States {p2_display, p2_move_up, p2_release, p2_move_down,p2_stay};
int p2_paddle_Tick(int p2_state) {

	// === Local Variables ===
	static unsigned char p2_turn_on = 0x38; // sets the pattern displayed on columns
	static unsigned char p2_column_num = 0xFE; // grounds column to display pattern
	uc p2_up = ~PINC &0x08;
	uc p2_down = ~PINC & 0x10;
	uc top = 0x0E;
	uc bottom = 0xE0;
	//uc max_turn_on =
	// === Transitions ===
	switch (p2_state) {
		case p2_display:
		if (p2_up)
		{
			p2_state = p2_move_up;
		}
		else if (p2_down)
		{
			p2_state = p2_move_down;
		}
		else {p2_state = p2_display;}
		break;
		case p2_move_up:
		if (!p2_up)
		{p2_state = p2_stay;}
		
		if (p2_up)
		{p2_state = p2_move_up;}
		break;
		case p2_move_down:
		if (!p2_down)
		{
			p2_state = p2_stay;
		}
		break;
		case p2_stay:
		p2_state = p2_stay;
		break;
		break;
		default:
		p2_state = p2_stay;
		break;
	}
	
	// === Actions ===
	switch (p2_state) {
		case p2_display:   // illuminate LED in First col
		p2_turn_on = p2_turn_on; // display far left column
		p2_column_num = p2_column_num; // pattern illuminates top row
		break;
		case p2_move_up:
		if (p2_turn_on > top){
		p2_turn_on = (p2_turn_on >> 1); }
		
		p2_turn_on = p2_turn_on;
		p2_column_num = p2_column_num;
		break;
		case p2_move_down:
		if (p2_turn_on < bottom){
		p2_turn_on = (p2_turn_on << 1);}
		
		p2_turn_on =p2_turn_on;
		p2_column_num = p2_column_num;
		break;
		case p2_stay:
		p2_turn_on = p2_turn_on;
		p2_column_num = p2_column_num;
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
		p2MatrixRow= p2_turn_on;
		p2MatrixColumn = p2_column_num;
		//matrix_display(p1_turn_on,p1_column_num);
		//PORTA = try_turn_on; // PORTA displays column pattern
		//PORTB = try_column_num; // PORTB selects column to display pattern

	return p2_state;
}
#endif //PLAYER2_PADDLE_H


