// Header:
// File Name: hal_i2c.c
// Author: Jose P. Leitao
// Date: 2019-04-13

/* Implement some level of abstraction for I2C communication */

#include "hal_i2c.h"

#define I2C_SPEED   100000UL

void hal_i2c_init(I2C_TypeDef* i2cx){
	I2C_InitTypeDef MyI2C_init;
	GPIO_InitTypeDef gpio_init;
	
	/* GPIO Ports Clock Enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/*pins PB8 (SCL1) PB9 (SDA1) */
	gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
		  
	GPIO_Init(GPIOB, &gpio_init);
	GPIO_PinRemapConfig( GPIO_Remap_I2C1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_StructInit(&MyI2C_init); //initialize with default values
	
	MyI2C_init.I2C_ClockSpeed = I2C_SPEED;  //standart mode
	MyI2C_init.I2C_Ack = I2C_Ack_Enable; //Enable ACK
	
	I2C_Init(i2cx, &MyI2C_init);
	
}

//Receive bytes from I2C device at specif memory address and store in buffer
uint8_t hal_i2c_mem_read(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){

    /* While the bus is busy */
    while (I2C_GetFlagStatus(i2cx, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(i2cx, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send device address and write command */ 
    I2C_Send7bitAddress(i2cx, dev_add, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(i2cx, ENABLE);

    /* Send the internal address to write to */
    I2C_SendData(i2cx, mem_add);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send START condition a second time */
    I2C_GenerateSTART(i2cx, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send device address and read command */
    I2C_Send7bitAddress(i2cx, dev_add, I2C_Direction_Receiver);

   /* Enable Acknowledgement to be ready */
   I2C_AcknowledgeConfig(i2cx, ENABLE);
		
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while (len)
    {
        if (len == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(i2cx, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(i2cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the device */
            *value = I2C_ReceiveData(i2cx);

            /* Point to the next location where the byte read will be saved */
            value++;

            /* Decrement the read bytes counter */
            len--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(i2cx, ENABLE);
    
	return 0;
}

//Receive 2 bytes from I2C device at specif memory address and store in buffer as 16 bits word
uint8_t hal_i2c_mem_read16(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint16_t *value){
	
	uint16_t tmpreg;
	uint8_t rx_buff[2];
	
	I2C_GenerateSTART(i2cx, ENABLE); //send START
	
	/* Test on EV5 and clear it */
  while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_MODE_SELECT));
//  while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_SB)){} //wait Start Bit sent
//	
//	//clear start bit sent
//	tmpreg = i2cx->SR1;
			
	I2C_Send7bitAddress(i2cx, dev_add, I2C_Direction_Transmitter);  //send device address, set TX mode
	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_ADDR)){} //wait for Address sent
	
	//clear address sent flag
	tmpreg = 	i2cx->SR1;
	tmpreg =  i2cx->SR2;  
		
	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_TXE)){} //wait for TXE
	
	I2C_SendData(i2cx, mem_add); //send memory address
	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_BTF)){} //wait for BYTE sent
	
	//clear BYTE sent flag
	tmpreg = 	i2cx->SR1;
		
	I2C_GenerateSTART(i2cx, ENABLE); //send re-START
	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_SB)){} //wait Start Bit sent
	
	//clear start bit sent
	tmpreg = i2cx->SR1;
	
  I2C_AcknowledgeConfig(i2cx, ENABLE);
		
	I2C_Send7bitAddress(i2cx, dev_add, I2C_Direction_Receiver);  //send device address, set RX mode
	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_ADDR)){} //wait for Address sent
	
	//clear address sent flag
	tmpreg = 	i2cx->SR1;
	tmpreg =  i2cx->SR2;  
	
		
	//receive 2 bytes and store at value buffer		
	    //Byte LSB
     	while (!I2C_GetFlagStatus( i2cx, I2C_FLAG_RXNE)){} //wait for RXNE
	    I2C_AcknowledgeConfig(i2cx, DISABLE); //send NAK after 2 bytes received
			I2C_GenerateSTOP(i2cx, ENABLE); //prepare to send STOP
			rx_buff[0] = I2C_ReceiveData(i2cx); //receive LSB byte
			rx_buff[1] = I2C_ReceiveData(i2cx); //receive MSB byte
			//clear BYTE receive flag
	    tmpreg = 	i2cx->SR1;
				
	*value = (uint16_t) ((rx_buff[1] << 8) | rx_buff[0]);
				
	return 0;	
}

//Transmit bytes to I2C device at specif memory address from a buffer
uint8_t hal_i2c_mem_write(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len){

	
	/* While the bus is busy */
  while (I2C_GetFlagStatus(i2cx, I2C_FLAG_BUSY));

	I2C_GenerateSTART(i2cx, ENABLE); //send START
	
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_MODE_SELECT));
			
	I2C_Send7bitAddress(i2cx, dev_add, I2C_Direction_Transmitter);  //send device address
	
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(i2cx, ENABLE);

	/* Send the internal address to write to */
  I2C_SendData(i2cx, mem_add);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	
  //send buffer bytes		
	for (uint8_t j=0; j< len; j++){
     	/* Test on EV8 and clear it */
      while (!I2C_CheckEvent(i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_SendData(i2cx, *value++); //send one byte
      }
	
	I2C_GenerateSTOP(i2cx, ENABLE); //send STOP
	
	return 0;
	
}

