#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

char a = 0;


int main(void){
	sei();
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;
	
	ADCSRA |= 1<<ADEN | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1;
	ADMUX |= 1<<ADLAR;
	DDRA = 0xFF;
	PORTA = 0;
	
    while (1) {
		_delay_ms(100);
		ADCSRA |= 1<<ADSC ;
		_delay_ms(10);
		
    }
}

ISR(ADC_vect){
	
	a = ADCH;
	PORTA = a;
	UDR0 = a;
}