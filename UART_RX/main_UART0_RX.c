#include "UART_RX.h"
#include "PLL.h"
uint8_t data;
int main(void){
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);
	UART0_Init();
	while(1){
		
		data = UART0_RX();
		(void)data;
	}
	return 0;
}



