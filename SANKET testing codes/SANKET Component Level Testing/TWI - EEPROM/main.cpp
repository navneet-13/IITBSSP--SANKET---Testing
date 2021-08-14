#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
char a = 0x08;

int main(void){
	DDRA = 1;
	DDRE = 0xFF;
	
    while (1) {
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = 0b10100010;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x18))
			;
		TWDR = 0x11;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x28))
			;
		
		TWDR = 0x01;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x28))
			;
		
		a ^= 1;	
		TWDR = a;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x28))
			;
		
		PORTA ^= 1;
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTO;
		
		
		
		_delay_ms(500);	
			
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = 0b10100010;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x18))
			;
		
		TWDR = 0x11;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x28))
			;
		
		TWDR = 0x01;
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x28))
			;
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x10))
			;
		
		TWDR = 0b10100011;
		TWCR = 1<<TWINT | 1<<TWEN ;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x40))
			;
			
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		PORTE = TWDR;
		while(!(TWSR == 0x58))
			;
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTO;
		
		_delay_ms(1);
    }
}

