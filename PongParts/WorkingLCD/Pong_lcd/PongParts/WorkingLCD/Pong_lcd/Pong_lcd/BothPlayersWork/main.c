#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"
#include "lcd_io.c"
#include "shared_variables.h"
//#include "lcd_8bit_task.h"
#include "queue.h"
#include "player1_paddle.h"
#include "player2_paddle.h"
#include "ball_logic.h"
#include "ball_control.h"
#include "matrix.h"
typedef unsigned char uc;

/*Assumptions: User will wait at least 4s for menu string to display. 
Then user will wait 7s for player_str to display
*/


uc m_string[] = {' ', ' ', ' ','A', 'r', 'e', ' ', 'y', 'o', 'u',  ' ',  'r', 'e', 'a', 'd', 'y', ' ', 't', 'o',  ' ',  'p', 'l', 'a', 'y',  ' ',  'P', 'O', 'N', 'G', '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
uc m_cnt = 1;
enum menu_str_States{m_init} m_state;
int menu_str_tick(int m_state) {
	switch(m_state) {
		case m_init:
		for (unsigned int i = 1; i <= 16; i++) {
			LCD_Cursor(i);
			LCD_WriteData(m_string[i+m_cnt-2]);
			
			if(m_cnt+i+1 == 42)
			{ //gets cut off at DAR when 36..
				m_cnt = 0; //reset
			}
		}
		
		m_cnt++;
	}
	
	return m_state;
}

uc p_string[] = {' ', ' ', ' ','P','l','a','y','e','r', ' ',   '1',  ' ', 'i','s',' ',  'o','n',' ',   't','h','e',' ',  'l', 'e','f','t',' ', 'P','l','a','y','e','r', ' ',   '2',  ' ', 'i','s',' ',  'o','n',' ',   't','h','e',' ',  'r','i','g','h','t',  ' ', ' ', ' ', ' ', ' '};
uc p_cnt = 1;																																																						// |
enum player_str_States{p_init} p_state;
int player_str_tick(int p_state) {
	switch(p_state) {
		case p_init:
		for (unsigned int i = 1; i <= 16; i++) {
			LCD_Cursor(i);
			LCD_WriteData(p_string[i+p_cnt-2]);
			
			if(p_cnt+i+1 == 58)
			{ //gets cut off at DAR when 36..
				p_cnt = 0; //reset
			}
		}
		
		p_cnt++;
	}
	
	return p_state;
}

//Beginning Display on board
enum question_mark_States{QM_START, QM_SIDES, QM_TOPandBOT}qm_state;
void question_mark_tick(){
	//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*A0 1 |  |  | @| @| @| @|  |  |
	  A1 1 |  |  |  |  |  | @|  |  |
	  A2 1 |  |  |  |  |  | @|  |  |
	  A3 1 |  |  |  | @| @| @|  |  |
	  A4 1 |  |  |  | @|  |  |  |  |
	  A5 1 |  |  |  | @|  |  |  |  |
	  A6 0 |  |  |  |  |  |  |  |  |
	  A7 1 |  |  |  | @|  |  |  |  |
  			1  1  0  0  0  0  1  1  
		    B0 B1 B2 B3 B4 B5 B6  B7 */
	//Start with ball in Left Top corner
		// === Local Variables ===
		static uc qm_turn_on = 0x00; // sets the pattern displayed on columns
		static uc qm_column_num = 0xFF; // grounds column to display pattern
		//Remember Columns have to be 0 first in order to turn on
		//Row value (turn_on) has to 1 to turn on a grounded (0) column
		
		switch (qm_state)
		{
			//State Transitions 
			case QM_START:
				qm_state = QM_SIDES;
			break;
			case QM_SIDES:
				qm_state = QM_TOPandBOT;
			break;
			case QM_TOPandBOT:
				qm_state = QM_SIDES;
			break;

			default:
				break;
		}

		switch (qm_state)
		{
			//State Actions 
			case QM_START: 
			break;
			case QM_SIDES:
				//sides
				qm_turn_on = 0xBF;
				qm_column_num = 0xC3;
			break;
			case QM_TOPandBOT:
				//top+bottom
				qm_turn_on = 0xBF;
				qm_column_num = 0xC3;
			break;
			default:
			break;
		}
		
		PORTA = qm_turn_on;
		PORTB = qm_column_num;
		
		 
}
	
enum Menu_States{MENU_MAIN, MENU_Press, MENU_PLAYERS, MENU_READY } menu_state;
void Menu_tick() {
	uc start_button = ~PINC & 0x01; //Start = C0
	uc set_ready1 = 0;
	switch(menu_state) 
	{//Transitions
		case MENU_MAIN:
			if(!start_button){
				menu_state = MENU_MAIN;
			}
			if(start_button){
				menu_state = MENU_Press;
			}
		break;
		case MENU_Press:
			if (!start_button)
			{
				menu_state = MENU_PLAYERS;
			}
			if (start_button)
			{
				menu_state = MENU_Press;
			}
		break;
		case MENU_PLAYERS:
			if (start_button)
			{
				menu_state = MENU_READY;
			}
			else{menu_state = MENU_PLAYERS;}
		case MENU_READY:
			break;
		default:
		break;
	}
	switch(menu_state)
	{//State Actions
		case MENU_MAIN:
			menu_str_tick(m_init);
			//question_mark_tick();
		break;
		case MENU_Press:
		break;
		case MENU_PLAYERS:
			player_str_tick(p_init);
			//question_mark_tick();
			break;
		case MENU_READY:
			if(!start_button){
				set_ready1 =1;
				//LCD_DisplayString(1,"All players ready"); //Print to know in this state
				ready = set_ready1;
				if (ready = 1){
					LCD_DisplayString(1,"Ready!");
				}
			}
		default:
		break;
	}
	
	
}

//Player1 State Machine
enum Player1_States{P1_START, P1_SHOW_PADDLE} p1_state;
void Player1_tick(){
	
	switch(p1_state)
	{//State Transitions 
		case P1_START:
			if (ready==1)
			{
				p1_state = P1_SHOW_PADDLE;
				break;
			}
			p1_state = P1_START;
			break;
		case P1_SHOW_PADDLE:
			if (reset==1)
			{
				//break out and start game over
				//will handle later
			}
			break;	
	}
	
	switch (p1_state)
	{//State Actions
		case P1_START:
			break;
		case P1_SHOW_PADDLE:
			//Working on logic to call tick function to display paddle 1	
			//LCD_DisplayString(1, "P1:");		
			///p1_paddle_Tick();	
			p1_paddle_Tick(p1_display);		
			//Will display 3 light paddle on left side of screen
			break;
	}
	
}
//Player2 State Machine
enum Player2_States{P2_START, P2_SHOW_PADDLE} p2_state;
void Player2_tick(){
	
	switch(p2_state)
	{//State Transitions
		case P2_START:
		if (ready==1)
		{
			p2_state = P2_SHOW_PADDLE;
			break;
		}
		p2_state = P2_START;
		break;
		case P2_SHOW_PADDLE:
		if (reset==1)
		{
			//break out and start game over
			//will handle later
		}
		break;
	}
	
	switch (p2_state)
	{//State Actions
		case P2_START:
		break;
		case P2_SHOW_PADDLE:
		//Working on logic to call tick function to display paddle 1
		//LCD_DisplayString(1, "P1:");
		///p1_paddle_Tick();
		p2_paddle_Tick(p2_display);
		//Will display 3 light paddle on left side of screen
		break;
	}
	
}

enum Ball_States{Ball_Start, Show_ball} bt_state;
void Ball_Tick(){
	switch(bt_state)
	{//State Transitions
		case Ball_Start:
			if (ready==1){
				bt_state = Show_ball;}
			else{bt_state=Ball_Start;}
		break;
		case Show_ball:
			if (reset==1)
			{
				//break out and start game over
				//will handle later
			}
			break;
		default:
		break;
	}
	switch(bt_state)
	{//State Actions
		case Ball_Start:
		break;
		case Show_ball:
			MakeBall_Tick();
			break;
		default:
		break;
	}
	
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; //PORTA = output x-axis (rows) for LED matrix
	DDRB = 0xFF; PORTB = 0x00; // PORTB = output y-axis (columns) for LED matrix
	DDRC = 0xC0; PORTC = 0x1F; //PORTC = pins 6 & 7 output to LCD E & RS
							   //PORTC = pins 0 - 4 inputs to game
							   //pin 0 = start
							   //pin 1 & 2 = player1 up & down
							   //pin 3 & 4 = player2 up & down
	DDRD = 0xFF; PORTD = 0x00; //PORTD output to LCD
	
	
	//Period for the tasks
	unsigned long int Menu_calc = 200;
	unsigned long int Player1_calc = 100;
	unsigned long int Player2_calc = 100;
	unsigned long int Ball_calc = 50;
	unsigned long int Matrix_calc = 5;
	
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int tempGCD = 1;
	tempGCD = findGCD(Menu_calc, Player1_calc);
	tempGCD = findGCD(tempGCD,Player2_calc);
	tempGCD = findGCD(tempGCD, Ball_calc);
	tempGCD = findGCD(tempGCD,Matrix_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int Menu_period = Menu_calc/GCD; //1
	unsigned long int Player1_period = Player1_calc/GCD;//2
	unsigned long int Ball_period = Ball_calc/GCD;//3
	unsigned long int Player2_period = Player2_calc/GCD;//4
	unsigned long int Matrix_period = Matrix_calc/GCD;//5

	
	//Declare an array of tasks
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	
	//task1
	task1.state = 0;//Task initial state.
	task1.period = Menu_period;//Task Period.
	task1.elapsedTime = Menu_period;//Task current elapsed time.
	task1.TickFct = &Menu_tick;//Function pointer for the tick.
	// Task 2
	task2.state = 0;//Task initial state.
	task2.period = Player1_period;//Task Period.
	task2.elapsedTime = Player1_period;//Task current elapsed time.
	task2.TickFct = &Player1_tick;//Function pointer for the tick.
	// Task 3
	task3.state = 0;//Task initial state.
	task3.period = Player2_period;//Task Period.
	task3.elapsedTime = Player2_period;//Task current elapsed time.
	task3.TickFct = &Player2_tick;//Function pointer for the tick.
	// Task 4
	task4.state = 0;//Task initial state.
	task4.period = Ball_period;//Task Period.
	task4.elapsedTime = Ball_period;//Task current elapsed time.
	task4.TickFct = &Ball_Tick;//Function pointer for the tick.
	// Task 5
	task5.state = 0;//Task initial state.
	task5.period = Matrix_period;//Task Period.
	task5.elapsedTime = Matrix_period;//Task current elapsed time.
	task5.TickFct = &Matrix_Tick;//Function pointer for the tick.
	
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	
	unsigned short i; // Scheduler for-loop iterator
	//unsigned char tmp = 0;
	
	while(1) {

		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;

	} //end while
	// Error: Program should not exit!
	
	return 0;
} //end main


/*Use the LCD code, along with a button and/or time delay to display the message "CS120B is Legend... wait for it DARY!"
The string will not fit on the display all at once, so you will need to come up with some way to paginate or scroll the text.
Note: If your LCD is exceptionally dim, adjust the resistance provided by the potentiometer connected to Pin #3.*/