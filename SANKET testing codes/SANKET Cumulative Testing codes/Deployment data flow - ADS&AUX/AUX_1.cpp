#include <avr/io.h>
#define F_CPU 800000UL
#include <util/delay.h>


int main(void){
	DDRA = 0x00000001;
	DDRB;
	int ubrr = 51;
	char add_  = 0b10101100;
	char comm_ = 0b11001100;
	char a,b,c; 
	UBRR0H = char(ubrr>>8);
	UBRR0L = char(ubrr);
	
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0C = 1<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00;

    while (1){
		
		PORTA = 1;
		_delay_ms(100);
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTA;			//START
		
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = add_;									//ADDRESS of Slave
		
		TWCR = 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x18))
			;
		TWDR = comm_;									//Deployment Command
		
		TWCR = 1<<TWEN | 1<<TWINT;
		while(!!TWCR & 1<<TWINT)
			;
		while(!(TWCR & 1<<TWINT))
			;
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTO;			//Stop TWI Communication
		
		while(!(PINA & 1<<PINA1))						//Checking for confirmation from ADS to Start TWI Communication
			;
		
		TWCR = 1<<TWSTA | 1<<TWEN | 1<<TWINT;			//Start
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x08))
			;
		TWDR = 0b10101101;								//Read Command
		
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
		a = TWDR;										//Deployment Switch State data
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x50))
			;
		b = TWDR;										//Current Sensor 1 Data
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 0<<TWEA;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x58))
			;
		c = TWDR;										//Current Sensor 2 data
		
		TWCR = 1<<TWSTO | 1<<TWINT | 1<<TWEN;			//Stop
		
		
		
		//Displaying Received data on portb
		PORTB = a;
		_delay_ms(200);
		PORTB = b;
		_delay_ms(200);
		PORTB = c;
		_delay_ms(200);
		
    }	
	
}

