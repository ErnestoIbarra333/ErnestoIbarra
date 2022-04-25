#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile float potentiometer = 0; // value of potentiometer
void adcSetup();
void intSetup();
void PWMSetup();
void readADC();

int main(void)
{
	DDRC &= ~(1<<0); // make portc.0 an input (potentiometer)
	DDRC |= (1<<3); // make portc.3 an output (motor STBY)
	DDRC &= ~(1<<1); // make portc.1 as an input (button)
	DDRB |= (1<<1); // make portb.1 an output (motor PWM)
	PORTC |= (1<<3); // set portc.3 high
	PORTC |= (1<<0); // make portc.0 active high
	PORTC |= (1<<1); // make portc.1 active high
	
	adcSetup(); // initialize the ADC
	intSetup(); // initialize the interrupt
	PWMSetup();// initialize the PWM

	while (1)
	{
		readADC();
		_delay_ms(100);

		if ((potentiometer >= 62260) && (potentiometer < 65535))
		{
			OCR1A = 62260; // set the PWM to 95% of max
			_delay_ms(50); // wait to set in speed
		}
		else if ((potentiometer < 62257) && (potentiometer >= 3000))
		{
			OCR1A = potentiometer; // set the motor to potentiometer scaled value
			_delay_ms(50); // wait to set in speed
		}
		else
		{
			OCR1A = 0; //basically turns off motor
		}
	}
}

void adcSetup() //set up the ADC
{
	// use AVCC
	ADMUX |= (0<<REFS1);
	ADMUX |= (1<<REFS0);
	
	// select ADC0
	ADMUX |= (0<<MUX2);
	ADMUX |= (0<<MUX1);
	ADMUX |= (0<<MUX0);
	
	// left align
	ADMUX |= (1<<ADLAR);
	
	// enable ADC
	ADCSRA |= (1<<ADEN);
	
	// set pre-scaler to 128
	ADCSRA |= (1<<ADPS2);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS0);
}

void intSetup() //interrupt for pin
{
	PCICR = (1<<PCIE1); // enable pin change interrupt 1
	PCMSK1 = (1<<PCINT9); // Mask for PortC.1
	sei(); // enable global interrupts
}

void PWMSetup() // function to setup the PWM mode
{
	ICR1 = 0XFFFF; // ICR1 as top
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1); // non-inverted mode
	TCCR1A |= (1<<WGM11); // fast PWM
	TCCR1B |= (1<<WGM12)|(1<<WGM13); // fast PWM
	TCCR1B |= (1<<CS10); // start timer
}

void readADC() // function to readADC value
{
	int samples = 15; // number of samples
	potentiometer = 0; // initial potentiometer
	for (int i = 0; i < samples; i++)
	{
		ADCSRA |= (1<<ADSC); // start the ADC conversion
		while(ADCSRA & (1<<ADSC)); // wait until the conversion is done
		potentiometer += ADC; // store the value from the conversion
	}
	potentiometer = potentiometer/15; // take the average value
}

ISR(PCINT1_vect) // timer function
{
	if( (PINC & (1<< PINC1)) == 0 )
	{
		PORTC ^= (1<<3);
	}
}