#include <stdint.h>
#include <stdio.h>
#include "UART.h"
#include "PLL.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define TEMP_CODE			1
#define DISP_CODE			2
#define OTHER_CODE    3
#define MAX_LEN				10

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

/* vGenSenderHandler is a task function which takes care of putting data in to the queue. vGenReceiverHandler is task function which takes care of getting data from the queue */
static void vGenSenderHandler( void *pvParameters );
static void vGenReceiverHandler( void *pvParameters );

/*-----------------------------------------------------------*/

/* Declare a variable of type xQueueHandle.  This is used to store the queue
that is accessed by all three tasks. */
xQueueHandle xQueue;

/* Define the structure type that will be passed on the queue. */
typedef struct
{
	unsigned char code;
	int data[MAX_LEN];
} xData;

/* Declare 3 variables of type xData that will be passed on the queue. */
static const xData xStructsToSend[ 4 ] =
{
	{ TEMP_CODE, 33,22 }, /* Used by temp monitoring task. */
	{ DISP_CODE, 64,-32,10 }, /* Used by display monitoring task. */
	{ OTHER_CODE, 2017,2016 }  /* Used by other task. */
};

/*-----------------------------------------------------------*/


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
		
	#if (configUSE_TRACE_FACILITY == 1)
	/*Allocates, if necessary, and initializes the recorder data structure */
	vTraceEnable(TRC_START);
	#endif
	
	
	/*mensagem indicando que a console está OK */
	UART_OutChar(0);
	UART_OutString("Console OK \r\n");
	
	    /* The queue is created to hold a maximum of 3 structures of type xData. */
    xQueue = xQueueCreate( 3, sizeof( xData ) );

	if( xQueue != NULL )
	{
		/* Create 3 instances of the task that will write to the queue.  The
		parameter is used to pass the structure that the task should write to the 
		queue, all 3 sender tasks are created at priority 2 which is above the priority of the receiver. */
		xTaskCreate( vGenSenderHandler, "Temp-mon-task", 100, ( void * ) &( xStructsToSend[ 0 ] ), 2, NULL );
		
		xTaskCreate( vGenSenderHandler, "disp-mon-task", 100, ( void * ) &( xStructsToSend[ 1 ] ), 2, NULL );
		
		xTaskCreate( vGenSenderHandler, "other-task", 100, ( void * ) &( xStructsToSend[ 2 ] ), 2, NULL );

		/* Create the task that will read from the queue.  The task is created with
		priority 1, so below the priority of the sender tasks. */
		xTaskCreate( vGenReceiverHandler, "Receiver-task", 100, NULL, 1, NULL );

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}
	else
	{
		/* The queue could not be created. */
	}
		
    /* If all is well we will never reach here as the scheduler will now be
    running the tasks.  If we do reach here then it is likely that there was 
    insufficient heap memory available for a resource to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
	
/* Implementação das funções locais */

static void vGenSenderHandler( void *pvParameters )
{
portBASE_TYPE xStatus;
const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* The first parameter is the queue to which data is being sent.  The 
		queue was created before the scheduler was started, so before this task
		started to execute.

		The second parameter is the address of the structure being sent.  The
		address is passed in as the task parameter. 

		The third parameter is the Block time - the time the task should be kept
		in the Blocked state to wait for space to become available on the queue
		should the queue already be full.  A block time is specified as the queue
		will become full.  Items will only be removed from the queue when both
		sending tasks are in the Blocked state.. */
		xStatus = xQueueSendToBack( xQueue, pvParameters, xTicksToWait );
		

		if( xStatus != pdPASS )
		{
			/* We could not write to the queue because it was full - this must
			be an error as the receiving task should make space in the queue
			as soon as both sending tasks are in the Blocked state. */
			vPrintString( "Could not send to the queue.\r\n" );
		}else
		{
			vPrintString("Sender: data sent \r\n");
		}

		/* Allow the other sender task to execute. */
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

static void vGenReceiverHandler( void *pvParameters )
{
/* Declare the structure that will hold the values received from the queue. */
xData xReceivedStructure;
portBASE_TYPE xStatus;

	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* As this task only runs when the sending tasks are in the Blocked state, 
		and the sending tasks only block when the queue is full, this task should
		always find the queue to be full.  3 is the queue length. */
		if( uxQueueMessagesWaiting( xQueue ) == 3 )
		{
			vPrintString( "Queue should have been full!\r\n" );
		}

		/* The first parameter is the queue from which data is to be received.  The
		queue is created before the scheduler is started, and therefore before this
		task runs for the first time.

		The second parameter is the buffer into which the received data will be
		placed.  In this case the buffer is simply the address of a variable that
		has the required size to hold the received structure. 

		The last parameter is the block time - the maximum amount of time that the
		task should remain in the Blocked state to wait for data to be available 
		should the queue already be empty.  A block time is not necessary as this
		task will only run when the queue is full so data will always be available. */
		xStatus = xQueueReceive( xQueue, &xReceivedStructure, 0 );

		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value and the source of the value. */
			if( xReceivedStructure.code == TEMP_CODE )
			{
				//printf( "Data From Temp-mon-task = %d,%d \n", xReceivedStructure.data[0],xReceivedStructure.data[1] );
				vPrintString ("Data From Temp-mon-task = ");
				UART_OutUDec(xReceivedStructure.data[0]);
				UART_OutUDec(xReceivedStructure.data[1]);
				vPrintString ("\r\n");
			}
			else if (xReceivedStructure.code == DISP_CODE)
			{
				//printf( "Data From disp-mon-task = %d,%d,%d \n", xReceivedStructure.data[0],xReceivedStructure.data[1], xReceivedStructure.data[2]);
				vPrintString ("Data From disp-mon-task = ");
				UART_OutUDec(xReceivedStructure.data[0]);
				UART_OutUDec(xReceivedStructure.data[1]);
				UART_OutUDec(xReceivedStructure.data[2]);
				vPrintString ("\r\n");
			}
			else if (xReceivedStructure.code == OTHER_CODE)
			{
				//printf( "Data From other-task = %d,%d \n", xReceivedStructure.data[0],xReceivedStructure.data[1] );
				vPrintString ("Data From other-task = ");
				UART_OutUDec(xReceivedStructure.data[0]);
				UART_OutUDec(xReceivedStructure.data[1]);
				vPrintString ("\r\n");
			}else
			{
				vPrintString( "in-valid code \r\n");
				 
			}
		}
		else
		{
			/* We did not receive anything from the queue.  This must be an error 
			as this task should only run when the queue is full. */
			vPrintString( "Could not receive from the queue.\r\n" );
		}
	}
}

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
