#ifndef PLAYER1_PADDLE_H
#define PLAYER1_PADDLE_H
#include "bit.h"
////////////////////////////////////////////////////////////////////////////////
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
						//  0 1 2 3 4 5 6 7 */ //Matrix will print 
uc up_button = ~PINC & 0x01; //C0
uc down_button = ~PINC & 0x02;//C1
uc reset_button = ~PINC & 0x04;//C2

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
#endif //PLAYER1_PADDLE_H


