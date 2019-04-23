/*
 P15 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-04-23
 * Keil v5                                                        */

/* Includes *******************************************************/
#include "MKL25Z4.h"
#include "mcg.h"
#include <stdint.h>
#include "ssd1306.h"
#include "HMC5883L.h"
#include "conv_to_string.h"
#include "FreeRTOS.h."
#include "task.h"
#include "queue.h"
#include "semphr.h"



/* Global variables */

/* Defines ********************************************************/

#define ICR   0x0D    //Bus @ 48Mhz I2C baud rate 1MHz, see datasheet at pp706


/* Private macro -------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t xMutex; //Semaphore to cotrol access to I2C bus
xQueueHandle MyLeitura;  //fila para transferir dados de uma task para outra


/* Private function prototypes -----------------------------------------------*/
void vSetup(void *pvParameters);
void vReadHMC5883(void *pvParameters);
void vDisplayResult(void *pvParameters);


/* Private functions ---------------------------------------------------------*/
void vSetup(void *pvParameters){
/*This task takes care of initialize the hardware, and run only once */
//Test the connection
  ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 30);
		if ( HMC5883L_TestConnection() )
		   ssd1306_WriteString("HMC init OK!", Font_7x10, White);
		else
			  ssd1306_WriteString("HMC init NOK!", Font_7x10, White);
	ssd1306_UpdateScreen();	
  vTaskDelay( pdMS_TO_TICKS(3000));
		
//Create Tasks
  xTaskCreate(vReadHMC5883, "Read HMC5883", 100, NULL, 1, NULL);
  xTaskCreate(vDisplayResult,"Display Result", 100, NULL, 2, NULL);
		
//Delete yourself
  vTaskDelete( NULL );
}

void vReadHMC5883(void *pvParameters){
/*This tasks reads data on HMC5883L device via I2C Bus */
int16_t leitura[3];
const TickType_t xDelay = pdMS_TO_TICKS(200);
	
while (1){

		//try access I2C Bus
    xSemaphoreTake(xMutex, xDelay);
		HMC5883L_GetHeading( leitura );
			
		//update Queue			
		xQueueSendToBack( MyLeitura, &leitura, xDelay);
	
	  //release resource
		xSemaphoreGive(xMutex);
		    
   } 
}

void vDisplayResult(void *pvParameters){
/*This tasks display the results on OLED display via I2C Bus */	
int16_t resultado[3];
char zz[10];
const TickType_t xDelay = pdMS_TO_TICKS(50);
	
    while (1){
  
	  //read queue
		xQueueReceive(MyLeitura, &resultado, xDelay);
	
	  //try access I2C Bus
    xSemaphoreTake(xMutex, xDelay);
	  	  
	  //display
	  ssd1306_Fill(Black);
	  ssd1306_DrawBox(2, 2, 129, 62);
	  ssd1306_DrawHLine(2, 129, 18);
	  ssd1306_SetCursor(20, 8);
	  ssd1306_WriteString("HMC5883L by JPL", Font_7x10, White);
		ssd1306_SetCursor(5, 30);
		ssd1306_WriteString("X : ", Font_7x10, White);
		ssd1306_SetCursor(33, 30);
		ssd1306_WriteString(itoa(resultado[0], zz, 10), Font_7x10, White);
		ssd1306_SetCursor(5, 40);
		ssd1306_WriteString("Y : ", Font_7x10, White);
		ssd1306_SetCursor(33, 40);
		ssd1306_WriteString(itoa(resultado[1], zz, 10), Font_7x10, White);
		ssd1306_SetCursor(5, 50);
		ssd1306_WriteString("Z : ", Font_7x10, White);
		ssd1306_SetCursor(33, 50);
		ssd1306_WriteString(itoa(resultado[2], zz, 10), Font_7x10, White);
		ssd1306_UpdateScreen();
   
	 //release resource
		xSemaphoreGive(xMutex);
	 }
}

 

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
	/* Insert system clock initialization code here. */
	pll_init(8000000, LOW_POWER ,CRYSTAL, 4, 24, MCGOUT);
	
	/* Configure I2C module */
	//Select alternate 1 for I2C0, Multipler = 1 and ICR = 0x0D => baud rate 1MHz @bus clock 48MHz
	i2c_Init(SSD1306_I2C_PORT, ALT1, MULT0, ICR);
				
	/* Display configuration ------------------------------------------------------*/
  ssd1306_Init();

	/* HMC5883 configuration ------------------------------------------------------*/
	HMC5883L_Initialize();
	
  //Create Mutex and Queue
  xMutex = xSemaphoreCreateMutex();
  MyLeitura = xQueueCreate(3,3*sizeof(int16_t));
		
  //Create Main Task that run once and create other tasks
  xTaskCreate(vSetup,"Setup task", 100, NULL, 3, NULL);
   
  //Start Schedule
  vTaskStartScheduler();

while(1){}
 

}

