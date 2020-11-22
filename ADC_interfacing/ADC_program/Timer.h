#include <LPC214X.H>
#include<stdint.h>
#define DELAY_1MS			60000     /*    cclk=60000000 Hz, PR(prescalar value)=60000-1, time we get 1ms i.e; 60000/60MHz =1ms*/
#define DELAY_1SEC		60000000	/* PR=60000000-1 for 1 second */
#define DELAY_1us			60					// PR=60-1 for 1 micro second1

void Timer0_init_1sec(uint32_t delay);
void Timer0_init_1Ms(uint32_t delay);
void Timer0_init_1us(uint32_t delay);
void delay(uint32_t delay);

//initialze the timer with 1 second delay. when cclk is 60MHZ.
void Timer0_init_1sec(uint32_t delay){
	//select timer mode
	T0CTCR =0x00;
	//Load the prescalar value in T0PR register
	T0PR =delay-1;
	// reset the Timer counter and prescar counter when bit-1 is set.
	T0TCR = 0x02;
}

//initialize the timer with 1 milli second delay.when cclk is 60MHZ
void Timer0_init_1Ms(uint32_t delay){
	//select timer mode
	T0CTCR =0x00;
	//Load the prescalar value in T0PR register
	T0PR =delay-1;
	// reset the Timer counter and prescar counter when bit-1 is set.
	//T0TCR = 0x02;
}
	
// initialize the timer with 1us delay.when cclk is 60MHZ
void Timer0_init_1us(uint32_t delay){
	//select timer mode
	T0CTCR =0x00;
	//Load the prescalar value in T0PR register
	T0PR =delay-1;
	// reset the Timer counter and prescar counter when bit-1 is set.
	T0TCR = 0x02;
}
	
void delay(uint32_t delay){
	//set the timer counter (TC) to 0
	T0TC =0;
	// reset the Timer counter and prescar counter when bit-1 is set.
	T0TCR = 0x02;
	T0TCR =0x01; // enable the timer0
	
	while(T0TC < delay); // wait until PR+1 (when TC incremented by one PR+1)
	T0TCR = 0x00;
	
}


