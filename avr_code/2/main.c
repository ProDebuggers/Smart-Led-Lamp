 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/delay.h>

#define UBRRVAL 103

void usart_init(void);
void sendbyte(unsigned char);
void sendstr(unsigned char *);
unsigned char receivebyte(void);
void receivestr(unsigned char*);

unsigned char onmsg[] = "ON\n";	
unsigned char offmsg[] = "OFF\n";
unsigned char defaultmsg[] = "LED Status:\n";
unsigned char rxdata;

int main(){ 
	
	DDRB |= (1<<PORTB0);
	//PORTB |= (1<<PORTB0);	
	//_delay_ms(1000);
	//PORTB &= ~(1<<PORTB0);	
	usart_init();	
	
	
	while(1)
	{
		rxdata = receivebyte();
		
		if((char)rxdata == 'y')
		{
			PORTB |= (1<<PORTB0);	
			sendstr(defaultmsg); 
			sendstr(onmsg);
			
		}
		else if((char)rxdata =='n')
		{
			PORTB &= ~(1<<PORTB0);
			sendstr(defaultmsg); 
			sendstr(offmsg); 
		}
		else{
		
			sendbyte(rxdata);
				PORTB |= (1<<PORTB0);
				_delay_ms(1000);
				PORTB &= ~(1<<PORTB0);
			
		}
	}
   
   return 0;
 }
 
void usart_init(void){
	UBRR0H= (unsigned char)(UBRRVAL>>8);   //high byte
    UBRR0L=(unsigned char)UBRRVAL;     			//low byte
   // UCSR0B |= (1<<TXEN0) | (1<<RXEN0);		//Enable Transmitter and Receiver
    //UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); 
	  UCSR0B |= (1<<TXEN0) | (1<<RXEN0);		//Enable Transmitter and Receiver
	 UCSR0C = 0x06;	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
}

void sendbyte(unsigned char MSG){
    while((UCSR0A&(1<<UDRE0)) == 0);     // Wait if a byte is being transmitted
    UDR0 = MSG; 
}

void sendstr(unsigned char *s){
	unsigned char i = 0;
	while(s[i] != '\0'){
	 sendbyte(s[i]);
	 i++;
	 }
}

unsigned char receivebyte(void){
	while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
}