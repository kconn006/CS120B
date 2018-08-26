v#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"
#include "lcd_io.c"
//#include "lcd_8bit_task.h"
#include "queue.h"


unsigned char string[] = {'C', 'S', '1', '2', '0', 'B', ' ',  'i', 's', ' ',  'L', 'e', 'g', 'e', 'n', 'd', '.', '.', '.',  'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ',  'i', 't', ' ', 'D', 'A', 'R', 'Y', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
unsigned char cnt = 1;
enum States{init} state;
	
int str_tick(int state) {
	switch(state) {
		case init:
		for (unsigned int i = 1; i <= 16; i++) {
			LCD_Cursor(i);
			LCD_WriteData(string[i+cnt-2]);
		}
		cnt++;
	}
	
	return state;
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRA = 0xFF; PORTA = 0x00; //outputs
	DDRD = 0xFF; PORTD = 0x00;
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = 15;

	//Recalculate GCD periods for scheduler
	unsigned long int lcd_period = 15;
	
	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	
	//task1
	task1.state = 0;//Task initial state.
	task1.period = lcd_period;//Task Period.
	task1.elapsedTime = lcd_period;//Task current elapsed time.
	task1.TickFct = &str_tick;//Function pointer for the tick.
	
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


/*int main(void) {
	
	DDRA = 0xFF; PORTA = 0x00; //outputs
	DDRD = 0xFF; PORTD = 0x00;
	
   // Initializes the LCD display
   LCD_init();
   
   // Starting at position 1 on the LCD screen, writes Hello World
   LCD_DisplayString(1, "Hello World");
   
   while(1) {continue;}
   
}*/

/*Use the LCD code, along with a button and/or time delay to display the message "CS120B is Legend... wait for it DARY!"
The string will not fit on the display all at once, so you will need to come up with some way to paginate or scroll the text.
Note: If your LCD is exceptionally dim, adjust the resistance provided by the potentiometer connected to Pin #3.*/