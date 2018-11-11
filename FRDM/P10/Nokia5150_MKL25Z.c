// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) FRDM-KL25Z pin
// Reset         (RST, pin 1) connected to PTD4
// SSI0Fss       (CE,  pin 2) connected to PTD0
// Data/Command  (DC,  pin 3) connected to PTD5
// SSI0Tx        (Din, pin 4) connected to PTD2
// SSI0Clk       (Clk, pin 5) connected to PTE1
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

#include "Nokia5110.h"
#include <MKL25Z4.h>
#include <stdint.h>


// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.

// This is a helper function that sends an 8-bit message to the LCD.
// inputs: type     COMMAND or DATA
//         message  8-bit code to transmit
// outputs: none
// assumes: SSI0 and port A have already been initialized and enabled
void lcdwrite(enum typeOfWrite type, char message){
	  unsigned int i;
	  char read_spi;
	SPI0->C1 |= SPI_C1_SPE_MASK;
	  if(type == COMMAND){
        //DC = DC_COMMAND;
		    GPIOD->PCOR |=(1<<5);
		} else {
			  //DC = DC_DATA;
			  GPIOD->PSOR |=(1<<5);
		}
		for(i=0;i<1000;i++){};	
    
    while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK) {};
    SPI0_D = message;                // data out
			                               // wait until SSI0 not busy/transmit FIFO empty
		while(!(SPI0_S & SPI_S_SPRF_MASK)) {};
    read_spi = SPI0_D;
	SPI0->C1 &= ~SPI_C1_SPE_MASK;
}

//********Nokia5110_Init*****************
// Initialize Nokia 5110 48x84 LCD by sending the proper
// commands to the PCD8544 driver.  One new feature of the
// LM4F120 is that its SSIs can get their baud clock from
// either the system clock or from the 16 MHz precision
// internal oscillator.  If the system clock is faster than
// 50 MHz, the SSI baud clock will be faster than the 4 MHz
// maximum of the Nokia 5110.
// inputs: none
// outputs: none
// assumes: system clock rate of 50 MHz or less
void Nokia5110_Init(void){
  volatile unsigned long delay;

/*incializa o clock no Módulo SPI0 */
SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK; // set BIT 22 on SCGC4 Register

/*Habilita o clock na Porta D (SPI0) */
SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

/*configura pinos 0, 1, 2 e 3 da porta D para função SPI */
PORTD->PCR[0] |= PORT_PCR_MUX(2);   //Função SPI0_PCS0
PORTD->PCR[1] |= PORT_PCR_MUX(2);   //Função SPI0_SCK
PORTD->PCR[2] |= PORT_PCR_MUX(2);   //Função SPI0_MOSI
//PORTD->PCR[3] |= PORT_PCR_MUX(2);   //Função SPI0_MISO - Não utilizada
	
/*configura pinos 4 e 5 da porta D para função GPIO */
PORTD->PCR[4] |= PORT_PCR_MUX(1);   //Função GPIO RST
PORTD->PCR[5] |= PORT_PCR_MUX(1);   //Função GPIO DC
	
/*Configura a SPI0 */
SPI0->C1 = 0x00;
//SPI0->C1 |= SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;                                            //funcionou
SPI0->C1 |= SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK | SPI_C1_CPOL_MASK | SPI_C1_CPHA_MASK;    //funcionou
//SPI0->C1 |= SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK | SPI_C1_CPOL_MASK;                       //não funcionou
//SPI0->C1 |= SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK | SPI_C1_CPHA_MASK;                       //não funcionou
SPI0->C2 |= SPI_C2_MODFEN_MASK;
SPI0->BR |= SPI_BR_SPPR(2)	| SPI_BR_SPR(0);  //BUSCLOCk dividido por 3 e depois dividido por 2 (24/3/2 = 4 MHz)
//SPI0->C1 |= SPI_C1_SPE_MASK;

/*Configura Pinos Port D 4 e 5 como saída (RST e DC)*/
GPIOD->PDDR |= (1<<4) | (1<<5);

/*Reset no pino 4 da porta D */
GPIOD->PCOR |=(1<<4);
for (delay=0; delay<1000000; delay++);
GPIOD->PSOR |=(1<<4);

lcdwrite(COMMAND, 0x21);              // chip active; horizontal addressing mode (V = 0); use extended instruction set (H = 1)
                                        // set LCD Vop (contrast), which may require some tweaking:
  lcdwrite(COMMAND, 0xB8);          // try 0xB1 (for 3.3V red SparkFun), 0xB8 (for 3.3V blue SparkFun), 0xBF if your display is too dark, or 0x80 to 0xFF if experimenting
  lcdwrite(COMMAND, 0x04);              // set temp coefficient
  lcdwrite(COMMAND, 0x13);              // LCD bias mode 1:48: try 0x13 or 0x14

  lcdwrite(COMMAND, 0x20);              // we must send 0x20 before modifying the display control mode
  lcdwrite(COMMAND, 0x0C);              // set display control to normal mode: 0x0D for inverse
	
}

//********Nokia5110_OutChar*****************
// Print a character to the Nokia 5110 48x84 LCD.  The
// character will be printed at the current cursor position,
// the cursor will automatically be updated, and it will
// wrap to the next row or back to the top if necessary.
// One blank column of pixels will be printed on either side
// of the character for readability.  Since characters are 8
// pixels tall and 5 pixels wide, 12 characters fit per row,
// and there are six rows.
// inputs: data  character to print
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutChar(unsigned char data){
  int i;
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
  for(i=0; i<5; i=i+1){
    lcdwrite(DATA, ASCII[data - 0x20][i]);
  }
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
}

//********Nokia5110_OutString*****************
// Print a string of characters to the Nokia 5110 48x84 LCD.
// The string will automatically wrap, so padding spaces may
// be needed to make the output look optimal.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutString(char *ptr){
  while(*ptr){
    Nokia5110_OutChar((unsigned char)*ptr);
    ptr = ptr + 1;
  }
}

//********Nokia5110_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    Nokia5110_OutString("    ");
    Nokia5110_OutChar(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    Nokia5110_OutString("   ");
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  } else if(n<1000){
    Nokia5110_OutString("  ");
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    Nokia5110_OutChar(' ');
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else {
    Nokia5110_OutChar(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
}

//********Nokia5110_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=11)
//         newY  new Y-position of the cursor (0<=newY<=5)
// outputs: none
void Nokia5110_SetCursor(unsigned char newX, unsigned char newY){
  if((newX > 11) || (newY > 5)){        // bad input
    return;                             // do nothing
  }
  // multiply newX by 7 because each character is 7 columns wide
  lcdwrite(COMMAND, 0x80|(newX*7));     // setting bit 7 updates X-position
  lcdwrite(COMMAND, 0x40|newY);         // setting bit 6 updates Y-position
}

//********Nokia5110_Clear*****************
// Clear the LCD by writing zeros to the entire screen and
// reset the cursor to (0,0) (top left corner of screen).
// inputs: none
// outputs: none
void Nokia5110_Clear(void){
  int i;
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
    lcdwrite(DATA, 0x00);
  }
  Nokia5110_SetCursor(0, 0);
}

//********Nokia5110_DrawFullImage*****************
// Fill the whole screen by drawing a 48x84 bitmap image.
// inputs: ptr  pointer to 504 byte bitmap
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_DrawFullImage(const char *ptr){
  int i;
  Nokia5110_SetCursor(0, 0);
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
    lcdwrite(DATA, ptr[i]);
  }
}
