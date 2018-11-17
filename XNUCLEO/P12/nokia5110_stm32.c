#include "nokia5110_stm32.h"

/* Private variables ---------------------------------------------------------*/ 
 
RCC_ClocksTypeDef RCC_ClockFreq; 
SPI_InitTypeDef  SPI_InitStructure;



void SPI1_init(void) {
// Enable CLK to port GPIOB, SPI1  
RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_SPI1, ENABLE); 
	

	
	// Configure SPI
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; 
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
SPI_InitStructure.SPI_CRCPolynomial = 7; 
SPI_Init(SPI1, &SPI_InitStructure);
SPI_SSOutputCmd(SPI1,ENABLE); //habilita controle do CS (SPIx->C2 (bit SSOE))


  // Configure SPI - CE PA4, CLK PA5, MOSI PA7
	
  GPIO_PinConfigure(GPIOA, 4, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);	
	GPIO_PinConfigure(GPIOA, 5, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
	GPIO_PinConfigure(GPIOA, 7, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
	
	// Configure GPIOB - DC PB9, RST PB0 
	GPIO_PinConfigure(GPIOB, 9, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);
	GPIO_PinConfigure(GPIOB, 0, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);
	
}

void LCD_init(void) {
GPIO_PinWrite(GPIOB, 0, 0);  //RST = LOW
delayMs(100);
GPIO_PinWrite(GPIOB, 0, 1);  //RST = HIGH
delayMs(100);
LCD_write_byte (0x21, 0); // use the extended command set the LCD mode
LCD_write_byte (0xB3, 0); // set the contrast
LCD_write_byte (0x04, 0); // temperature correction
LCD_write_byte (0x13, 0); // 1:48
LCD_write_byte (0x20, 0); // use basic commands
LCD_clear (); // clear the screen
LCD_write_byte (0x0c, 0); // set display mode, the normal display
}

void LCD_write_byte(unsigned char data, unsigned char dc) {
int i;
SPI_Cmd(SPI1, ENABLE);           /* Enable the SPI (Clock e CS)  */
//delayMs(1);                      /* aguarda estabilizar o clock */
for (i=0; i<1000; i++){};
	
if(dc) { GPIO_PinWrite(GPIOB, 9, 1);} // DC = 1 (DATA)
       else GPIO_PinWrite(GPIOB, 9, 0); // DC = 0 (COMMAND)

while(!(SPI1->SR & SPI_SR_TXE)){}; // Aguarda até o Buffer de TX estiver vazio
SPI1->DR = data; // TX character
	
SPI_Cmd(SPI1, DISABLE);           /* Disable the SPI (clock e CS)  */

}
void LCD_write_logo(unsigned const char *ptr) {
unsigned int ctr = 0;
while(ctr++ < 504) {
LCD_write_byte(*ptr++,1);
}
}
void LCD_clear(void) {
unsigned int i;
LCD_write_byte(0x0c, 0);
LCD_write_byte(0x80, 0);
for (i=0; i<504; i++)
LCD_write_byte(0, 1);
}

//********LCD_set_XY*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// Inputs: newX  new X-position of the cursor (0<=newX<=11)
//         newY  new Y-position of the cursor (0<=newY<=5)
// Outputs: none
void LCD_set_XY(uint8_t newX, uint8_t newY){
if((newX > 11) || (newY > 5)){        // bad input
    return;                             // do nothing
  }
  // multiply newX by 7 because each character is 7 columns wide
  LCD_write_byte(0x80|newX*7,0);     // setting bit 7 updates X-position
  LCD_write_byte(0x40|newY, 0);      // setting bit 6 updates Y-position
}


void LCD_write_char(unsigned char c) {
unsigned char line;
//LCD_write_byte(0x00, 1);
for (line=0; line<5; line++)
LCD_write_byte(ASCII[c-32][line], 1);
LCD_write_byte(0x00, 1);
}
void LCD_write_string(char *s) {

while (*s) LCD_write_char(*s++);
}

//********LCD_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void LCD_OutUDec(unsigned short n){
  if(n < 10){
    LCD_write_string("     ");
    LCD_write_char(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    LCD_write_string("     ");
    LCD_write_char(n/10+'0'); /* tens digit */
    LCD_write_char(n%10+'0'); /* ones digit */
  } else if(n<1000){
    LCD_write_string("   ");
    LCD_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    LCD_write_char(n/10+'0'); /* tens digit */
    LCD_write_char(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    LCD_write_char(' ');
    LCD_write_char(n/1000+'0'); /* thousands digit */
    LCD_write_char(',');
		n = n%1000;
    LCD_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    LCD_write_char(n/10+'0'); /* tens digit */
    LCD_write_char(n%10+'0'); /* ones digit */
  }
  else {
    LCD_write_char(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    LCD_write_char(n/1000+'0'); /* thousands digit */
		LCD_write_char(',');
    n = n%1000;
    LCD_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    LCD_write_char(n/10+'0'); /* tens digit */
    LCD_write_char(n%10+'0'); /* ones digit */
  }
}


//---------------------------------------
// implementa delay de 1 ms, conforme clock de 48 MHz
//---------------------------------------
void delayMs(int n) {
int i, j;
for(i = 0 ; i < n; i++)
for (j = 0; j < 8010; j++);
}

