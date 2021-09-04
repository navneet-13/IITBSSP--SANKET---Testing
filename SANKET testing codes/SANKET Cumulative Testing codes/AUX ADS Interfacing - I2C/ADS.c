#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void Init(){
	TWAR = 0b10101110;
	DDRA = 0xff;
	ADCSRA = 1<<ADEN | 1<<ADPS2 ;
}

char ADCConversion(int pin){									// start ADC conversion
	char value;													// pin - this variable decides which analogue pin will be used for conversion
	ADMUX = 1<<ADLAR | (char)(pin)<<MUX0;
	ADCSRA |= 1<<ADSC;
	while(ADCSRA & 1<<ADSC)
		;
	value = ADCH;
	return value;												//return 8-bit digital value
}

void checkStatusCode(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set after receiving Write Command
		;
	while(!(TWSR == data))
		;
}

void checkAndTransmit(char code, char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set
		;
	while(!(TWSR == code))										//Check for status code
		;
	TWDR = data;												//Write data to be transmitted in TWDR 
}

int main(void){
	Init();														//Initialize
	char expectedData = 0b10101100;								//Data expected to be received from AUX
	char receivedData ;				
	char defined_1 = 0b10100011;								//Pre-defined that is to be sent if received match expected data
	char curr_1;												//Variable for current sensor data
	char err_1 = 0x00000001;									//error message that is to be sent if received data do not match expected data
	
    while(1){
			
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;					//Enabled as slave
		checkStatusCode(0x60);									//Check if Slave is addressed or not by master
		
		TWCR = 1<<TWEA | 1<<TWINT | 1<<TWEN;					//Clear Twint Flag
		checkStatusCode(0x80);									//Check if data is received
		receivedData = TWDR;									//Read received data
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;					//Clear Twint Flag
		checkStatusCode(0xA0);									//Check if Repeated start command received
		
		curr_1 = ADCConversion(0);								//Start ADC conversion
		PORTA = 1<<PINA1;										//Make pinA1 high to tell the AUX to send read data command
		
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;					//clear Twint flag
		
		if( receivedData == expectedData){
			checkAndTransmit(0xA8, defined_1);					//if received data = expected data send pre-defined data after checking status code
			TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;				//Clear Twint flag
			
			checkAndTransmit(0xB8, curr_1);						//Check if previous data transmitted and then send current sensor data
			TWCR = 1<<TWEN | 1<<TWINT | 0<<TWEA;
		}
		

		
		else if( receivedData != expectedData){					
			checkAndTransmit(0xA8, err_1);						//if received data not equal to expected data send error message after checking status code
			TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;				//Clear twint flag
			
			checkAndTransmit(0xB8, err_1);						//Send error message again after checking status code
			TWCR = 1<<TWEN | 1<<TWINT | 0<<TWEA;				//Clear Twint flag and 
			}
		
		checkStatusCode(0xC0);									//check if last byte sent and nack is received from master
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;					//Slave is unaddressed
		PORTA = 0;
	}
	
}

