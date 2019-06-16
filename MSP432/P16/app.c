/*Template para utilizar o uCOS-III */

/****************************** Include Files ********************************/
#include "msp.h"              // Device header
#include <stdint.h>
#include "os.h"              // Micrium.Micrium::RTOS:uC/OS Kernel



/****************************** Global Variables *****************************/
#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO     1
static OS_TCB AppTaskStartTCB;
static OS_TCB AppTaskCounterTCB;

static CPU_STK AppTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK AppTaskCounterSTK[128u];

uint32_t contador;


/*********************** Private function prototypes *************************/
static void App_task_start(void *p_arg);
static void App_task_counter (void *p_arg);



/******************************** Main Block *********************************/
int main(void){
	
  OS_ERR os_err;
	
  SystemInit();   //MSP432 Board Initialization
	
	CPU_Init();
	
	OS_CPU_SysTickInit(SystemCoreClock/OSCfg_TickRate_Hz);  //Setup Systick for 1ms
	
	
	OSInit( &os_err );
	
	OSTaskCreate(   	(OS_TCB      *) &AppTaskStartTCB,            /* create start task */
	              		(CPU_CHAR    *) "App Task Start",
	              		(OS_TASK_PTR  ) App_task_start,
	              		(void        *) 0,
			              (OS_PRIO      ) APP_TASK_START_PRIO,
			              (CPU_STK     *) &AppTaskStartSTK[0],
			              (CPU_STK_SIZE ) APP_TASK_START_STK_SIZE/10,
			              (CPU_STK_SIZE ) APP_TASK_START_STK_SIZE,
			              (OS_MSG_QTY   ) 0,
			              (OS_TICK      ) 0,
			              (void        *) 0,
			              (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
			              (OS_ERR      *) &os_err);
							
	OSStart(& os_err);
	
}


/*********************** Private function definitions *************************/
static void App_task_start (void *p_arg){
	OS_ERR os_err;
	
 	//The others Tasks starts here
	
	OSTaskCreate(   	(OS_TCB      *) &AppTaskCounterTCB,            /* create counter task */
	              		(CPU_CHAR    *) "App Task Counter",
	              		(OS_TASK_PTR  ) App_task_counter,
	              		(void        *) 0,
			              (OS_PRIO      ) 5,
			              (CPU_STK     *) &AppTaskCounterSTK[0],
			              (CPU_STK_SIZE ) 0,
			              (CPU_STK_SIZE ) 128u,
			              (OS_MSG_QTY   ) 0,
			              (OS_TICK      ) 0,
			              (void        *) 0,
			              (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
			              (OS_ERR      *) &os_err);

	//Terminate the StartTask
  OSTaskSuspend(&AppTaskStartTCB, &os_err);
										
}

//Definition for Other Tasks

static void App_task_counter (void *p_arg){
	/* Task Example */
	while(DEF_TRUE){
		contador++;
	}
}



