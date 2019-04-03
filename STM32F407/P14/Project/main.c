/**
  Projeto 14 - conexão de display OLED baseado no SSD1306 via I2C
	utiliza a biblioteca padrão STM32F4xxx
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "my_tick.h"
#include "hal_i2c.h"
#include "ssd1306.h"

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
i2c_handle_t hi2c1; 
 
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void gpio_i2c_init(void);
void i2c_init(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
/* Insert system clock initialization code here. */
	
	
/*Systick initalization */
  InitMyTick();

/* HW initialization */
  gpio_i2c_init();
  i2c_init();
	
/* OLED Display initialization */
	ssd1306_Init();
	

	
/*infinite loop */
	
		while (1)
		{ssd1306_Fill(Black);
		for(int j=0; j<63; j +=10){
		   ssd1306_SetCursor(2, j);
       ssd1306_WriteString("Hello World!", Font_7x10, White);
       ssd1306_UpdateScreen();
		   }
	  }
			 
}


/* initialization functions */
void gpio_i2c_init(void)
{
	GPIO_InitTypeDef gpio_init;
	
	/* GPIO Ports Clock Enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*pins PB6 (SCL1) PB7 (SDA1) */
	gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_Speed = GPIO_High_Speed;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init.GPIO_OType = GPIO_OType_OD;
	
  GPIO_Init(GPIOB, &gpio_init);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	
}

void i2c_init(void)
{
	_HAL_RCC_I2C1_CLK_ENABLE();
	
	hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_FM_DUTY_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRMODE_7BIT;
  hi2c1.Init.DualAddressMode = 0;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = 0;
  hi2c1.Init.NoStretchMode = I2C_ENABLE_CLK_STRETCH;
	
 	
	hal_i2c_init(&hi2c1);
		
	hi2c1.Instance->CR1 |= I2C_REG_CR1_ENABLE_I2C;
  
	
}

