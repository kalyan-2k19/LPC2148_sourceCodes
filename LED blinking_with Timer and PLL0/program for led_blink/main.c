#include <LPC214X.H>
#include "PLL.h"
#include "Timer.h"

#define led (1<<1)

int main(void){
	
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);
	Timer0_init_1us(DELAY_1us);
	PINSEL0 = 0;
	IO0DIR |= led;
	
	while(1){
		IO0SET = led;
		delay(1000);
		IO0CLR = led;
		delay(1000);
	}
	return 0;
}
	
