#include <stdint.h>
#include <stdio.h>
#include "UART.h"
#include "PLL.h"
#include "FreeRTOS.h"
#include "task.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )
portBASE_TYPE volatile resultado;

/* Prototipos das funções locais */
void console_init(void);
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );
void vPrintString( const char *pcString );

/* The task functions prototype*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTaskFunction( void *pvParameters );

/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running\n\r";
const char *pcTextForTask2 = "Task 2 is running\n\r";

/* Declare a variable that is used to hold the handle of Task2. */
xTaskHandle xTask2Handle;

/*-----------------------------------------------------------*/

int main (void)
{
	uint32_t i;
	/* Insert system clock initialization code here (sysclk_init()). */
  PLL_Init();               // clock at 50 MHz
	UART_init();              // initialize UART
		
	/*mensagem indicando que a console está OK */
	UART_OutChar(0);
	UART_OutString("Console OK \r\n");
	
	/* Create one of the two tasks. Note that a real application should check
	the return value of the xTaskCreate() call to ensure the task was created
	successfully. */
  resultado = xTaskCreate( vTaskFunction, "Task 1", 200, (void *)pcTextForTask1,  1, NULL ); 
				 
	/* Create the other task in exactly the same way and at the same priority. */
	resultado = xTaskCreate( vTaskFunction, "Task 2", 200, (void *)pcTextForTask2,  1, NULL );
	
	/* Start the scheduler so the tasks start executing. */
	vTaskStartScheduler();
	
	/* If all is well then main() will never reach here as the scheduler will
	now be running the tasks. If main() does reach here then it is likely that
	there was insufficient heap memory available for the idle task to be created.
	Chapter 2 provides more information on heap memory management. */
	for( ;; );
}

/* Implementação das funções locais */

/* Implementação da Task1 */
void vTask1( void *pvParameters )
{
	const char *pcTaskName = "Task 1 is running\r\n";
	volatile uint32_t ul; /* volatile to ensure ul is not optimized away. */
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		vPrintString( pcTaskName );
		//UART_OutChar(88);
		/* Delay for a period. */
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
			/* This loop is just a very crude delay implementation. There is
			nothing to do in here. Later examples will replace this crude
			loop with a proper delay/sleep function. */
		}
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
	const char *pcTaskName = "Task 2 is running\r\n";
	volatile uint32_t ul; /* volatile to ensure ul is not optimized away. */
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		vPrintString( pcTaskName );
		
		/* Delay for a period. */
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
			/* This loop is just a very crude delay implementation. There is
			nothing to do in here. Later examples will replace this crude
			loop with a proper delay/sleep function. */
		}
	}
}
void vTaskFunction( void *pvParameters )
{
	char *pcTaskName;
	TickType_t xLastWakeTime;
	/* The string to print out is passed in via the parameter. Cast this to a
	character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* The xLastWakeTime variable needs to be initialized with the current tick
	count. Note that this is the only time the variable is written to explicitly.
	After this xLastWakeTime is automatically updated within vTaskDelayUntil(). */
	xLastWakeTime = xTaskGetTickCount();
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		vPrintString( pcTaskName );
		
		/* This task should execute every 2500 milliseconds exactly. As per
		the vTaskDelay() function, time is measured in ticks, and the
		pdMS_TO_TICKS() macro is used to convert milliseconds into ticks.
		xLastWakeTime is automatically updated within vTaskDelayUntil(), so is not
		explicitly updated by the task. */
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 2500 ) );
	}
}

void vPrintString( const char *pcString )
{
	/* Write the string to stdout, using a critical section as a crude method of
	mutual exclusion. */
	taskENTER_CRITICAL();
	{
		//printf( "%s", pcString );
		UART_OutString((char *) pcString);
		//fflush( stdout );
	}
	taskEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/
/* funções apenas para completar o FreeRTOS */
void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

