/*
 P14 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-03-18
*******************************************************************************/
/* DriverLib Includes */
#include "msp432.h"
#include "driverlib.h"


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "HMC5883L.h"
#include "conv_to_string.h"
#include "my_tick.h"
#include "FreeRTOS.h."
#include "task.h"
#include "queue.h"
#include "semphr.h"


/* Global variables */
SemaphoreHandle_t xMutex; //Semaphore to cotrol access to I2C bus
xQueueHandle MyLeitura;  //fila para transferir dados de uma task para outra

/*prototypes for local functions */
void vSetup(void *pvParameters);
void vReadHMC5883(void *pvParameters);
void vDisplayResult(void *pvParameters);
void clock_init(void);

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


/*MAIN Program */
int main (void)
{

	/* Halting the Watchdog */
  MAP_WDT_A_holdTimer();
	
	/* Insert system clock initialization code here. */
  clock_init();
	
		
	/* HW init ------------------------------------------------------*/
  hal_i2c_init(EUSCI_B0);
	ssd1306_Init();
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

void clock_init(void){
	/* configure MCU for DCO source @ 3Mhz */
 CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
 CS_setDCOFrequency(3000000);  
	/* Configuring pins for peripheral/crystal usage */
 GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	/* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
   * let’s set the clock frequency in the code.
  */
 CS_setExternalClockSourceFrequency(32000, 48000000);
  /* Starting HFXT in non-bypass mode without a timeout. Before we start
   * we have to change VCORE to 1 to support the 48MHz frequency */
 PCM_setCoreVoltageLevel(PCM_VCORE1);
 FlashCtl_setWaitState(FLASH_BANK0, 2);
 FlashCtl_setWaitState(FLASH_BANK1, 2);
 CS_startHFXT(false);
  /* Initializing MCLK to HFXT (effectively 48MHz) */
 CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	
	/* Initializing SMCLK to DCO (effectively 3MHz) */
 CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
 CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);	
}

