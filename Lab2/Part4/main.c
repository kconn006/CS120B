/*
 * CS120B_lab2_part4.c
 *
 * Created: 8/1/2018 3:46:47 PM
 * Author :Keilani Conner kconn006
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	unsigned char tempA = 0x00;
	unsigned char map_on_B;
	unsigned char map_on_C;

    while (1) 
    {
		tempA = PINA & 0xFF;
		map_on_B = tempA >> 4;
		map_on_C = tempA << 4;
		PORTB = map_on_B;
		PORTC = map_on_C;
    }
}

