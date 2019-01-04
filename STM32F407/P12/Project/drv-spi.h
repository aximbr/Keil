/* drv-spi.h */

#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include <stdint.h>

void spi_init(void);
void RST_reset(void);
void spi_write_byte( uint8_t data, uint8_t dc);

#endif /* __DRV_SPI_H */
