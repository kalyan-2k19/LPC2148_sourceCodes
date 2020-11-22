#include "UART_tx_rx.h"
#include "PLL.h"

int main(void){
	char data;
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);
	UART0_Init();
	while(1){
		data = UART0_RX();
		UART0_TX(data);
	}
}
