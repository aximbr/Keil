/*Meu Hal I2C */
#include "hal_i2c.h"
#include "my_tick.h"



/****************** Public Functions ******************/

//Master TX using Memory device address
void i2c_mem_write(I2C_TypeDef* I2Cx, uint8_t slave_address, uint8_t mem_address, uint8_t *buffer, uint32_t len)
{
  
	
	//define number of bytes to transfer
	uint32_t count;
	count = len + 1; //include the memory address
	
	//go to start mode
	while (I2Cx->ISR & I2C_ISR_BUSY); //wait until BUSY bit clear
	
	/*            I2C_Reload_Mode: Enable Reload mode .
  *             I2C_AutoEnd_Mode: Enable Automatic end mode.
  *             I2C_SoftEnd_Mode: Enable Software end mode.
  * @param  StartStopMode: new state of the I2C START condition generation.
  *          This parameter can be one of the following values:
  *            @arg I2C_No_StartStop: Don't Generate stop and start condition.
  *            @arg I2C_Generate_Stop: Generate stop condition (Number_Bytes should be set to 0).
  *            @arg I2C_Generate_Start_Read: Generate Restart for read request.
  *            @arg I2C_Generate_Start_Write: Generate Restart for write request.*/
	I2C_TransferHandling(I2Cx, slave_address, count, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

	//first set the device memory address
	while (!(I2Cx->ISR & I2C_ISR_TXIS)) {}  //wait for TXIS
     I2Cx->TXDR = mem_address;
	   count--;
	
  //Then send the data		
	while(count)
	 { 
		 while (!(I2Cx->ISR & I2C_ISR_TXIS)) {}  //wait for TXIS
     I2Cx->TXDR = *buffer++;
	   count--;
	 }	
  
	 //clear STPOF bit
	 if (I2Cx->ISR & I2C_ISR_STOPF)
		   I2Cx->ISR |= I2C_ISR_STOPF;
	 
	 
  
}
