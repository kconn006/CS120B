#ifndef PLAYER2_PADDLE_H
#define PLAYER2_PADDLE_H
#include "bit.h"
////////////////////////////////////////////////////////////////////////////////
enum P2_Paddle_States{P2P_START, P2P_PRESS, P2P_RELEASE, P2P_UP, P2P_DOWN, P2P_RESET} p2p_state;
void p2_paddle_Tick(){
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

	switch(p2p_state)
	{
		case P2P_START:
			if (!up_button && !down_button)
			{
				p2p_state = P2P_START; //Stay in start if no button pressed
			}
			else if (up_button && !down_button)
			{
				p2p_state = P2P_UP;//C0 = press
			}
			else if (!up_button && down_button) {
				p2p_state = P2P_DOWN;//C1 = press
			}
			else if (reset_button)
			{
				p2p_state = P2P_RESET;//C2 =press start player1 over
			}
		break;
		case P2P_UP: 
			if (reset_button)
			{
				p2p_state = P2P_RESET;//C2 =press start player1 over
			}
			else 
			{//Move up then read release
				p2p_state = P2P_RELEASE; 
			}
		break;
		case P2P_DOWN:
			if (reset_button)
			{
				p2p_state = P2P_RESET;//C2 =press start player1 over
			}
			else
			{//Move down then read release
				p2p_state = P2P_RELEASE;}
		break;
		case P2P_RELEASE:
			if (!up_button && !down_button )
			{//buttons released
				p2p_state = P2P_PRESS;
			}
			else if (reset_button)
			{
				p2p_state = P2P_RESET;
			}
			else{p2p_state = P2P_RELEASE;}
		break;
		case P2P_PRESS:
			if (up_button && !down_button)
			{
				//Move up 
				p2p_state = P2P_UP;
			}
			else if (!up_button && down_button)
			{
				p2p_state = P2P_DOWN;
			}
			else if (reset_button)
			{
				p2p_state = P2P_RESET;
			}
			else {p2p_state=P2P_PRESS;}
			break;
		case P2P_RESET:
			p2p_state = P2P_START;
			break;
		default:
		break;
	}
	
	switch(p2p_state)
	{//State Actions
		case P2P_START:
			//call function that displays player1
			//paddle in bottom right corner PORTA or y-comp=0xE0
		break;
		case P2P_UP:
		//Call function that moves ball up
		//Function will increase ball y-comp, shift (PORTA) to LEFT 
		break;
		case P2P_DOWN:
		//Call function that moves ball down
		//Function will decrease ball y-comp, shift (PORTA) to RIGHT
		break;
		case P2P_RELEASE:
		break;
		case P2P_PRESS:
		break;
		case P2P_RESET:
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
#endif //PLAYER2_PADDLE_H


