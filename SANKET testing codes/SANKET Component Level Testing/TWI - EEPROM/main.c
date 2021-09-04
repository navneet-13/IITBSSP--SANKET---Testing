#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
char data = 0x08;												//Variable data to be sent to eeprom

void Init(){
	DDRA = 1;
	DDRE = 0xFF;
}
	
void checkStatusCode(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set 
		;
	while(!(TWSR == data))										//check status code
		;
}

void I2CTransmit(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set
		;
	TWDR = data;												//write data in twdr register
}
int main(void){
	
	Init();
	
    while (1) {
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;					//Start command sent
		checkStatusCode(0x08);
		
		I2CTransmit(0b10100010);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x18);
		
		I2CTransmit(0x11);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x28);
		
		I2CTransmit(0x01);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x28);
		data ^= 1;												//change the variable data to be sent to eeprom
		I2CTransmit(data);
		
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x28);
		
		PORTA ^= 1;												//Toggle PINA0
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTO;					//Stop command sent
		
		
		
		_delay_ms(500);	
			
		
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;					//Start command sent
		checkStatusCode(0x08);
		
		I2CTransmit(0b10100010);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x18);
		
		I2CTransmit(0x11);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x28);
		
		I2CTransmit(0x01);
		TWCR = 1<<TWINT | 1<<TWEN;								//Twint Flag cleared
		checkStatusCode(0x28);
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;					//start command sent
		checkStatusCode(0x10);
		
		I2CTransmit(0b10100011);
		TWCR = 1<<TWINT | 1<<TWEN ;								//Twint Flag cleared
		checkStatusCode(0x40);
			
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		checkStatusCode(0x58);
		PORTE = TWDR;											//reading received data
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTO;					//Stop command set
		
		_delay_ms(1);
    }
}

