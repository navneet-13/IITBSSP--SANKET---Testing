#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void){
	char a;
	char b, c;
	DDRA = 0b11111100;
	DDRB = 0xFF;
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;
	
	ADCSRA = 1<<ADEN | 1<<ADPS2;
    while (1) {
		
		if(PINA == 0){
			a = 00;
			PORTB = 00;
			UDR0 = a;
			_delay_ms(10);
		}
		
		else if(PINA == 0b00000001){
			a = 0b01;
			PORTB = 0b01;
			UDR0 = a;
			_delay_ms(10);
		}
		
		
		else if(PINA == 0b000000010){
			a = 0b10;
			PORTB = 0b10;
			UDR0 = a;
			_delay_ms(10);
		}
		
		else if(PINA == 0b00000011){
			a = 0b11;
			PORTB = 0b11;
			UDR0 = a;
			_delay_ms(10);
		}
		
		ADMUX = 1<<ADLAR;
		ADCSRA |= 1<<ADSC;
		while(ADCSRA & 1<<ADSC)
			;
		b = ADCH;
		UDR0 = b;
		_delay_ms(10);
		
		ADMUX = 1<<ADLAR | 1<<MUX1;
		while(ADCSRA & 1<<ADSC)
			;
		c = ADCH;
		UDR0 = c;
		_delay_ms(10);
    
	
	}
	
	
}

