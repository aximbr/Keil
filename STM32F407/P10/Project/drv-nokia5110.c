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
spi_write_byte(0x80 | X, 0); // row
}

/*LCD_write_char */
void nokia5110_write_char(uint8_t c) {
unsigned char line;
for (line=0; line<5; line++)
spi_write_byte(ASCII[c-32][line], 1);
}

/*LCD_write_string */
void nokia5110_write_string(uint8_t x, uint8_t y,char *s) {
nokia5110_set_XY(x,y);
while (*s) nokia5110_write_char(*s++);
}

