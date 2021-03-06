#include <stdio.h>
#include "stm32f407xx.h"
#include "../Inc/GPIO_driver.h"
#include "../Inc/MAX7219.h"

MAX7219_Pin MAX7219 = {GPIOA, 4, 5, 7};

const uint8_t characterLib[43][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0 
{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},//1 
{0x7E,0x02,0x02,0x7E,0x40,0x40,0x40,0x7E},//2 
{0x3E,0x02,0x02,0x3E,0x02,0x02,0x3E,0x00},//3  
{0x08,0x18,0x28,0x48,0xFE,0x08,0x08,0x08},//4 
{0x3C,0x20,0x20,0x3C,0x04,0x04,0x3C,0x00},//5 
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x00},//6 
{0x3E,0x22,0x04,0x08,0x08,0x08,0x08,0x08},//7  
{0x00,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8 
{0x3E,0x22,0x22,0x3E,0x02,0x02,0x02,0x3E},//9 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//:
{0,0,0,0,0,0,0,0},//;
{0,0,0,0,0,0,0,0},//<
{0,0,0,0,0,0,0,0},//=
{0,0,0,0,0,0,0,0},//>
{0,0,0,0,0,0,0,0},//?	
{0,0,0,0,0,0,0,0},//@
{0x08,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A 
{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x00},//B 
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x00},//C 
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x00},//D 
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E 
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F 
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x08,0x08,0x08,0x08,0x08,0x48,0x30},//J
{0x00,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x00,0x42,0x62,0x52,0x4A,0x46,0x42,0x00},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x00,0x1E,0x20,0x20,0x3E,0x02,0x02,0x3C},//S
{0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x00,0x49,0x49,0x49,0x49,0x2A,0x1C,0x00},//W
{0x00,0x41,0x22,0x14,0x08,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x08,0x08,0x08,0x08,0x08},//Y
{0x00,0x7F,0x02,0x04,0x08,0x10,0x20,0x7F},//Z
};

volatile uint32_t ticks;

void delay(volatile uint32_t time)
{
  ticks = time;
  while(ticks !=0);
} 

void SysTick_Handler(void){
	if(ticks !=0)
	{
		ticks --;
	}	
}


void setup()
{
	/* COnfigure MAX7219 pins: PA7 as MOSI, PA4 as CS and PA5 as CLK */
	GPIOInit(GPIOA, 4, GPIO_MODER_OUTPUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_HIGH_SPEED, GPIO_PUPDR_NOPULL, GPIO_AFR_NO_ALTERNATE_FUNCTION);
	GPIOInit(GPIOA, 5, GPIO_MODER_OUTPUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_HIGH_SPEED, GPIO_PUPDR_NOPULL, GPIO_AFR_NO_ALTERNATE_FUNCTION);
	GPIOInit(GPIOA, 7, GPIO_MODER_OUTPUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_HIGH_SPEED, GPIO_PUPDR_NOPULL, GPIO_AFR_NO_ALTERNATE_FUNCTION);	
  delay(500);
  init_MAX7219();
}

void writeChar(uint8_t charC){
	uint8_t i;
	for(i = 1; i < 9; i++)
		write_MAX7219(i, characterLib[charC - '0'][i-1]);
		delay(500);
}

void writeString(uint8_t str[], uint8_t len){
	uint8_t i;
	for(i = 0; i < len; i++){
		writeChar(str[i]);		
	}
}

int main(void) {
	uint8_t str[5] = {'H', 'A', 'P', 'P', 'Y'};	
	
	/* Configure SysTick interrupt every 1ms */
	SysTick_Config(SystemCoreClock/1000);
	
	setup();	
	
	while(1){
		writeString( str, 5);		
	}
}
