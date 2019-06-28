/* Count Events */

#include "TM4C123.h"                    // Device header


//Prototypes
void timer3A_RisingEdgeEvent_init(void);
int timer3A_RisingEdgeEvent_capture(void);

//Global variables
uint32_t number_of_events = 0;

int main(void){

	timer3A_RisingEdgeEvent_init();  //initialize Timer3 as event counter

	while(1){
		number_of_events = timer3A_RisingEdgeEvent_capture();
	 	}
	
}

void timer3A_RisingEdgeEvent_init(void){
	SYSCTL->RCGCTIMER |= 0x08;   //enable clock on Timer3
	SYSCTL->RCGCGPIO |= 0x02;    //enable clock on GPIO B for use input on Timer3 trigger
	GPIOB->DIR &= ~(0x04);       //set PB2 as input
	GPIOB->DEN |= 0x04;          //digital enable on PB2
	GPIOB->AFSEL |= 0x04;        //Alternate function for PB2
	GPIOB->PCTL &= ~0x00000F00;  //configure PB2 as T3CCP0 pin
	GPIOB->PCTL |=  0x00000700;
	
	TIMER3->CTL &= ~(0x01);     //disable Timer3 during setup
	TIMER3->CFG = 0x04;         //select 16 btis
	TIMER3->TAMR = 0x013;       //set up-count, edge-count, capture mode;
	TIMER3->TAMATCHR = 0xFFFF;  //set the count limit, compared to TAR to determine the MATCH event
	TIMER3->TAPMR = 0xFF;       //add a prescaler, so the limit uprise to 0xFFFFFF events
	TIMER3->CTL |= 0x01;        //enable the counter
		
}

int timer3A_RisingEdgeEvent_capture(void){
	return TIMER3->TAR;         //compared with TAMATCHR to determinte match event
}


