#include <LPC214X.H>
#include <stdint.h>
#include <stdio.h>


void LCD_Init(void);
void LCD_send_CMD(uint8_t cmd);
void LCD_send_Data(uint8_t data);
void LCD_send_String(uint8_t *data);
void LCD_send_Interger(uint32_t *number, uint16_t Len);

/* LCD 4-bit mode
 *VSS---->GND,
 * VDD---->+5V,
 * VEE---->GND or connected to 10k potentiomer ouput,
 * RS----->P0.0 of LPC2148
 * EN----->P0.1 of LPC2148


