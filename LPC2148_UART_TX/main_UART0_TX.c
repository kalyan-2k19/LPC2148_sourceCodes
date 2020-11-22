#include "PLL.h"
#include "UART_TX.h"

int main(void){
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);
	UART0_Init();
	
	while(1){
		UART0_TX_String("Hello world\n\r");
		UART0_TX_String("My project is success\n\r");
	}
	return (0);
}




