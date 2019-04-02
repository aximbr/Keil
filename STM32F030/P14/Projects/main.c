/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ssd1306.h"

/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void i2c_init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
//Enable Systick
	InitMyTick();
	
//Enable I2C peripheric
	i2c_init();
	
//Init Display
	ssd1306_Init();
	
  
  /* Infinite loop */
  while (1)
  {
   ssd1306_Fill(Black);
		for(int j=0; j<63; j +=10){
		   ssd1306_SetCursor(2, j);
       ssd1306_WriteString("Hello World!", Font_7x10, White);
       ssd1306_UpdateScreen();
		   }
	}
}

void i2c_init(void)
{
	GPIO_InitTypeDef my_gpio_pin;
	my_gpio_pin.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	my_gpio_pin.GPIO_Mode = GPIO_Mode_AF;
	my_gpio_pin.GPIO_OType = GPIO_OType_OD;
	my_gpio_pin.GPIO_PuPd = GPIO_PuPd_UP;
	my_gpio_pin.GPIO_Speed = GPIO_Speed_Level_3;
		
	I2C_InitTypeDef my_i2c_init;
	I2C_StructInit(&my_i2c_init);
	//set SCLH and SCLL on TIMINGR
	my_i2c_init.I2C_Timing = 0x2000090E; //I2C at 100Khz with Clock at 8Mhz
	my_i2c_init.I2C_Ack = I2C_Ack_Enable;
	
	//enable clock on device
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	//configure pins for I2C functions
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);  //I2C1_SCL
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4); //I2C1_SDA

	GPIO_Init(GPIOA, &my_gpio_pin);
	
	//configure i2c
	I2C_Init(I2C1, &my_i2c_init);
	I2C_AutoEndCmd(I2C1, ENABLE);

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
