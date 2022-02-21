#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main ()
{
	DDRB |= (1<<3);// PB3 as an output
	
	PORTB |= (1<<3);// turn off the LED
	
	DDRC &= (0<<2); //make portC.2 an input
	PORTC |= (1 <<2); //enable pull up
	
	PORTD |= (1<<2);//pull-up activated
	EICRA = 0x02;//make INT0 falling edge triggered
	EIMSK = (1<<INT0);//enable external interrupt 0
	sei ();//enable interrupts
	
	while (1)
	{
		if(!(PINC & (1<<PINC2))) // Here we are doing Part2 of the assignment
		{
			PORTB &= ~(1<<3); // turns on
			_delay_ms(1250);
		}
		else
		{
			PORTB |= (1<<3); // turns off
		}

	}
}
ISR (INT0_vect)//ISR for external interrupt 0, here we are doing part 3 of the assignment.
{
	PORTB &= (0<<3);//turn on PORTB.3
	_delay_ms(500);
}

