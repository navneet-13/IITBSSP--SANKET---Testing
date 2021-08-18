#include <avr/io.h>
#define F_CPU 800000UL
#include <util/delay.h>


int main(void){
	
	//Initialize
	DDRA = 0b00001001;										
	DDRB = 0xFF;
	DDRC = 0xFF;
	TWAR = 0b10101100;
	ADCSRA = 1<<ADEN | 1<<ADPS2;
	char a = 0, b = 0;
	char dep_;
    
	while (1){
		
		
		TWCR = 1<<TWEN | 1<<TWEA;								//Waiting for command from master
		while(!(TWCR & 1<<TWINT))								//Twint Flag set after receiving Write Command
			;	
		while(!(TWSR == 0x60))
			;
		
		TWCR = 0<<TWEA | 1<<TWINT | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0x88))									//data has been received and Nack has been sent
			;
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;					//Unaddressed Slave
		_delay_ms(5);
		
		
		PORTA = 0b00000001;										//Enable current limiter for Burner Circuit
		_delay_ms(10);											
		
			
		ADMUX = 1<<ADLAR;										
		ADCSRA |= 1<<ADSC;										//Start ADC Conversion
		while(ADCSRA & (1<<ADSC))
			;
		a = ADCH;
		PORTB = a;												//First current Sensor Value
		
		ADMUX = 1<<ADLAR | 1<<MUX0;
		ADCSRA |= 1<<ADSC;
		while(ADCSRA & (1<<ADSC))
			;
		b = ADCH;
		PORTC = b;												//Second Current Sensor Value
		
		dep_ = ((PINA & 1<<PINA4) | (PINA & 1<<PINA5))>>4;		//Detection Switch status;
		
		
		PORTA |= 1<<PINA3;										//Tell AUX that HM data is ready to be sent
		
		
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xA8))
			;
		TWDR = (dep_);											//Deployment switch state sent
		
													
		TWCR  = 1<<TWEN | 1<<TWINT | 1<<TWEA;
			
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xB8))
			;
		TWDR = a;												//First Current Sensor Value sent
		
													
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xB8))
			;
		TWDR = b;												//Second Current Sensor Value Sent
		
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		while(!(TWCR & 1<<TWINT))
			;
		while(!(TWSR == 0xC0))
			;
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN;
    }
}

