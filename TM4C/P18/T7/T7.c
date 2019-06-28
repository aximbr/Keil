/* Count Time betwwen Events */
/* code used with ultrasonic sensor to measure distance */

#include "TM4C123.h"                    // Device header

#define ECHO          (1U << 6)   //PB6
#define TRIGGER_PIN   (1U << 4)   //PA4
#define BLUE_LED      (1U << 2)   //PF2

//Prototypes
void timer1_delay_microsecond(int usec);
void timer0_init(void);
void ports_init(void);
uint32_t measureD(void);

//Global variables
uint32_t high_edge, low_edge, distance;
uint32_t distance_cm;
const double _16MHz_1clock = 62.5e-9;
const uint32_t MULTIPLIER = 5882;

int main(void){
	
	ports_init();
	timer0_init();
	
	while(1){
	   distance_cm = measureD();
		
		 if (distance_cm < 15) {
			 GPIOF->DATA |= BLUE_LED;
		 }
		 else{
			 GPIOF->DATA &= ~BLUE_LED;
		 }
	 	}
	
}

void timer1_delay_microsecond(int usec){
	SYSCTL->RCGCTIMER |= 0x02; //enable clock on timer1
	
	TIMER1->CTL &= ~(0x01); //disable timer
  TIMER1->CFG = 0x04; //configure timer for 16 bits (maximum delay is 4 ms)
	TIMER0->TAMR |= 0x02; //TAMR bits: periodic timer mode
	TIMER0->TAILR = 16 - 1; //Load Timer with number that's count for 1 usec. Clock @ 16MHz
	TIMER0->ICR |= 0x01; //clear raw interrupt
	TIMER0->CTL |= 0x01; //enable timer
	
	for (int j = 0; j < usec; j++){
		while ( (TIMER0->RIS & 0x01) == 0 ); //wait until timer expire
		TIMER0->ICR |= 0x01; //clear interrupt
	}
}
	
void timer0_init(void){

	SYSCTL->RCGCTIMER |= (1U << 0);   //enable clock on Timer0
	SYSCTL->RCGCGPIO |= 0x02;         //enable clock on GPIO B for use input on Timer0 trigger
	GPIOB->DIR &= ~ECHO;              //set PB6 as input
	GPIOB->DEN |= ECHO;               //digital enable on PB6
	GPIOB->AFSEL |= ECHO;             //Alternate function for PB6
	GPIOB->PCTL &= ~0x0F000000;       //configure PB6 as T0CCP0 pin
	GPIOB->PCTL |=  0x07000000;
	
	TIMER0->CTL &= ~(0x01);     //disable Timer0 during setup
	TIMER0->CFG = 0x04;         //select 16 btis
	TIMER0->TAMR = 0x017;       //set edge-time mode, capture mode, up counter
	TIMER0->CTL |= 0x0C;        //set TAEVENT bit to both edges
	TIMER0->CTL |= 0x01;        //enable the counter
		
}

void ports_init(void){
	SYSCTL->RCGCGPIO |= 0x01;         //enable clock on GPIO A
	SYSCTL->RCGCGPIO |= 0x05;         //enable clock on GPIO F
	GPIOA->DIR |= TRIGGER_PIN;        //set direction for Trigger Pin PA.6
	GPIOA->DEN |= TRIGGER_PIN;        //enable digital mode for PA.6
	GPIOF->DIR |= BLUE_LED;           //set direction for Blue Led PF.2
	GPIOF->DEN |= BLUE_LED;           //enable digital mode for PF.2
}

uint32_t measureD(void){
	GPIOA->DATA &= ~TRIGGER_PIN;      //clear trigger pin for 12 usec
	timer1_delay_microsecond(12);
	GPIOA->DATA |= TRIGGER_PIN;       //set trigger pin for 12 usec
	timer1_delay_microsecond(12);
	GPIOA->DATA &= ~TRIGGER_PIN;      //clear trigger pin
	
	/*capture first edge*/
	TIMER0->ICR |= 0x4;           //set Bit 2 (CAECINT) of ICR clears time capture flag
	while( (TIMER0->RIS & 0x04) == 0); //pool for bit 2 of IRIS (CAERIS)
	high_edge = TIMER0->TAR;           //store the up edge
	
  /*capture second edge*/
	TIMER0->ICR |= 0x4;           //set Bit 2 (CAECINT) of ICR clears time capture flag
	while( (TIMER0->RIS & 0x04) == 0); //pool for bit 2 of IRIS (CAERIS)
	low_edge = TIMER0->TAR;           //store the down edge
	
	distance = low_edge - high_edge;
	distance = _16MHz_1clock * (double) MULTIPLIER * (double) distance;
	
	return distance;
}

