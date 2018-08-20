/*
 * Lab8_part1.c
 *
 * Created: 8/15/2018 1:11:00 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion comple
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port B's 8 pins as outputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	
	unsigned short ADC_result =0x00;
	unsigned char tempB = 0x00;  
	unsigned char tempD = 0x00;
	
	ADC_init();

	while(1)
	{
		ADC_result = ADC;
		tempB = (char)(ADC_result >> 8);
		tempD = (char)ADC_result;
		
		PORTD = tempD;
		PORTB = tempB;
		

	}
}
