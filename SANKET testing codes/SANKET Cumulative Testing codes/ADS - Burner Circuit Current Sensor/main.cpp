#include <avr/io.h>
#define F_CPU 8000000UL
# include <util/delay.h>
int main(void){
	char a;
	DDRA = 0xFF;
	int ubrr = 51;
	UBRR0H = (char)(ubrr>>8);
	UBRR0L = (char)(ubrr);
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;
	
	
	ADCSRA = 1<<ADEN | 1<<ADPS2;
	ADMUX = 1<<ADLAR;

    while (1){
		_delay_ms(500);
		ADCSRA |= 1<<ADSC;
		while(ADCSRA & 1<<ADSC)
			;
		a = ADCH;
		PORTA  = a;
		UDR0 = a;
		_delay_ms(10);
		
    }
}

