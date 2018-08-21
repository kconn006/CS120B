/*
 * Lab10_part1.c
 *
 * Created: 8/21/2018 12:21:16 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include "lcd_io.c"

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "Hello World");
	
	while(1) {continue;}
}


