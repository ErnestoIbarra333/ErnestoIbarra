#define F_CPU 16000000UL                               
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1) 
#define  T_pin PB1 // pin for trigger
int TimerOF = 0; // overflow counter

//Simple Wait Function
void Wait()
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        _delay_loop_2(0);
    }

}

void main()
{
		char distance_string[10];
		long count2;
		double distance;
		double Angle = 80;
		DDRB = 0x02; // set PB1 and PB2 as outputs
		USART_init(); // initialize the USART
 		TCCR1A = 0;  // start in normal mode
 		TIMSK1 = (1 << TOIE1); // enable timer 1 overflow interrupt
 		sei(); // enable interrupts
	
	TCCR3A |= (1<<COM3A1) | (1<<COM3B1) | (1<<WGM31);
    TCCR3B |= (1<<WGM33) | (1<<WGM32) | (1<<CS31) | (1<<CS30);
    ICR3 = 4999;

    DDRD|=(1<<PD0);   //PWM Pins as Out

    while(1)
    {
		double i = 115;
		while(i < 570) // here we start off the counter at 0 degrees which is 115 
		{
			OCR3A = i; //counter will go to 180 degrees which is 570
			i = i + 6.5;
			Wait(); // everytime the counter increments we take a snapshot of the results of our Ultrasonic Sensor
			
					PORTB |= (1 << T_pin);
					_delay_us(10); // add a quick trigger pulse for trigger pin
					PORTB &= (~(1 << T_pin));
					
					TCNT1 = 0; // start timer at 0
					TCCR1B = 0x41; // capture rising edge and with no pre scalar
					TIFR1 = 1<<ICF1; // clear ICP flag
					TIFR1 = 1<<TOV1; // clear overflow flag
					
					while ((TIFR1 & (1 << ICF1)) == 0); //We stay here until rising edge
					TCNT1 = 0; // start timer at 0
					TCCR1B = 0x01; // capture falling edge instead now, no pre scalar
					TIFR1 = 1<<ICF1; // clear ICP flag
					TIFR1 = 1<<TOV1; // clear overflow flag
					TimerOF = 0; // clear our overflow timer

					while ((TIFR1 & (1 << ICF1)) == 0); // we stay until falling edge
					count2 = ICR1 + (65535 * TimerOF); // receive value from capture register
					/* 8MHz Timer freq, sound speed = 343 m/s, calculation mentioned in doc. */
					distance = (double) count2 / (933);
					
					 Angle = Angle + 2.5;
					 dtostrf(Angle, 2, 0, distance_string);//turns distance into a string
					 strcat(distance_string, ","); // formatting
					 USART_putstring(distance_string);    //prints to terminal
					 dtostrf(distance, 2, 0, distance_string);//turns distance into a string
					 strcat(distance_string, "."); // formatting					 
					 USART_putstring(distance_string);    //prints to terminal
					
		}
		
		double j = i;
		Angle = 180;
		while(j > 110) // Here we go backwards from 180 to 0 degrees
		{
			OCR3A = j;
			j = j - 6.5; // we subtract instead of add since we are going backwards
			Wait();
					PORTB |= (1 << T_pin);
					_delay_us(10); // add a quick trigger pulse for trigger pin
					PORTB &= (~(1 << T_pin));
					
					TCNT1 = 0; // start timer at 0
					TCCR1B = 0x41; // capture rising edge and with no pre scalar
					TIFR1 = 1<<ICF1; // clear ICP flag
					TIFR1 = 1<<TOV1; // clear overflow flag
					
					while ((TIFR1 & (1 << ICF1)) == 0); //We stay here until rising edge
					TCNT1 = 0; // start timer at 0
					TCCR1B = 0x01; // capture falling edge instead now, no pre scalar
					TIFR1 = 1<<ICF1; // clear ICP flag
					TIFR1 = 1<<TOV1; // clear overflow flag
					TimerOF = 0; // clear our overflow timer

					while ((TIFR1 & (1 << ICF1)) == 0); // we stay until falling edge
					count2 = ICR1 + (65535 * TimerOF); // receive value from capture register
					/* 8MHz Timer freq, sound speed = 343 m/s, calculation mentioned in doc. */
					distance = (double) count2 / (933);
					
					 Angle = Angle - 2.5;
					 dtostrf(Angle, 2, 0, distance_string);//turns distance into a string
					 strcat(distance_string, ","); // formatting
					 USART_putstring(distance_string);    //prints to terminal
					 dtostrf(distance, 2, 0, distance_string);//turns distance into a string
					 strcat(distance_string, "."); // formatting					 
					 USART_putstring(distance_string);    //prints to terminal
					
		}
		Angle = 0;	
    }
}

ISR(TIMER1_OVF_vect)
{
	TimerOF++; // here we increment our counter
}

void USART_init(void)
{
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (0<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data)
{
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr)
{
	
	while(*StringPtr != 0x00)
	{
		USART_send(*StringPtr);
		StringPtr++;
	}
	
}


