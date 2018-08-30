#ifndef BALL_LOGIC_H
#define BALL_LOGIC_H
#include "ball_control.h"
#include "shared_variables.h"
#include "matrix.h"
#include "player1_paddle.h"
#include "player2_paddle.h"

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
		
		//uc LeftWall = 0x7F;
		//uc RightWall = 0xFE;
		
	switch (ball_state)
		{	//State Transitions 
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
				{	bally = 0x08;
					ballx = ~(0x10);
					ball_state = ball_display;
				}
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
		//Matrix_display( y-axis, x-axis
		//matrix_display(bally,ballx);
		ballMatrixRow = bally;
		ballMatrixColumn = ballx;
		//matrix_on(ballMatrixRow,ballMatrixColumn);
}

#endif