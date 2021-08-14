#include <avr/io.h>
#define F_CPU 8000000UL


int main(void){
	TWAR = 0b10101110;
	char a = 0b10101100;
	char b = 0;
	char defined_1 = 0b11111111;
	char curr_1 = 00;
	char err_1 = 0x00000001;
	ADCSRA = 1<<ADEN | 1<<ADPS2 ;
	ADMUX = 1<<ADLAR;
	
	
    while(1){
		
		
		TWCR = 1<<TWEN | 1<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x60))
			;
		
		TWCR = 0<<TWEA | 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x80))
			;
		b = TWDR;
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xA0))
			;
		
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xA8))
			;
		
		
		if(a==b){
			TWDR = defined_1;
			
			TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
			while(!(TWCR & 1<<TWINT))
				;
			while(!(TWSR == 0xB8))
				;
			
			ADCSRA |= 1<<ADSC;
			while(ADCSRA & 1<<ADSC)
				;
			curr_1 = ADCH;
			TWDR = curr_1;
			TWCR = 1<<TWEN | 1<<TWINT | 0<<TWEA;
			
			
			
		}
		
		else if( a != b){
			TWDR = err_1;
			TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
			
			while(!(TWCR & 1<<TWINT))
				;
			while(!(TWSR == 0xB8))
				;
			TWDR = err_1;
			TWCR = 1<<TWEN | 1<<TWINT | 0<<TWEA;
		}
		
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xC0))
			;
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
	
	}
	
}

