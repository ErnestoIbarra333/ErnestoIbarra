
#define F_CPU 16000000UL //Set clock frequency
#define BAUDRATE 9600 //Set the baud rate
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1) //Prescalar for the baud rate
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>


uint16_t ADCX; //ADC X-axis
uint16_t ADCY; //ADC Y-axis
char buffer[5]; //String for output of string function
float XF = 0.0;	//x-axis value to frequency for timer
float YDC = 0.0; //y-axis value to duty cycle for timer
float percent = 0.0;// Stores value of duty cycle to convert to a percentage
float Hzz = 0.0;// variable used to display frequency

void adc_init(void); //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel); //Function to read the analog input
void USART_init(void); //Function to initialize and configure the USART/serial
void USART_send( unsigned char data); //Function that sends a char over the serial port
void USART_putstring(char* StringPtr); //Function that sends a string over the serial port
void adc_convert(void); //Main function used to read the ADC values and use our timbers with those values
void timer_init(void); //Function to initialize the CTC timer1
void timer_update(void); //Function to update the timer values when new ADC values come in

int main(void){
	DDRC &= (0 << 4) | (0 << 5); //Set PC4 and PC5 as input
	DDRB |= (1 << 5); //set PB5 as output
	PORTB &= (0 << 5);
	adc_init(); //Start ADC
	USART_init(); //Start the USART
	timer_init(); //Start Timer1
	
	while(1)
	{        
		adc_convert(); //Grab ADC values and process them
		timer_update(); //Update timer
	}
	
	return 0;
}


void adc_convert(void) 
{
	// Here is part 2 of the HW here we will display the RAW values of the ADC
	ADCX = read_adc(4); //Read ADC channel 4 
	USART_putstring("RAW FREQ: ");
	itoa(ADCX, buffer, 10); 
	USART_putstring(buffer); 
	USART_putstring(",");           
	ADCY = read_adc(5); //Read ADC channel 5
	USART_putstring("RAW DUTY: ");
	itoa(ADCY, buffer, 10);        
	USART_putstring(buffer);        
	USART_send('\n');
	
	//Here we take care of part 3 of the assignment by using this IF statements to create the correct Frequency and Duty Cycle
	if (ADCX >= 512) 
	{
		XF = 260 + (1023-ADCX)/2; //If valX > 512 it will use this formula to scale down
	}
	else 
	{
		XF = 260+(512-ADCX)*15; //If valX < 512 it will use this formula to scale up
	}
	
	YDC = XF * (1.0 * ADCY / 1000.0); //This formula will convert valY into a percentage to get the duty cycle
	
	if(YDC >= XF) 
	{
		YDC = XF;
	}
	
	percent = ADCY / 10; //This will give us the duty cycle from 0 - 100
	//Hzz = XF 
	USART_putstring("Freq: ");
	itoa(XF, buffer, 10);
	USART_putstring(buffer);
	USART_putstring(",");
	USART_putstring("DUTY: ");
	itoa(percent, buffer, 10);
	USART_putstring(buffer);
	USART_putstring("%");
	USART_send('\n');
	_delay_ms(500);
}

void adc_init(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (1<<REFS0);                //Voltage reference from Avcc (5v)
	ADCSRA |= (1<<ADEN);                //Turn on ADC
	ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

uint16_t read_adc(uint8_t channel)
{
	ADMUX &= 0xF0;                    //Clear the older channel that was read
	ADMUX |= channel;                //Defines the new ADC channel to be read
	ADCSRA |= (1<<ADSC);                //Starts a new conversion
	while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
	return ADCW;                    //Returns the ADC value of the chosen channel
}

void timer_init(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); //Sets prescalar to 1024
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); //Enable OCR1A and OCR1B 
	TCNT1 = 0; // Start timer at 0
	sei(); //Activate global interrupts
}

void timer_update(void) 
{
	OCR1A = XF; //Store new frequency value
	OCR1B = YDC; //Store new duty cycle value
}

ISR(TIMER1_COMPB_vect) //Interrupt for duty cycle 
{
	PORTB |= (1 << 5); //turn on the LED
}

ISR(TIMER1_COMPA_vect)  //Interrupt for frequency
{ 
	PORTB &= (0 << 5); // turn off LED
	TCNT1 = 0; //Reset to zero
}

void USART_init(void)//Function to initialize the USART
{ 
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data)//Function to send data using USART
{ 
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr)//Function to turn send a string on USART
{ 
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}