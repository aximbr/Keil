/**
  Projeto 15 - conexão de display OLED baseado no SSD1306 via I2C
	utiliza a biblioteca padrão STM32F4xxx
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdio.h>
//#include "EventRecorder.h"


//#include <stdio.h>



/* Private macro -------------------------------------------------------------*/


#define DAC_PORT      GPIOA
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
	GPIO_InitTypeDef mygpio;
	mygpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	mygpio.GPIO_Mode = GPIO_Mode_OUT;
	mygpio.GPIO_Speed = GPIO_Low_Speed;
	mygpio.GPIO_OType = GPIO_OType_PP;
	mygpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_Init(DAC_PORT, &mygpio);
}

void DAC_out(uint16_t data){
	GPIO_Write(DAC_PORT, data);
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
	//EventRecorderInitialize(EventRecordAll, 1);
	sound_init();
	
	while(1) {
	x = wave[pos];
	printf("%i",0);
	//printf("wave[ %i ] = %i\n",pos, wave[pos]);
	//for(int j; j<1000; j++);

	}
}
