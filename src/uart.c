#include "uart.h"

u8 command_received;

u8 rxBuffer[20];

void uartPrint(char * string)
{
	while(*string)
	{
		while(!(UCA1IFG & UCTXIFG));
		UCA1TXBUF = *string++;
	}
}

u8 commandPending(void) 
{
	u8 ans = command_received;
	command_received = 0;
	return ans;
}

u8* getCommand(void)
{
	return rxBuffer;
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
		command_received = 1;
	
	__low_power_mode_off_on_exit();
}
