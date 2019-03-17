// Header:
// File Name: i2c_driver.c
// Author: José P. Leitão
// Date: 2019-03-12

/*Driver I2C para TM4C123 
  contem funções básicas necessárias para trabalhar com o Display OLED baseado no SD1306 */
	
#include "i2c_driver.h"
#include "my_tick.h"

/****************** Help Functions ******************/

//setup device speed
void hal_i2c_setup_speed(i2c_handle_t *handle)
{
#define SYSTEM_SPEED_KHZ (uint32_t) 20000   //System Clock in KHZ 1000 KHz = 1MHZ
	uint32_t tpr;
	tpr = (uint32_t)(SYSTEM_SPEED_KHZ/(20*100));  //passar a velocidade do device
	tpr = 0x0000007F & tpr;
	
	if (handle->Init.Mode_Speed == MASTER_HIGH_SPEED)
	 {
	 tpr = I2C_REG_MTPR_HS | tpr;
	 }
	
	handle->Instance->MTPR |= tpr;
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
	i2cx->MSA |= mem_add;
}

//Write data
void hal_i2c_write_data(I2C0_Type *i2cx, uint8_t send_data)
{
	i2cx->MDR |= send_data;
}

//Command to transmit without stop
void hal_i2c_cmd_tx_nostop(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_ACK | I2C_REG_MCS_START | I2C_REG_MCS_RUN;
}

//Command to transmit run
void hal_i2c_cmd_tx_run(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_RUN;
}

//Command to transmit stop
void hal_i2c_cmd_tx_stop(I2C0_Type *i2cx)
{
	i2cx->MCS = I2C_REG_MCS_STOP | I2C_REG_MCS_RUN;
}

//Wait until BUS is clear
HAL_StatusTypeDef wait_for_bus_clear(i2c_handle_t *handle, uint32_t timeout, uint32_t timestart)
{
	while(!(handle->Instance->MCS & I2C_REG_MCS_BUSBSY) )
	{
		/* Check for the Timeout */
    if(timeout != HAL_MAX_DELAY)
    {
      if((timeout == 0U) || ((GetMyTick()-timestart) > timeout))
      {
        handle->ErrorCode |= HAL_TIMEOUT;
        handle->State = HAL_I2C_STATE_READY;
        return HAL_TIMEOUT;
      }
		}
	}
	return HAL_OK;
}

//Wait until BUSY is clear
HAL_StatusTypeDef wait_for_busy_clear(i2c_handle_t *handle, uint32_t timeout, uint32_t timestart)
{
	while(!(handle->Instance->MCS & I2C_REG_MCS_BUSY) )
	{
		/* Check for the Timeout */
    if(timeout != HAL_MAX_DELAY)
    {
      if((timeout == 0U) || ((GetMyTick()-timestart) > timeout))
      {
        handle->ErrorCode |= HAL_TIMEOUT;
        handle->State = HAL_I2C_STATE_READY;
        return HAL_TIMEOUT;
      }
		}
	}
	return HAL_OK;
}

HAL_StatusTypeDef is_error(i2c_handle_t *handle)
{
	if (handle->Instance->MCS & I2C_REG_MCS_ERROR)
	return HAL_ERROR;
	else
	return HAL_OK;
}


/****************** Public Functions ******************/
//Configure the peripheral
void hal_i2c_init(i2c_handle_t *handle)
{
if (handle->Instance == I2C0)
  {
  /*configuration for I2C0  (I2C0SCL = PB2, I2C0SDA = PB3) */
	_HAL_RCGC_I2C0_CLK_ENABLE();   //enable clock on I2C0
	_HAL_RCGC_GPIOB_CLK_ENABLE();  //enable clock on GPIOB
	GPIOB->AFSEL |= (1 <<2) | (1 << 3); //PB2 and PB3 set on AFSEL
	GPIOB->DEN |= (1 << 2) | (1 << 3);  //Digital enable
	GPIOB->ODR |= (1 << 3); //PB3 (I2C0SDA) as Open Drain
  GPIOB->PCTL |= 0x003300; //PB2 and PB3 selected as I2C peripheric (function 3)
	}

if (handle->Instance == I2C1)
  {	
  /*configuration for I2C1 (I2C1SCL = PA6, I2C1SDA = PA7)*/	
	_HAL_RCGC_I2C1_CLK_ENABLE();   //enable clock on I2C1
	_HAL_RCGC_GPIOA_CLK_ENABLE();  //enable clock on GPIOA
	GPIOA->AFSEL |= (1 <<6) | (1 << 7); //PA6 and PA7 set on AFSEL
	GPIOA->DEN |= (1 << 6) | (1 << 7);  //Digital enable
	GPIOA->ODR |= (1 << 7); //PA7 (I2C1SDA) as Open Drain
	GPIOA->PCTL |= 0x33000000; //PA6 and PA7 selected as I2C peripheric (function 3)
	}
	
	hal_i2c_enable_master(handle->Instance);  //setup device as Master
	hal_i2c_setup_speed(handle);  //setup device speed
	
}

//Master TX using a memory address to device
HAL_StatusTypeDef hal_i2c_mem_write(i2c_handle_t *handle, uint8_t slave_address, uint8_t mem_address, uint8_t mem_len, uint8_t *buffer, uint32_t len, uint32_t Timeout)
{
//uint32_t tmpreg;
	uint32_t Timestart = GetMyTick();

//Write device address
  hal_i2c_write_slave_address(handle->Instance, slave_address);
	
//Write memory address
	hal_i2c_write_data(handle->Instance, mem_address);
	
//Wait for bus is clear
	
  while((handle->Instance->MCS & I2C_REG_MCS_BUSBSY));
	
	handle->State = HAL_I2C_STATE_BUSY_TX;
	
//Send command to transmit
	hal_i2c_cmd_tx_nostop(handle->Instance);
	
//Wait for busy is clear
		
	while((handle->Instance->MCS & I2C_REG_MCS_BUSY));
	
	//if no error continue sending buffer
	if (is_error(handle))
	{//rotina de erro
	}

	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
		
	while(handle->XferCount)
	{
	hal_i2c_write_data(handle->Instance, (*handle->pBuffPtr++));
	handle->XferCount--;
	handle->XferSize--;
	hal_i2c_cmd_tx_run(handle->Instance);
	while((handle->Instance->MCS & I2C_REG_MCS_BUSY));
		
	}//end of while
	
	//send STOP
	hal_i2c_cmd_tx_stop(handle->Instance);
	
	if( wait_for_busy_clear(handle, Timeout, Timestart) != HAL_OK)
	{
		handle->State = HAL_I2C_STATE_TIMEOUT;
	}
	
	if (is_error(handle))
	{//rotina de erro
	}
	
	handle->State = HAL_I2C_STATE_READY;
	return HAL_OK;
	
}
