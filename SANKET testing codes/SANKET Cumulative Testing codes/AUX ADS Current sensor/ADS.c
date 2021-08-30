#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

char currSensor;												//Variable for current sensor value
void Init(){
	DDRA = 0b11111111;											//Set I/O pins for PORT A
	int ubrr = 51;												//ubrr value
	UBRR0H = (char)(ubrr>>8);										//set ubrr value in registers
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN | 1<<TXEN;									//Enable receive and transmit for USART
	UCSR0C = 1<<UCSZ00 | 1<<UCSZ01 | 1<<USBS0;					//Select number of stop bit and bit size of data
	
	ADCSRA = 1<<ADEN | 1<<ADPS2 | 1<<ADIE;								//Enable ADC and set Prescaler
}


void USARTTransmit(char data){
	while(!(UCSR0A & 1<<UDRE0))									//Transmit data suing USART
	;
	UDR0  = data;
	_delay_ms(10);
	
}

void ADCConversion(int pin){									// start ADC conversion
																// pin - this variable decides which analogue pin will be used for conversion
	ADMUX = 1<<ADLAR | (char)(pin)<<MUX0;
	ADCSRA |= 1<<ADSC;
}

int main(void){
	sei();
	Init();
	PORTA = 1;
    while (1) {
		ADCConversion(0);
    }
}

ISR(ADC_vect){
	
	currSensor = ADCH;
	PORTA = currSensor;
	USARTTransmit(currSensor);
}