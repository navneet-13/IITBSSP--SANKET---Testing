#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void){
	DDRA = 0xFF;
	DDRB = 0xFF;
	_delay_ms(50);
	PORTA = 1;
	
    while (1) {
		_delay_ms(2000);
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = 0b10101110;
		
		
		
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x18))
			;
		TWDR = 0b10101100;
		
		
		
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x30))
			;
		TWCR = 1<<TWINT | 1<<TWSTO | 1<<TWEN;
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = 0b10101111;
		
		
		
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x40))
			;
		
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x50))
			;
		PORTA = TWDR;
		
		
		
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR = 0x58))
			;
		PORTB = TWDR;
		
		
		
		TWCR = 1<<TWSTO | 1<<TWINT;	
	}
	
}

