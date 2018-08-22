/*
 * Lab10_part1.c
 *
 * Created: 8/20/2018 10:54:19 PM
 * Author : Keilani
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lcd_io.c"

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
unsigned char shifter =16;


enum DispMessageStates {DM_START, DM_INIT, DM_OFF} dm_state;
void Disp_Message_Tick(){
	char try[100]= {'H','e','l','l','o',' ','W','o','r','l','d',' ','C','S','1','2','0','B',' ','l','o','w','k','e','y',' ','s','u','c','k','s',' ','b','u','t',' ','i','t',' ','s',' ','a','p','a','r','t',' ','o','f',' ','m','y',' ','m','a','j','o','r',' ','\0'};
	//char wtf[6] = {'t','h','i','s'};
	

	for (int i = 0; try[i] != '\0'; ++i)
	{
			LCD_Cursor(1);
			LCD_WriteData(try+shifter);
	}
	shifter += shifter;

	/*switch(dm_state){
		//Transitions 
		case DM_START:
			dm_state = DM_INIT;
		break;
		case DM_INIT:
			dm_state = DM_INIT;
		break;
		/*case DM_OFF:
			dm_state = DM_START;
			break;
		default:
		break;
	}
	switch (dm_state)
	{
		case DM_START:
		break;
		case DM_INIT:
			//LCD_Cursor(1);
			//LCD_DisplayString(1,"Hey RiRi, you're amazing");

			for (int i = 0; try[i] != '\0'; ++i)
			{
				LCD_Cursor(1);
				LCD_WriteData(i);
			}
			shifter += shifter;

		break;
		case DM_OFF:
			break;
		default:
		break;
	}*/
}
	
	
	
int main(void)
{
	
	
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	
	// Initializes the LCD display
	LCD_init();
	
	TimerSet(2000); //1 sec
	TimerOn();
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_Cursor(1);
	LCD_DisplayString(1,"Hello World");
	while(1) {
		//Disp_Message_Tick();
		while(!TimerFlag) {};
			TimerFlag = 0;
	}
	
	return 0;
}
