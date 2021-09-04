#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void Init(){
	DDRA = 0xff;
	DDRB = 0xFF;
	DDRC = 0;
	
}

void checkStatusCode(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set after receiving Write Command
	;
	while(!(TWSR == data))
	;
}

void I2CTransmit(char data){
	while(!(TWCR & 1<<TWINT))									//Twint Flag set
		;
	TWDR = data;
}
int main(void){
	Init();														//Initialize
	char slaveAddress = 0b10101110;								//Slave Address
	char msg_1 = 0b10101100;									//message to be sent to ADS(Slave)
    char receivedData_1;
	char receivedData_2;
	while (1) {
		_delay_ms(200);
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWSTA;					//Start command sent
		checkStatusCode(0x08);									//Check status if start has been sent
		
		I2CTransmit(slaveAddress | 0);							//Slave address with write command
		TWCR = 1<<TWINT | 1<<TWEN;								//Clear Twint flag
		checkStatusCode(0x18);									//Check status code
		
		I2CTransmit(msg_1);										//Send the predefined message
		TWCR = 1<<TWINT | 1<<TWEN;								//Clear Twint Flag
		checkStatusCode(0x28);									//check status code if data is sent and ACK is receied
		
		TWCR = 1<<TWINT | 1<<TWSTA | 1<<TWEN;					//send repeated start condition
		
		while(!(PINC & 1<<PINC0))								//Wait for ADS to give the signal to start reading data
			;
		
		checkStatusCode(0x10);									//Check status code if repeated start is sent
		
		I2CTransmit(slaveAddress | 1);							//Send Slave address with read command
		TWCR = 1<<TWINT | 1<<TWEN;								//Clear twint flag
		checkStatusCode(0x40);									//Check status code if SLA+R is sent and ACK is received
		
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;					
		checkStatusCode(0x50);
		receivedData_1 = TWDR;									//Read 1st received data
		PORTA = receivedData_1;									//Display the data on portA
		
		TWCR = 1<<TWINT | 0<<TWEA | 1<<TWEN;
		checkStatusCode(0x58);
		receivedData_2 = TWDR;									//Read 2nd received data
		PORTB = receivedData_2;									//Display the data on portB
		
		TWCR = 1<<TWSTO | 1<<TWINT | 1<<TWEN;					//Stop command sent
	}
	
}

