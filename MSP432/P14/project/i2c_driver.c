// Header:
// File Name: i2c_driver.c
// Author: José P. Leitão
// Date: 2019-03-12

/*Driver I2C para MSP432 usando a driverlib TivaWare
  contem funções básicas necessárias para trabalhar com o Display OLED baseado no SD1306 */

#include "driverlib.h"
#include "i2c_driver.h"
#include "my_tick.h"

/*Implementation Local functions */
void hal_i2c_mem_write(uint32_t i2cx, uint8_t slave_address, uint8_t mem_address, uint8_t *buffer, uint32_t len)
{
	
//Write device address
  I2C_setSlaveAddress(i2cx, slave_address);
		
//Master send Start
  
	I2C_setMode(i2cx, EUSCI_B_I2C_TRANSMIT_MODE);
	
//Enable device
	I2C_enableModule(i2cx);
	
//Write to memory address
	/* Making sure the last transaction has been completely sent out */
  while (I2C_masterIsStopSent(i2cx) == EUSCI_B_I2C_SENDING_STOP);
	
	I2C_masterSendMultiByteStart(i2cx, mem_address);

//Write data to memory	
	uint32_t count = len;	
	while(len)
	{
	I2C_masterSendMultiByteNext(i2cx, buffer[count-len]);
	len--;	
	}//end of while
	
//send STOP
	I2C_masterSendMultiByteStop(i2cx);
	
	while (I2C_masterIsStopSent(i2cx) == EUSCI_B_I2C_SENDING_STOP);
	
		
		
}
