#include <LPC214X.H>
#include<stdint.h>

/*PLL types*/
#define PLL0 	0 
#define PLL1  1

// vpbdiv peripheral clock divisor of CCLK (cpu clock)
#define PLL0_15MHz_pclk_60MHz_cclk		15000000
#define PLL0_60MHz_pclk_60MHz_cclk		60000000
#define PLL0_30MHz_pclk_60MHz_cclk		30000000

void PLL_init(uint8_t PLLx, uint32_t clock);

/****************************************************************
   							PLL0 configuration 
 ****************************************************************/
 
 /*FOSC is in range of 10MHz to 25MHz external clock used for LPC2148
  *In our controller external clock is 12MHz.
	*CCLK (cpu clock is 10MHz to 60MHz(max clock))
	*FCCO ( Frequency of current controlled oscillator range from 156MHz to 360MHz)
	*/
	
	/*Registers is used for configure PLL*/
	/*1. PLL0FEED  Feed Sequence SFR
	  2. PLL0CON   PLL Control Register
		3. PLL0CFG   PLL Configure Register
		4. PLL0STAT  PLL Status Register
		5. VPBDIV    VLSI Peripheral Bus Divider*/
 
/*procedure to setup PLL*/
/*1. Setup PLL
	2. Apply Feed Sequence
	3. Wait for PLL to lock and then connect PLL
	4. Apply Feed Sequence.*/
	
	void PLL_init(uint8_t PLLx, uint32_t clock){
		
		if(PLLx == PLL0){
			
		PLL0CON |= 0x01; // enable PLLE bit in PLL0CON 
		
		/* CCLK = M x FOSC where FOSC is 12MHz, we need 60MHz cclk
		 * CCLK = 5 x 12MHz =60MHz. or M= CCLK/PCLK => 60MHz/12MHz =5.
		 *  M must be in the range of 1 to 32. The value written to the MSEL bits in PLLCFG is M-1
		
		 *  MSEL[4:0] = M-1 =>5-1 = 4. Load 4 value written to MSEL bits in PLL0CFG register. MSEL[4:0]=00100; 
		
		 * calculate 'P' value , PLL Divider value from the PSEL bits in the PLLCFG register
		 * FCCO = CCLK x 2 x P => p =FCCO/2xCCLK =>156M/2x60M =1.3
															p = 320M/2*60M => 2.66
		weget'P[6:5]' value is '2'[01] in reference manual. P[6:5]=01;
		7th bit reserved.
		so PLL0CFG |=0x24;*/
		
		  
		PLL0CFG |= 0x24;
		
		//Apply feed sequene
		PLL0FEED = 0xAA;						// these values from reference manual of PLLOFEED register
		PLL0FEED = 0x55;
		//check PLL status register of PLL0STAT
		while((PLL0STAT &0x400) ==0);//When one, the PLL is locked onto the requested frequency.
		PLL0CON |=0x03;// PLL connect 
		PLL0FEED = 0xAA;						// these values from reference manual of PLLOFEED register
		PLL0FEED = 0x55;
		
		if(clock == PLL0_15MHz_pclk_60MHz_cclk){
			VPBDIV = 0x00;
		}
		else if(clock == PLL0_60MHz_pclk_60MHz_cclk){
			VPBDIV = 0x01;}
		
		else if(clock == PLL0_30MHz_pclk_60MHz_cclk){
			VPBDIV = 0x02;
		}
	}
}		
	
