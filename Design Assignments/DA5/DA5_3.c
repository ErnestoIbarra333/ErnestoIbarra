#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile float potentiometer = 0; // global variable to hold potentiometer value
void adcSetup();
void pwmSetup();
void readADC();

int main(void)
{
	adcSetup(); // initialize ADC
	pwmSetup();
	
	while (1)
	{
		readADC(); // read in pote. value
		if (potentiometer > 570)
		{
			OCR1A=570; // sets the motor to 180 degrees
		}
		else if (potentiometer > 115)
		{
			OCR1A = potentiometer; // set the servo position based on potentiometer value
		}
		else
		{
			OCR1A = 115; // sets motor to 0 degrees
		}
	}
}

void adcSetup() // Here we set everything to use the potentiometer
{
	
	DDRC &= ~(1<<0);
	PORTC |= (1<<0);
	
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

void pwmSetup() // function to setup the PWM timer
{
	// setup Timer 1 for non-inverted PWM
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	// set the pre-scaler to 64 with fast PWM mode
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	// set the PWM frequency to  50Hz
	ICR1=4999;
	// set PB1 as the PWM output
	DDRB |= (1<<1);
}

void readADC() // function to read ADC value from the potentiometer
{
	int samples = 15; // number of samples
	potentiometer = 0; // initial potValue
	for (int i = 0; i < samples; i++)
	{
		ADCSRA |= (1<<ADSC); // start the ADC conversion
		while(ADCSRA & (1<<ADSC)); // wait until the conversion is done
		potentiometer += ADC; // store the value from the conversion
	}
	potentiometer = potentiometer/15; // take the average value
	potentiometer = (potentiometer/104) + 115; // scale the range to 125-567
}