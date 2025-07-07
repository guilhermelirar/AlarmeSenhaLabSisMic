#include "uart.h"

u8 ready;

u8 rxBuffer[20];

void uartPrint(char * string)
{
	while(*string)
	{
		while(!(UCA1IFG & UCTXIFG));
		UCA1TXBUF = *string++;
	}
}

void uartConfig(void)
{	
	UCA1CTL1  = UCSWRST;		// RST a interface
	UCA1CTL1 |= UCSSEL__SMCLK;	// SM@2^20 Hz
	UCA1BRW   = 109;
	UCA1MCTL  = UCBRF_2;
	P4SEL    |= BIT4 | BIT5;
	UCA1CTL1 &= ~UCSWRST;
	UCA1IE = UCRXIE;

	__enable_interrupt();

	while(1)
	{
		while(!ready);
		ready = 0;

		uartPrint("recebido!\n\r");

	}
}

#pragma vector = 46 //USCI A1
__interrupt void uart_isr()
{
	static u8 i = 0;
	u8 c;
	
	c = UCA1RXBUF;
	UCA1TXBUF = c;
	rxBuffer[i++] = c;

	if(i==20) 
		i = 0;

	if(c == 0x0D)
		ready = 1;
	
}