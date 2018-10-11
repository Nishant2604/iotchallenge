#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

#define Baud_Rate 9600UL
#define ubrr_value ((F_CPU/(16UL*Baud_Rate)) - 1)


void usart_init();
void usart_send(unsigned char);
unsigned char usart_rec();
void adc_init();
unsigned int adc_read(unsigned char channel);

int main()
{	
	unsigned char i=0;
	DDRB = 0XFF;
	DDRA = 0x00;

	//lcd_init();
	usart_init();
    adc_init();
	
	while(1)
	{
	DDRA  =	0b00000110;
	PORTA = 0B00001010;

    int x= adc_read(0);


	if (x >= 800 )
	{
	PORTB=0b00001111;
	}
	if (x < 800 )
	{
	PORTB=0b11110000;
	}
	return 0;
}
}
void usart_init()
{

   //Set Baud rate
   UBRRL = ubrr_value;
   UBRRH = (ubrr_value>>8);


   UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
   

   //Enable The receiver and transmitter

   UCSRB=(1<<RXEN)|(1<<TXEN);

}
////////////////////////////////////////////////
void usart_send(unsigned char data)
{
  //Wait until the transmitter is ready

   while(!(UCSRA & (1<<UDRE)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR=data;
}
////////////////////////////////////////////////
unsigned char usart_rec()
{
 //Wait untill a data is available

   while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR;
}
void adc_init()
{
	ADMUX = 0B01000000;//(1<<REFS0);// | (1<<REFS1);
	ADCSRA = ((1<<ADEN) | (1<<ADPS2)| (1<<ADPS1)| (1<<ADPS0));
	}
unsigned int adc_read(unsigned char channel)
{	
	
	ADMUX = 0x40|channel; 
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));

	ADCSRA |= (1<<ADIF);
	return ADC;
}
