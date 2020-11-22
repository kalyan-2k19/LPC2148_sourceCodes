#include <LPC214X.H>
#include <stdint.h>

#define RX_STATUS			(1<<0)

void UART0_Init(void);
char UART0_RX(void);

void UART0_Init(void){
	PINSEL0 |= 0x05;			// 0000 0101 select Rx and Tx 
	
	U0FCR |= 0x07;   // Enable FIFO register and reset UART0 TX and RX
	U0LCR |= 0x83;	// DLAB =1, and 8bit word length selected.
	U0DLM = 1;
	U0DLL = 134;		// setting baudrate for 9600 bits/sec
	U0LCR = 0x03;		// Disable DLAB = 0.and set 8-bit word.
}
char UART0_RX(void){
	uint8_t rx_data;
	while(!(U0LSR & RX_STATUS));
	rx_data = U0RBR;
	return rx_data;
}



