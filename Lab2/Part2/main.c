/*
 * CS120B_lab2_part2.c
 *
 * Created: 8/1/2018 1:35:27 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>
// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
		DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
		DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
    /* Replace with your application code */
	unsigned char tempA = 0x00;
	unsigned char tempC = 0x00;
	tempC = PINC;


    while (1) 
    {	
		tempA = PINA & 0x0F;
 		if ( (tempA == 1) || (tempA == 2) )  //Turn on PC5
		{
			tempC = SetBit(tempC, 5, 1);
		}
		else if ( (tempA == 3) || (tempA == 4) ) //Turn on PC5 &PC4
		{
			tempC = SetBit(tempC, 5, 1);
			tempC = SetBit(tempC, 4, 1);
		}
 		else if ( (tempA == 5) || (tempA == 6) ) //Turn on PC5 & PC4 &PC3
		{
			tempC = SetBit(tempC, 5, 1);
			tempC = SetBit(tempC, 4, 1);
			tempC = SetBit(tempC, 3, 1);
		}
		else if ( (tempA == 7) || (tempA == 9) ) //Turn on PC5 & PC4 &PC3 &PC2 
		{
			tempC = SetBit(tempC, 5, 1);
			tempC = SetBit(tempC, 4, 1);
			tempC = SetBit(tempC, 3, 1);
			tempC = SetBit(tempC, 2, 1);
		}
		else if ( (tempA == 10) || (tempA == 12) ) //Turn on PC5 & PC4 &PC3 &PC2 &PC1
		{
			tempC = SetBit(tempC, 5, 1);
			tempC = SetBit(tempC, 4, 1);
			tempC = SetBit(tempC, 3, 1);
			tempC = SetBit(tempC, 2, 1);
			tempC = SetBit(tempC, 1, 1);
		}
		else if ( (tempA == 13) || (tempA == 15) ) //Turn on PC5 & PC4 &PC3 &PC2 &PC1 &PC0
		{
			tempC = SetBit(tempC, 5, 1);
			tempC = SetBit(tempC, 4, 1);
			tempC = SetBit(tempC, 3, 1);
			tempC = SetBit(tempC, 2, 1);
			tempC = SetBit(tempC, 1, 1);
			tempC = SetBit(tempC, 0, 1);
		}
		if (tempA <= 4)
		{
			tempC = SetBit(tempC, 6, 1);
		}
		PORTC = tempC;
    }
	
}

