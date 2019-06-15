/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-05-07
  * @brief   Main program body
  ******************************************************************************
  ******************************************************************************
  */  


/* Includes ------------------------------------------------------------------*/
#include "tm4c123.h"
#include <stdio.h>

/* Private macro -------------------------------------------------------------*/
#define DAC_PORT      GPIOB
#define PINOS         (1U << 0) | (1U << 1) | (1U << 2) | (1U << 3) | (1U << 4)
#define NUM_ENT_TABLE 64

/* Private variables ---------------------------------------------------------*/
// 6-bit 64-element sine wave
const unsigned short wave[NUM_ENT_TABLE] = {
  32,35,38,41,44,47,49,52,54,56,58,
  59,61,62,62,63,63,63,62,62,61,59,
  58,56,54,52,49,47,44,41,38,35,
  32,29,26,23,20,17,15,12,10,8,
  6,5,3,2,2,1,1,1,2,2,3,
  5,6,8,10,12,15,17,20,23,26,29};

	uint8_t pos; //index of wave table
	short x;

/* Private function prototypes -----------------------------------------------*/
void DAC_init(void); //inicializa as portas digitais como saída
void DAC_out(uint16_t data); //saída digital do valor da tabela
void sound_init(void); //inicializa o systick
void sound_tone(uint32_t tone); //ajusta a frequência de saida mudando o tempo do Systick
void sound_off(void); //mute a saída

/* Private functions ---------------------------------------------------------*/
void DAC_init(void){
    
	SYSCTL->RCGC2 |= (1U << 1);      // Enable clock GPIOB
	uint32_t delay = SYSCTL->RCGC2;  //small delay
  GPIOB->AMSEL &= ~(PINOS);        // no analog
  GPIOB->PCTL &= ~0x00FFFFFFFF;    // regular function
  GPIOB->DIR |= PINOS;	           // define direction as output
  GPIOB->AFSEL &= ~(PINOS);        // disable alt funct on PB4-0
  GPIOB->DEN |= PINOS;             // Set the GPIODEN AH for Port F (Digital Function)
	GPIOB->DR8R |= PINOS;            // enable 8 mA drive on PB4-0
}

void DAC_out(uint16_t data){

	GPIOB->DATA = data;
}

void sound_init(void){
	DAC_init();
	uint32_t tone = SystemCoreClock/NUM_ENT_TABLE;
	tone = tone/880; //880 Hz is the fundamental freq.
	SysTick_Config(tone);
}

void sound_tone(uint32_t tone){
  SysTick_Config(tone);
}

void sound_off(void){
	DAC_out(0);
}

void SysTick_Handler(void){
	pos = (pos+1) & 0x3F;  //cycle count from 0 to 63
  DAC_out(wave[pos]);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
	sound_init();
	
	while(1) {
	x = wave[pos];
	//printf("wave[ %i ] = %i\n",pos, wave[pos]);
	for(int j; j<500; j++);

	}
	
}
