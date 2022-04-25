#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile float potentiometer = 0; // potentiometer value
volatile int moves = 0; //moves
volatile int compValue = 0; // global variable to track number of compValue
volatile int delayValue = 0; // delay

void adcSetup();
void readADC();
void rotate();
void pinSetup();
void ctcSetup();

int main(void)
{
	pinSetup(); // setup functions for movesper/ctc/adc
	ctcSetup();
	adcSetup();
	
	while (1)
	{
		readADC(); // read pot value
		
		if (potentiometer > 100)
		{
			delayValue = 1; // set max speed of movesper motor
		}
		else if (potentiometer > 1)
		{
			delayValue = (100 - potentiometer); // scale motor to potentiometer
		}
		else
		{
			delayValue = 10000; // turn off motor
		}
	}
}


void pinSetup() //setups the pins for movesper motor
{

	DDRB |= (1<<1);// set PortB 1-4 as outputs
	DDRB |= (1<<2);
	DDRB |= (1<<3);
	DDRB |= (1<<4);

	PORTB &= ~(1<<1);// start PinB 1-4 at low
	PORTB &= ~(1<<2);
	PORTB &= ~(1<<3);
	PORTB &= ~(1<<4);
}

void ctcSetup()// function the sets up the CTC Timer
{
	OCR0A = 125; // Set compare register to 125
	TCCR0A = 2; // Enable CTC Mode
	TCCR0B = 4; // set the pre-scaler to 256 and start timer
	TIMSK0 = (1<<OCIE0A); // Enable the timer interrupt
	sei(); // Enable global interrupts
}

void adcSetup() // function to set up the adc
{
	// set PC0 as an input and active high (potentiometer)
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

void readADC() // function the reads the potentiometer value using ADC
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
	potentiometer = potentiometer/600; // scale the value down by 600 (range of 0-110)
}

void rotate() // here we rotate the movesper motor accordingly
{
	if(moves == 1)
	{
		PORTB |= (1<<1);
		PORTB &= ~(1<<2);
		PORTB &= ~(1<<3);
		PORTB &= ~(1<<4);
	}
	if(moves == 2)
	{
		PORTB &= ~(1<<1);
		PORTB |= (1<<2);
		PORTB &= ~(1<<3);
		PORTB &= ~(1<<4);
	}
	if(moves == 3)
	{
		PORTB &= ~(1<<1);
		PORTB &= ~(1<<2);
		PORTB |= (1<<3);
		PORTB &= ~(1<<4);
	}
	if(moves == 4)
	{
		PORTB &= ~(1<<1);
		PORTB &= ~(1<<2);
		PORTB &= ~(1<<3);
		PORTB |= (1<<4);
	}
	if (moves == 4)
	{
		moves = 1; //reset to moves 1
	}
	else
	{
		moves++; // else we go to next moves
	}
}

ISR (TIMER0_COMPA_vect) // CTC Interrupt Function
{
	compValue++; // increment compValue
	if (compValue >= delayValue){ // if we delayed long enough (based on potentiometer value)
		rotate(); // call the rotate function to turn a moves
		compValue = 0; // reset compValue
	}
}