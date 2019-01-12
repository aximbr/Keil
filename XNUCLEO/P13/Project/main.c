/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-01-09
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * P13 - conexão ao MultiShield para utilizar o display de 8 segmentos
  *
  * <h2><center>&copy; COPYRIGHT JPL</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdint.h>
#include <stdbool.h>
#include "drv-clock.h"

/* Private macro -------------------------------------------------------------*/
#ifndef _BV
#define _BV(n) (1 << n)
#endif

/* Private variables ---------------------------------------------------------*/
#define LATCH_PORT  (GPIOA)
#define CLK_PORT    (GPIOA)
#define DATA_PORT   (GPIOA)

#define LATCH_PIN    (GPIO_Pin_5)  //D4 <-> PA5
#define CLK_PIN      (GPIO_Pin_6)  //D7 <-> PA6
#define DATA_PIN     (GPIO_Pin_7)  //D8 <-> PA7

#define bitSet(_pin)    GPIO_SetBits(GPIOA, _pin)
#define bitClear(_pin)  GPIO_ResetBits(GPIOA, _pin)

#define REFRESH 1000  //Taxa de refresh ou base de tempo do contador (ms)
 
/* Global variables ----------------------------------------------------------*/
int volatile tick_count=0;  //contador de eventos
/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

/* Private function prototypes -----------------------------------------------*/
void WriteNumberToSegment(uint8_t Segment, uint8_t Value);

/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void) 
{
	tick_count++;
}


/* Private functions ---------------------------------------------------------*/
/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value)
{
	uint8_t x;
	bitClear(LATCH_PIN);

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_MAP[Value] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_SELECT[Segment] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	bitSet(LATCH_PIN);
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 GPIO_InitTypeDef GPIO_pin_config;
	
/* Insert system clock initialization code here. */
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  SetSysClock();
	
	
/* initialise  Systick  for 1 ms */
	SysTick_Config(SystemCoreClock / 1000);
	
 
/* Initiate GPIO Data Strutcute */
	GPIO_StructInit(&GPIO_pin_config);
	GPIO_pin_config.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_pin_config.GPIO_Mode = GPIO_Mode_Out_PP;
	
/* Initiate GPIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_pin_config);
	
tick_count = 0;    //reset tick count
/*infinite loop */
	
		while (1)
		{
			for (int m=0; m<6; m++) {
				for (int c=0; c<10; c++) {
					for (int d=0;d<6;d++) {
						for (int u=0;u<10;u++) {
							while(tick_count < REFRESH){   //keeping display the numbers before increment a unit 
								WriteNumberToSegment(0 , m);
								WriteNumberToSegment(1 , c);
								WriteNumberToSegment(2 , d);
								WriteNumberToSegment(3 , u);
								
							}
							tick_count = 0;   //reset tick count 
						}
					}
				}
			} 
		}
	}
