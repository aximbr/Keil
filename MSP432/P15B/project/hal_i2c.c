// Header:
// File Name: hal_i2c.c
// Author: Jose P. Leitao
// Date: 2019-04-28

/* Implement some level of abstraction for I2C communication */

#include "hal_i2c.h"


#define I2C_SPEED   I2C_SET_DATA_RATE_400KBPS

/****************** Help Functions ******************/

//setup device speed
void hal_i2c_setup_speed(EUSCI_B_Type *i2cx, uint32_t speed)
{
#define SYSTEM_SPEED (uint32_t) SystemCoreClock   //System Clock in KHZ 1000 KHz = 1MHZ
	uint16_t prescaler;
	prescaler = (uint16_t)(SYSTEM_SPEED/(speed));  //passar a velocidade do device

	i2cx->BRW = prescaler;
}

//Disable the I2C peripheral
void hal_i2c_disable_peripheral(EUSCI_B_Type *i2cx)
{
	
	i2cx->CTLW0 |= I2C_REG_CTLW0_RST;
}

//Enable the I2C peripheral
void hal_i2c_enable_peripheral(EUSCI_B_Type *i2cx)
{
	
	i2cx->CTLW0 &= ~I2C_REG_CTLW0_RST;
}

//Enable the I2C peripheral as Master
void hal_i2c_enable_master(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 &= ~I2C_REG_CTLW0_UCMST;
	i2cx->CTLW0 |= I2C_REG_CTLW0_UCMST;
}

//Write device Address
void hal_i2c_write_slave_address(EUSCI_B_Type *i2cx, uint8_t mem_add)
{
	i2cx->I2CSA = mem_add;
}

//Write data
void hal_i2c_write_data(EUSCI_B_Type *i2cx, uint8_t send_data)
{
	i2cx->TXBUF = send_data;
}

//Receive data
uint8_t hal_i2c_receive_data(EUSCI_B_Type *i2cx)
{
	return i2cx->RXBUF;
}

//Command to send START on TX mode
void hal_i2c_cmd_start_tx(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 |= I2C_REG_CTLW0_UCTXSTT | I2C_REG_CTLW0_UCTR;
}

//Command to send START on RX mode
void hal_i2c_cmd_start_rx(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 &= ~I2C_REG_CTLW0_UCTR;
	i2cx->CTLW0 |= I2C_REG_CTLW0_UCTXSTT;
}
//Command to send stop
void hal_i2c_cmd_stop(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 |= I2C_REG_CTLW0_UCTXSTP;
}

//Command to set TX mode
void hal_i2c_cmd_tx_mode(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 |= I2C_REG_CTLW0_UCTR;
}

//Command to set RX mode
void hal_i2c_cmd_rx_mode(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 &= ~I2C_REG_CTLW0_UCTR;
}

//set I2C mode
void hal_i2c_set_i2c_mode(EUSCI_B_Type *i2cx)
{
	i2cx->CTLW0 |= I2C_REG_CTLW0_I2CMODE;
}
/****************** Public Functions ******************/
//Configure the peripheral

void hal_i2c_init(EUSCI_B_Type *i2cx){
		/* Configure I2C module */
//Set primary function for P1.6 (UCB0SDA) and P1.7 (UCB0SCL)
//Set primary function for P6.4 (UCB1SDA) and P6.5 (UCB1SCL)
	if (i2cx == EUSCI_B0)
	    {	P1->SEL0 |= BIT6 | BIT7;}                // I2C pins
	else
	    { P6->SEL0 |= BIT4 | BIT5;}                // I2C pins
	
	hal_i2c_disable_peripheral(i2cx);
	hal_i2c_setup_speed(i2cx, I2C_SPEED); //defined at begining of file and at header file
	hal_i2c_set_i2c_mode(i2cx);
	hal_i2c_enable_master(i2cx);
	hal_i2c_enable_peripheral(i2cx);
}

//Receive bytes from I2C device at specif memory address and store in buffer
uint8_t hal_i2c_mem_read(EUSCI_B_Type *i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){
  
	//Write device address
  hal_i2c_write_slave_address(i2cx, dev_add);

	//Send command to transmit
	hal_i2c_cmd_start_tx(i2cx);
		
	/*wait for TX buffer is empty and Device Address was transmited*/
	while(!(i2cx->IFG & I2C_REG_IFG_UCTXIFG0) || (i2cx->CTLW0 & I2C_REG_CTLW0_UCTXSTT) ) {}
  
	//Write memory address
	hal_i2c_write_data(i2cx, mem_add);
	
	/*wait for TX buffer is empty UCTXIFG0 */
	while(!(i2cx->IFG & I2C_REG_IFG_UCTXIFG0)) {}; 
		
	//Send command to send START and go to RX Mode
	hal_i2c_cmd_start_rx(i2cx);
		
	/*wait for Device Address was transmited*/
	while(i2cx->CTLW0 & I2C_REG_CTLW0_UCTXSTT) {}
  
	while(len){
		
		if (len == 1){
			hal_i2c_cmd_stop(i2cx); //send stop
		  /* wait for RX Buffer is ready with data */
		  while (!(i2cx->IFG & I2C_REG_IFG_UCRXIFG0)) {}
		  *value++ = hal_i2c_receive_data(i2cx);
		}
		else {
			/* wait for RX Buffer is ready with data */
      while (!(i2cx->IFG & I2C_REG_IFG_UCRXIFG0)) {}
		  *value++ = hal_i2c_receive_data(i2cx);
		}
		len--;
		}//end of while
	
	/*wait for STOP finish UCTXSTP=0 */
	while((i2cx->CTLW0 & I2C_REG_CTLW0_UCTXSTP)) {};
		
	return 0;
}


//Transmit bytes to I2C device at specif memory address from a buffer
uint8_t hal_i2c_mem_write(EUSCI_B_Type *i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){
 	
	//Write device address
  hal_i2c_write_slave_address(i2cx, dev_add);

	//Send command to transmit
	hal_i2c_cmd_start_tx(i2cx);
		
	/*wait for TX buffer is empty and Device Address was transmited*/
	while(!(i2cx->IFG & I2C_REG_IFG_UCTXIFG0) || (i2cx->CTLW0 & I2C_REG_CTLW0_UCTXSTT) ) {}
  
	//Write memory address
	hal_i2c_write_data(i2cx, mem_add);
		
  while(len)
	{
	/*wait for TX buffer is empty UCTXIFG0 */
	while(!(i2cx->IFG & I2C_REG_IFG_UCTXIFG0)) {}; 
		
	if (len == 1) {
		   hal_i2c_write_data(i2cx, (*value++));
		   /*wait for TX buffer is empty UCTXIFG0 */
       while(!(i2cx->IFG & I2C_REG_IFG_UCTXIFG0)) {};
			 hal_i2c_cmd_stop(i2cx); //send stop
	    }
	else {
	     hal_i2c_write_data(i2cx, (*value++));
	     }
	
	len--;	
	}//end of while
	
	/*wait for STOP finish UCTXSTP=0 */
	while((i2cx->CTLW0 & I2C_REG_CTLW0_UCTXSTP)) {};
		
	return 0;
	
}

