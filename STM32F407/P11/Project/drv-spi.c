/* Driver para SPI usando STM32F407ZG */
#include "main.h"
#include "drv-spi.h"
#include "stm32f4xx_spi.h"

void spi_init(void){
	GPIO_InitTypeDef SPI_Pin_Init, RST_DC_Pin_Init;
	SPI_InitTypeDef SPI_InitStructure;
	
	//Enable clock to SPI1 and to GPIO for SPI pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 0;
	
	/* SPI pins MOSI = PA7  CLK = PA5  SS = PA4 */
	SPI_Pin_Init.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7; //here we can use multiple Pins
	SPI_Pin_Init.GPIO_Mode = GPIO_Mode_AF;
	SPI_Pin_Init.GPIO_Speed = GPIO_Speed_100MHz;
	SPI_Pin_Init.GPIO_OType = GPIO_OType_PP;
	SPI_Pin_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1); //only one Pin each time. Pin definition is different!!!
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	GPIO_Init(GPIOA, &SPI_Pin_Init);
			
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI1, ENABLE); //necessary for hardware SS pin on SPI peripheric
	
	/* RST = PE4, DC = PE5 (using GPIO) */
	RST_DC_Pin_Init.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	RST_DC_Pin_Init.GPIO_Mode = GPIO_Mode_OUT;
	RST_DC_Pin_Init.GPIO_Speed = GPIO_Speed_100MHz;
	RST_DC_Pin_Init.GPIO_OType = GPIO_OType_PP;
	RST_DC_Pin_Init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &RST_DC_Pin_Init);
	
		
}

void RST_reset(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	for(int j=0; j<1000; j++);
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
	for(int j=0; j<3000; j++);
}

void spi_write_byte( uint8_t data, uint8_t dc){
	SPI_Cmd(SPI1, ENABLE);
	
	if (dc) GPIO_SetBits(GPIOE, GPIO_Pin_5); //DC 1 -> data DC 0 -> command
	else GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	
	while(!(SPI1->SR & SPI_SR_TXE)){}; // wait unti buffer empty
  SPI1->DR = data; // TX character
	
	SPI_Cmd(SPI1, DISABLE);	
}

