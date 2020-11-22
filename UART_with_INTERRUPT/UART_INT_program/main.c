#include "PLL.h"

// UART functions.
void UART0_Init(void);				// UART0 initialization function.
void UART0_Interrupt(void); // UART0 interrupt configuration function
void UART0_ISR(void)__irq; // UART0 ISR function 

int main(void){
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);		// Initialize the PLL0 for 60MHz clock
	UART0_Init();																// initialize the UART0
	UART0_Interrupt();													// UART0 interrupt initialization.
	
	while(1){
		// Background task , when no interrupt generated.
	}
}


// UART0 initialization definition
void UART0_Init(void){
	PINSEL0 |= 0x05;			// 0000 0101 select Rx and Tx 
	
	U0FCR |= 0x07;   // Enable FIFO register and reset UART0 TX and RX
	U0LCR |= 0x83;	// DLAB =1, and 8bit word length selected.
	U0DLM = 1;
	U0DLL = 134;		// setting baudrate for 9600 bits/sec
	U0LCR = 0x03;		// Disable DLAB = 0.and set 8-bit word.
}

// UART0 interrupt definition
void UART0_Interrupt(void){
	VICIntEnable = (1<<6); // Enable UART0 interrupt.
	VICVectCntl0 = (0x20)| 6; // select slot0 for UART0 interrupt.
	VICVectAddr0 = (unsigned)UART0_ISR; // assign ISR address to corresponding slot address register.
	U0IER = 0x03;											// Enable UART0 rx interrupt.
}
// UART0 Interrupt Service Routine function.
void UART0_ISR(void)__irq
{
	long int temp;
	uint8_t data;
	temp = U0IIR;						// read the Interrupt identification register data for status information.
	if(temp & (0x04)){      // check the RX data
		data = U0RBR;			    // read the receiving data
		U0THR = data;					// transmitt the data (single character).
	}
	VICVectAddr = 0; // dummy write to this register .

	}

	
	