#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void){
	DDRA = 0xFF;
	DDRB = 0xFF;
	ADCSRA |= 1<<ADEN | 1<<ADPS2; 
	ADMUX |= 1<ADLAR; 
	
	
	    while (1) {
		ADCSRA |= 1<<ADSC;
		while((ADCSRA & (1<<ADSC)))
			;
		char c = ADCH;
		PORTA = c;
		_delay_ms(500);
    }
}

