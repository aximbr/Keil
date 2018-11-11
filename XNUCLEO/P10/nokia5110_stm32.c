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
LCD_write_byte (0xB8, 0); // set the contrast
//LCD_write_byte (0x14, 0); // set the bias voltage
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
// LCD_set_XY: posiciona o cursor
// x: coluna (0-83)
// y: linha (0-5)
void LCD_set_XY(unsigned char X, unsigned char Y)
{
LCD_write_byte(0x40 | Y, 0); // column
LCD_write_byte(0x80 | X, 0); // row
}
void LCD_write_char(unsigned char c) {
unsigned char line;
for (line=0; line<5; line++)
LCD_write_byte(ASCII[c-32][line], 1);
}
void LCD_write_string(unsigned char x,unsigned char y,char *s) {
LCD_set_XY(x,y);
while (*s) LCD_write_char(*s++);
}

//---------------------------------------
// implementa delay de 1 ms, conforme clock de 48 MHz
//---------------------------------------
void delayMs(int n) {
int i, j;
for(i = 0 ; i < n; i++)
for (j = 0; j < 8010; j++);
}

