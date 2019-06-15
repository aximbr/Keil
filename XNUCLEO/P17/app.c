/*Template para utilizar o uCOS-III */

/****************************** Include Files ********************************/
#include "stm32f10x.h"
#include <stdint.h>
#include "os.h"              // Micrium.Micrium::RTOS:uC/OS Kernel
#include "GPIO_STM32F10x.h"  // Keil::Device:GPIO
#include "os_trace.h"        // Trace capebility with SysView


/****************************** Global Variables *****************************/
#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO     1
#define BIT_MASK                0x3F
#define NUM_ENT_TABLE           BIT_MASK + 1


static OS_TCB AppTaskStartTCB;
static OS_TCB AppTaskDACOutTCB;

static CPU_STK AppTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK AppTaskDACOutSTK[128u];

// 6-bit 64-element sine wave
const unsigned short wave[NUM_ENT_TABLE] = {
  32,35,38,41,44,47,49,52,54,56,58,
  59,61,62,62,63,63,63,62,62,61,59,
  58,56,54,52,49,47,44,41,38,35,
  32,29,26,23,20,17,15,12,10,8,
  6,5,3,2,2,1,1,1,2,2,3,
  5,6,8,10,12,15,17,20,23,26,29};

uint8_t pos; //index of wave table


/*********************** Private function prototypes *************************/
static void App_task_start(void *p_arg);
static void App_task_dac_out (void *p_arg);
void dac_out( uint16_t val);
void dac_init(void);

/******************************** Main Block *********************************/
int main(void){
	
  OS_ERR os_err;

	OS_TRACE_INIT();
	OS_TRACE_START();
	
  SystemInit();   //STM32F103 Board Initialization
	
	CPU_Init();
	
/* other Hardware initialization here */
	dac_init();
	
/* initialize here the OS Kernel */
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
	
	OSTaskCreate(   	(OS_TCB      *) &AppTaskDACOutTCB,            /* create counter task */
	              		(CPU_CHAR    *) "App Task DAC out",
	              		(OS_TASK_PTR  ) App_task_dac_out,
	              		(void        *) 0,
			              (OS_PRIO      ) 5,
			              (CPU_STK     *) &AppTaskDACOutSTK[0],
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

static void App_task_dac_out (void *p_arg){
	OS_ERR os_err;
	
	while(DEF_TRUE){
		dac_out(wave[++pos & BIT_MASK]);
		OSTimeDly(5,                  //define the period of frequencie here (5 ms => 200 Hz)
		          OS_OPT_TIME_PERIODIC,
		          &os_err);
	}
}

void dac_out( uint16_t val){
	GPIO_PortWrite (GPIOA, BIT_MASK, val);
}

void dac_init(void){
	GPIO_PortClock(GPIOA, ENABLE);
	for (int i=0; i<6; i++){
	GPIO_PinConfigure(GPIOA, i, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);
	}
	
}

/***************************************/
CPU_TS_TMR_FREQ  CPU_TS_TmrFreqGet(CPU_ERR *p_err){
	return SystemCoreClock;
}



