/* drv-nokia5110.c */

#include "main.h"
#include "drv-nokia5110.h"
#include "drv-spi.h"

void nokia5110_init(void){
	spi_init();
	RST_reset();
	spi_write_byte(0x21, 0); // use the extended command set the LCD mode
  spi_write_byte(0xAF, 0); // set the contrast
	spi_write_byte (0x04, 0); // temperature spi_init
	spi_write_byte (0x13, 0); // 1:48
	spi_write_byte (0x20, 0); // use basic commands
	
  nokia5110_clear (); // clear the screen
  spi_write_byte (0x0c, 0); // set display mode, the normal display
}

/* Clear the screen */
void nokia5110_clear(void) {
unsigned int i;
spi_write_byte(0x0c, 0);
spi_write_byte(0x80, 0);
for (i=0; i<504; i++)
spi_write_byte(0, 1);
}

// nokia_set_XY: posiciona o cursor
// x: coluna (0-83)
// y: linha (0-5)
void nokia5110_set_XY(uint8_t X, uint8_t Y)
{
spi_write_byte(0x40 | Y, 0); // column
spi_write_byte(0x80 | (X*5), 0); // row
}

/*nokia5110_write_char */
void nokia5110_write_char(uint8_t c) {
unsigned char line;
for (line=0; line<5; line++)
spi_write_byte(ASCII[c-32][line], 1);
}

/*LCD_write_string */
void nokia5110_write_string(char *s) {
//nokia5110_set_XY(x,y);
while (*s) nokia5110_write_char(*s++);
}

//********LCD_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    nokia5110_write_string("     ");
    nokia5110_write_char(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    nokia5110_write_string("     ");
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  } else if(n<1000){
    nokia5110_write_string("   ");
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    nokia5110_write_char(' ');
    nokia5110_write_char(n/1000+'0'); /* thousands digit */
    nokia5110_write_char(',');
		n = n%1000;
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
  else {
    nokia5110_write_char(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    nokia5110_write_char(n/1000+'0'); /* thousands digit */
		nokia5110_write_char(',');
    n = n%1000;
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
}

