/*
 * kmccu003__lab4_part1.c
 *
 * Created: 8/5/2018 5:22:50 PM
 * Author : ANARCHY
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
	//DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	// initialize to 0s
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpC = 0x00;
	//unsigned char count = 0x00;
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = ~PINA & 0x0F; // Mask PINA to only get the bit you are interested in, first 4 bits
		tmpC = 0x00;
		// 2) Perform Computation
		
		if (tmpA <= 4) { //low fuel PC6
			tmpC = SetBit(tmpC, 6, 1);
		}
		
		else {
			tmpC = SetBit(tmpC, 6, 0); //turn off low fuel light if not less than 4
		}
		
		if (tmpA == 1 || tmpA == 2) {
			tmpC = SetBit(tmpC, 5, 1); //clear lsb, then set to 1
			//tmpC = 0x20;
		}
		
		if (tmpA == 3 || tmpA == 4) {
			//tmpC = SetBit(tmpC, 1, 1);
			tmpC = 0x30;
		}
		
		if (tmpA == 5 || tmpA == 6) {
			tmpC = 0x38;
		}
		
		if (tmpA == 7 || tmpA == 8 || tmpA == 9) {
			tmpC = 0x3C;
		}
		
		if (tmpA == 10 || tmpA == 11 || tmpA == 12) {
			tmpC = 0x3E;
		}
		
		if (tmpA == 13 || tmpA == 14 || tmpA == 15) {
			tmpC = 0x3F;
		}
		
		PORTC = tmpC;

		// 3) write results to port
		//PORTC = count; //result of 1's in A & B
	}
}


/*A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full).
A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level.
If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light.
Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0.
Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less.
(The example below shows the display for a fuel level of 3).*/

/*PB0 and PB1 each connect to an LED, and PB0's LED is initially on.
Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED,
staying that way after button release. Pressing the button again turns off PB1's LED and
turns on PB0's LED.*/