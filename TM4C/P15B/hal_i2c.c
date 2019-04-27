// Header:
// File Name: hal_i2c.c
// Author: Jose P. Leitao
// Date: 2019-04-13

/* Implement some level of abstraction for I2C communication */

#include "hal_i2c.h"

#define I2C_SPEED   100000UL

/****************** Help Functions ******************/

//setup device speed
void hal_i2c_setup_speed(I2C0_Type *i2cx)
{
#define SYSTEM_SPEED_KHZ (uint32_t) 50000   //System Clock in KHZ 1000 KHz = 1MHZ
	uint32_t tpr;
	tpr = (uint32_t)(SYSTEM_SPEED_KHZ/(20*100));  //passar a velocidade do device
	tpr = 0x0000007F & tpr;
	
//	if (handle->Init.Mode_Speed == MASTER_HIGH_SPEED)
//	 {
//	 tpr = I2C_REG_MTPR_HS | tpr;
//	 }
	
	i2cx->MTPR |= tpr;
}

//Enable the I2C peripheral
void hal_i2c_enable_peripheral(I2C0_Type *i2cx)
{
	i2cx->MCS |= I2C_REG_MCS_RUN;
}

//Enable the I2C peripheral as Master
void hal_i2c_enable_master(I2C0_Type *i2cx)
{
	i2cx->MCR &= ~I2C_REG_MCR_SFE;
	i2cx->MCR |= I2C_REG_MCR_MFE;
}
//Write device Address
void hal_i2c_write_slave_address(I2C0_Type *i2cx, uint8_t mem_add)
{
	i2cx->MSA = mem_add;
}

//Write data
void hal_i2c_write_data(I2C0_Type *i2cx, uint8_t send_data)
{
	i2cx->MDR = send_data;
}

//Receive data
uint8_t hal_i2c_receive_data(I2C0_Type *i2cx)
{
	return i2cx->MDR;
}

//Command to transmit without stop
void hal_i2c_cmd_tx_nostop(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_ACK | I2C_REG_MCS_START | I2C_REG_MCS_RUN;
}

//Command to transmit run
void hal_i2c_cmd_tx_run(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_ACK | I2C_REG_MCS_RUN;
}

//Command to transmit stop
void hal_i2c_cmd_tx_stop(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_STOP | I2C_REG_MCS_RUN;
}


/****************** Public Functions ******************/
//Configure the peripheral

void hal_i2c_init(I2C0_Type *i2cx){
	if (i2cx == I2C0)
  {
  /*configuration for I2C0  (I2C0SCL = PB2, I2C0SDA = PB3) */
	_HAL_RCGC_I2C0_CLK_ENABLE();   //enable clock on I2C0
	_HAL_RCGC_GPIOB_CLK_ENABLE();  //enable clock on GPIOB
	GPIOB->AFSEL |= (1 <<2) | (1 << 3); //PB2 and PB3 set on AFSEL
	GPIOB->DEN |= (1 << 2) | (1 << 3);  //Digital enable
	GPIOB->ODR |= (1 << 3); //PB3 (I2C0SDA) as Open Drain
  GPIOB->PCTL |= 0x003300; //PB2 and PB3 selected as I2C peripheric (function 3)
	}

if (i2cx == I2C1)
  {	
  /*configuration for I2C1 (I2C1SCL = PA6, I2C1SDA = PA7)*/	
	_HAL_RCGC_I2C1_CLK_ENABLE();   //enable clock on I2C1
	_HAL_RCGC_GPIOA_CLK_ENABLE();  //enable clock on GPIOA
	GPIOA->AFSEL |= (1 <<6) | (1 << 7); //PA6 and PA7 set on AFSEL
	GPIOA->DEN |= (1 << 6) | (1 << 7);  //Digital enable
	GPIOA->ODR |= (1 << 7); //PA7 (I2C1SDA) as Open Drain
	GPIOA->PCTL |= 0x33000000; //PA6 and PA7 selected as I2C peripheric (function 3)
	}
	
	hal_i2c_enable_master(i2cx);  //setup device as Master
	hal_i2c_setup_speed(i2cx);  //setup device speed	
}

//Receive bytes from I2C device at specif memory address and store in buffer
uint8_t hal_i2c_mem_read(I2C0_Type* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){

    //Write device address
    hal_i2c_write_slave_address(i2cx, dev_add);

	  //Write memory address
	  hal_i2c_write_data(i2cx, mem_add);
	
	  /* While the bus is busy */
  	while((i2cx->MCS & I2C_REG_MCS_BUSBSY)){};
	
    //Send command to transmit
	  hal_i2c_cmd_tx_nostop(i2cx);
	
    //Wait for busy is clear
		while((i2cx->MCS & I2C_REG_MCS_BUSY)){};

    //Write device address with Receive BIT Set
	  hal_i2c_write_slave_address(i2cx, dev_add | 0x01);
	
    //Send command to transmit
	  hal_i2c_cmd_tx_nostop(i2cx);
			
		//Wait for busy is clear
		while((i2cx->MCS & I2C_REG_MCS_BUSY)){};

    /* While there is data to be read */
    while (len)
    {
      /* Read a byte from the device */
        *value = hal_i2c_receive_data(i2cx);

        /* Point to the next location where the byte read will be saved */
        value++;

        /* Decrement the read bytes counter */
        len--;
			
			  if (len == 1)
        {
            /* Disable Acknowledgement */
            /* Send STOP Condition */
            hal_i2c_cmd_tx_stop(i2cx);
        }
				else 
				{
					hal_i2c_cmd_tx_run(i2cx);
				}

        //Wait for busy is clear
		     while((i2cx->MCS & I2C_REG_MCS_BUSY)){};
			
           
    }   
	return 0;
}


//Transmit bytes to I2C device at specif memory address from a buffer
uint8_t hal_i2c_mem_write(I2C0_Type* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){


  //Write device address
  hal_i2c_write_slave_address(i2cx, dev_add);
	
  //Write memory address
	hal_i2c_write_data(i2cx, mem_add);
	
  //Wait for bus is clear
	while((i2cx->MCS & I2C_REG_MCS_BUSBSY)){};
	
	//Send command to transmit
	hal_i2c_cmd_tx_nostop(i2cx);
	
  //Wait for busy is clear
	while((i2cx->MCS & I2C_REG_MCS_BUSY));
		
	while(len)
	{
	hal_i2c_write_data(i2cx, (*value++));
	if (len > 1) {
		 hal_i2c_cmd_tx_run(i2cx); //keep running
	   }
	else {
	   hal_i2c_cmd_tx_stop(i2cx); //send stop
	   }
	
	while((i2cx->MCS & I2C_REG_MCS_BUSY)){};
	len--;	
	}//end of while
	
	
	return 0;
	
}

