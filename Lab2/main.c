/*
 * CS120B_lab2_part1.c
 *
 * Created: 8/1/2018 1:08:57 PM
 * Author : Keilani (kconn006)
 */ 

#include <avr/io.h>
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs,
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char tempA = 0x00; // intermediate variable used for port updates
	unsigned char tempB = 0x00; // intermediate variable used for port updates
	unsigned char tempC = 0x00; // intermediate variable used for port updates


    /* Replace with your application code */
    while (1) 
    {
		unsigned char i;
		unsigned char cnt;
		tempA = PINA & 0x00;
		tempB = PINB & 0x00;
		tempC = PINC & 0x00;
		cnt=0;
		for (i=0; i<8; i++)
		{
			if (GetBit(tempA, i)) {
				cnt++;
			}
			if (GetBit(tempB, i)) {
				cnt++;
			}
		tempC = cnt;
		}
    }
	PORTC = tempC;
}

