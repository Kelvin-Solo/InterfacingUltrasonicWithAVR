#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<util/setbaud.h>
#include<stdlib.h>
#include<stdio.h>

static volatile uint16_t pulse = 0;
//static volatile size_t i = 0;

void InitUSART(void){               /*initialize USART*/
	UBRRH = UBRRH_VALUE;			/*setting baud rate*/
	UBRRL = UBRRL_VALUE;
#if USE_2X
	UCSRA |= (1 << U2X);
#else
	UCSRA &= ~(1 <<U2X);
#endif
	
	UCSRB = (1 << RXEN) | (1 << TXEN); /*enable receiver and transmitter*/

	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); /*set frame format; 8data, 1stop bit*/
}

void USART_Transmit(char data){
    while(!(UCSRA & (1<<UDRE)));
    UDR = data;
}

void printString(char myString[]){      /*print on serial monito*/
    size_t i=0;
    while(myString[i] !=0){
            USART_Transmit(myString[i]);
            i++;
    }   
}

void InitInterrupt0(void){
        GICR |= (1<<INT0);
        MCUCR |= (1<<ISC00);
        sei();
}

ISR(INT0_vect){					
	if((PORTD |= (1 << PIND2))== 1){
		TCCR1B |= (1 << CS10);	/*start timer when echo pin goes high*/
	}

	if((PORTD &= ~(1 << PIND2))== 0){
		TCCR1B = 0;				/* stop timer when echo pin goes low*/
		pulse = TCNT1;			/* store value of time in pulse*/
		TCNT1 = 0;
	}

}

int main(void)
{
	DDRD = 0b11111011;

	uint16_t distance = 0;
	char data[16];

	InitInterrupt0();
	InitUSART();

	while(1){
		PORTD |= (1 << PIND3);
		_delay_ms(15);
		PORTD &= ~(1 << PIND3);

		distance = pulse/58;		/*get distance in cm*/

		itoa(distance, data, 10);

		printString("\r\n Distance: ");
		printString(data);
		printString(" cm. \r\n");
	}

	return 0;
}

