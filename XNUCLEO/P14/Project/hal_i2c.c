/*Meu Hal I2C */
#include "hal_i2c.h"
#include "my_tick.h"

/****************** Help Functions ******************/

//Enable the I2C peripheral
void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_ENABLE_I2C;
}

//Disable The I2C peripheral
void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 &= ~I2C_REG_CR1_ENABLE_I2C;
}

//Set the own address for I2C peripheral
void hal_i2c_set_own_address1(I2C_TypeDef *i2cx, uint32_t own_address)
{
	i2cx->OAR1 &= ~( 0x7f << 1);
	i2cx->OAR1 |=  (own_address << 1);
}

//Manage clock strech
void hal_i2c_manage_clock_stretch(I2C_TypeDef *i2cx, uint32_t no_stretch)
{
	if(no_stretch)
	{
		i2cx->CR1 |= I2C_REG_CR1_NOSTRETCH;
	}else
	{
		i2cx->CR1 &= ~I2C_REG_CR1_NOSTRETCH;
	}
}

//Configure the clock configuration register
void hal_i2c_configure_ccr(I2C_TypeDef *i2cx, uint32_t pclk, uint32_t clkspeed, uint32_t duty_cycle)
{
	//double Thigh, Tlow,  Tpclk;
	uint32_t ccr;
	if(clkspeed <= 100000)
	{
		/* configure ccr for standard mode */
		ccr = ( pclk * 1000000) / (clkspeed << 1);
	}else
	{
		 if(duty_cycle == I2C_FM_DUTY_2)
		 {
			 ccr = ( pclk * 1000000)/(3 * clkspeed);
			 ccr = I2C_REG_CCR_ENABLE_FM | ccr;
		 }
		 if(duty_cycle == I2C_FM_DUTY_16BY9)
		 {
			 /* this is to reach 400khz in fm mode */
			ccr = ( pclk * 1000000)/(25 * clkspeed);
			ccr = I2C_REG_CCR_ENABLE_FM | I2C_REG_CCR_DUTY | ccr;
		 }	
	}
	
	i2cx->CCR |= ccr;

}
	
//Configure rise time
void hal_i2c_rise_time_configuration(I2C_TypeDef *i2cx,uint32_t freqrange, uint32_t ClockSpeed)
{
	  /*---------------------------- I2Cx TRISE Configuration --------------------*/
  /* Configure I2Cx: Rise Time */
//  hi2c->Instance->TRISE = I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed);
	
	uint32_t trise;
	if( ClockSpeed <= 100000)
	{
		trise = freqrange +1;
	}else
	{
		trise = (((freqrange * 300) / 1000) + 1);
	}

		i2cx->TRISE  &= ~(0x3F);
		i2cx->TRISE  |= trise;
}

//Configure the Intial Clock
void hal_i2c_clk_init(I2C_TypeDef *i2cx, uint32_t clkspeed, uint32_t duty_cycle)
{
	//uint32_t pclk = I2C_PERIPHERAL_CLK_FREQ_8MHZ;
	uint32_t pclk = I2C_PERIPHERAL_CLK_FREQ_36MHZ;
	i2cx->CR2 |= (pclk );
	hal_i2c_rise_time_configuration(i2cx,pclk, clkspeed);
	hal_i2c_configure_ccr(i2cx,pclk,clkspeed,duty_cycle);
}

//Define the addressing mode
void hal_i2c_set_addressing_mode(I2C_TypeDef *i2cx, uint32_t adr_mode)
{
	if(adr_mode == I2C_ADDRMODE_10BI)
		i2cx->OAR1 |= I2C_REG_OAR1_ADDRMODE;
	else 
		i2cx->OAR1 &= ~I2C_REG_OAR1_ADDRMODE;
}

//Define the duty cycle for Fast Mode
void hal_i2c_set_fm_mode_duty_cycle(I2C_TypeDef *i2cx, uint32_t duty_cycle)
{
	if(duty_cycle == I2C_FM_DUTY_16BY9 )
	{
     i2cx->CCR |= I2C_REG_CCR_DUTY;
	}else
  {
     i2cx->CCR &= ~I2C_REG_CCR_DUTY;
	}		
	
}

//Enable/Disable ACK
void hal_i2c_manage_ack(I2C_TypeDef *i2cx, uint32_t ack_noack)
{
	if(ack_noack == I2C_ACK_ENABLE)
		i2cx->CR1 |= I2C_REG_CR1_ACK;
	else
		i2cx->CR1 &= ~I2C_REG_CR1_ACK;
}

//Generate START Condition
void hal_i2c_generate_start_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_START_GEN;
}

//Generate STOP Condition
void hal_i2c_generate_stop_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_STOP_GEN;
}

//Enable/Disable TX/RX Interrupt
void hal_i2c_configure_tx_rx_interrupt(I2C_TypeDef *i2cx, uint32_t val)
{
	if(val)
	  i2cx->CR2 |= I2C_REG_CR2_BUF_INT_ENABLE;
	else
		i2cx->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
}

//Enable/Disable Error Interrupt
void hal_i2c_configure_error_interrupt(I2C_TypeDef *i2cx, uint32_t val)
{
	
	if(val)
	  i2cx->CR2 |= I2C_REG_CR2_ERR_INT_ENABLE;
	else
		i2cx->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
}

//Enable/Disable Event Interrupt
void 	hal_i2c_configure_evt_interrupt(I2C_TypeDef *i2cx, uint32_t val)
{
		if(val)
			i2cx->CR2 |= I2C_REG_CR2_EVT_INT_ENABLE;
	  else
		  i2cx->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
}

//Check if BUSY Flag is set
uint8_t is_bus_busy(I2C_TypeDef *i2cx)
{
	if(i2cx->SR2 & I2C_REG_SR2_BUS_BUSY_FLAG )
		return 1; //is Busy
	else
		return 0;
}

//Wait for SB Bit 
uint8_t i2c_wait_untill_sb_set(I2C_TypeDef *i2cx)
{
	//EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
	
	if (i2cx->SR1 & I2C_REG_SR1_SB_FLAG )
	{
		return 1 ;
	}
	return 0;
}

//Wait for Address Bit
HAL_StatusTypeDef i2c_wait_untill_addr_set(i2c_handle_t *handle,uint32_t Timeout, uint32_t Tickstart)
{
	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	while(!(handle->Instance->SR1 & I2C_REG_SR1_ADDR_SENT_FLAG) )
	{
		/* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((GetMyTick()-Tickstart) > Timeout))
      {
        handle->ErrorCode |= HAL_TIMEOUT;
        handle->State = HAL_I2C_STATE_READY;
        return HAL_TIMEOUT;
      }
		}
	}
	return HAL_OK;
}

//Send the Address
void hal_i2c_send_addr_first(I2C_TypeDef *i2cx, uint8_t address)
{
	
	i2cx->DR = address;
	
}

//Clear Address Flag
void clear_addr_flag(I2C_TypeDef *i2cx)
{
	uint16_t val;
	
	val = i2cx->SR1;
	val = i2cx->SR2;
	
}

//wait for TXE
HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(i2c_handle_t *handle,uint32_t Timeout, uint32_t Tickstart)
{
	while(!(handle->Instance->SR1 & I2C_REG_SR1_TXE_FLAG) )
	{
		/* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((GetMyTick()-Tickstart) > Timeout))
      {
        handle->ErrorCode |= HAL_TIMEOUT;
        handle->State= HAL_I2C_STATE_READY;
        return HAL_TIMEOUT;
      }
		}
	}
	return HAL_OK;
}

//wait for BTF
HAL_StatusTypeDef I2C_WaitOnBTFFlagUntilTimeout(i2c_handle_t *handle,uint32_t Timeout, uint32_t Tickstart)
{
	while(!(handle->Instance->SR1 & I2C_REG_SR1_BTF_FLAG) )
	{
		/* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((GetMyTick()-Tickstart) > Timeout))
      {
        handle->ErrorCode |= HAL_TIMEOUT;
        handle->State= HAL_I2C_STATE_READY;
        return HAL_TIMEOUT;
      }
		}
	}
	return HAL_OK;
}

//Send Memory Address for a device
HAL_StatusTypeDef I2C_RequestMemoryWrite(i2c_handle_t *handle, uint16_t slave_address, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)
{
/* Generate Start */
  handle->Instance->CR1 |= I2C_CR1_START;

  /* Wait until SB flag is set */
	while(! i2c_wait_untill_sb_set(handle->Instance) ) {}
  

  /* Send slave address */
  hal_i2c_send_addr_first(handle->Instance,slave_address);

  /* Wait until ADDR flag is set */
  if(i2c_wait_untill_addr_set(handle, Timeout, Tickstart) != HAL_OK)
  {
    if(handle->ErrorCode == HAL_I2C_ERROR_AF)
    {
      return HAL_ERROR;
    }
    else
    {
      return HAL_TIMEOUT;
    }
  }

  /* Clear ADDR flag */
  clear_addr_flag(handle->Instance);

  /* Wait until TXE flag is set */
	if (I2C_WaitOnTXEFlagUntilTimeout(handle, Timeout, Tickstart) != HAL_OK)
  {
    if(handle->ErrorCode == HAL_I2C_ERROR_AF)
    {
      /* Generate Stop */
      handle->Instance->CR1 |= I2C_CR1_STOP;
      return HAL_ERROR;
    }
    else
    {
      return HAL_TIMEOUT;
    }
  }

  /* If Memory address size is 8Bit */
  if(MemAddSize == 1) //8 bits memory address size
  {
    /* Send Memory Address */
    handle->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }
  /* If Memory address size is 16Bit */
  else
  {
    /* Send MSB of Memory Address */
    handle->Instance->DR = I2C_MEM_ADD_MSB(MemAddress);

    /* Wait until TXE flag is set */
		if (I2C_WaitOnTXEFlagUntilTimeout(handle, Timeout, Tickstart) != HAL_OK)
    {
      if(handle->ErrorCode == HAL_I2C_ERROR_AF)
      {
        /* Generate Stop */
        handle->Instance->CR1 |= I2C_CR1_STOP;
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Send LSB of Memory Address */
    handle->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }

  return HAL_OK;	
	
}


/****************** Public Functions ******************/
//Configure the peripheral
void hal_i2c_init(i2c_handle_t *handle)
{
	hal_i2c_clk_init(handle->Instance, handle->Init.ClockSpeed,handle->Init.DutyCycle);
	hal_i2c_set_addressing_mode(handle->Instance, handle->Init.AddressingMode);
	hal_i2c_manage_ack(handle->Instance, handle->Init.ack_enable);
	hal_i2c_manage_clock_stretch(handle->Instance,handle->Init.NoStretchMode);
	hal_i2c_set_own_address1(handle->Instance,handle->Init.OwnAddress1);
}

//Master TX
void hal_i2c_master_tx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
  uint32_t timenow, timeout;
  timeout	= 5;  //timeout 5ms
	
	hal_i2c_enable_peripheral(handle->Instance);
	
	/* doesnt care for PE = 0 */
	timenow = GetMyTick();
	while ((GetMyTick()-timenow) < timeout)
	{
	if(is_bus_busy(handle->Instance) ){}
    else timeout = 0;		 
	}
	
	
	 /* Disable Pos */
  handle->Instance->CR1 &= ~I2C_CR1_POS;

	handle->State = HAL_I2C_STATE_BUSY_TX;
	
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	
	
		
  hal_i2c_generate_start_condition(handle->Instance);
	
	/* wait till sb is set */
	
	while(! i2c_wait_untill_sb_set(handle->Instance) );
	
	//clear_sb_flag(); ???
	
	hal_i2c_send_addr_first(handle->Instance,slave_address);
	
	
	//while ( ! i2c_wait_untill_addr_set(handle->Instance) ); */ O ADDR bit é limpo logo após carregar o DR */
	
	clear_addr_flag(handle->Instance); // IS THIS really needed ??
	
	/* enable the buff, err , event interrupts */
	hal_i2c_configure_tx_rx_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);
	hal_i2c_configure_evt_interrupt(handle->Instance,1);
}

//Master RX
void hal_i2c_master_rx(i2c_handle_t *handle, uint8_t slave_addr, uint8_t *buffer, uint32_t len)
{
	hal_i2c_enable_peripheral(handle->Instance);
	
	while(is_bus_busy(handle->Instance) );
	
	handle->Instance->CR1 &= ~I2C_CR1_POS;
	
	handle->State = HAL_I2C_STATE_BUSY_RX;
	
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	
	handle->Instance->CR1 |= I2C_CR1_ACK;
	
	hal_i2c_generate_start_condition(handle->Instance);
	
	/* wait till sb is set */
	
	
	while(! i2c_wait_untill_sb_set(handle->Instance) );
	
	//clear_sb_flag(); ???
	
	hal_i2c_send_addr_first(handle->Instance,slave_addr);
	
	//while ( ! i2c_wait_untill_addr_set(handle, Timeout, Tickstart) );
	
	clear_addr_flag(handle->Instance); // IS THIS really needed ??
	

	/* Enable the buff, err , event interrupts */
	hal_i2c_configure_tx_rx_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);
	hal_i2c_configure_evt_interrupt(handle->Instance,1);
}

//Slave TX
void hal_i2c_slave_tx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
	hal_i2c_enable_peripheral(handle->Instance);
	
	//while(is_bus_busy(handle->Instance) );
	
	handle->Instance->CR1 &= ~I2C_CR1_POS;
	
	handle->State = HAL_I2C_STATE_BUSY_TX;
	
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	
	 /* Enable Address Acknowledge */
	handle->Instance->CR1 |= I2C_CR1_ACK;
	
			/* ENABLE the buff, err , event interrupts */
	hal_i2c_configure_tx_rx_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);
	hal_i2c_configure_evt_interrupt(handle->Instance,1);
	
}

//Slave RX
void hal_i2c_slave_rx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
	//uint32_t val;
	
	hal_i2c_enable_peripheral(handle->Instance);
	
	//while(is_bus_busy(handle->Instance) );
	
	handle->Instance->CR1 &= ~I2C_CR1_POS;
	handle->State = HAL_I2C_STATE_BUSY_RX;
	
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	
	
	
	handle->Instance->CR1 |= I2C_CR1_ACK;
	
		/* disable the buff, err , event interrupts */
	hal_i2c_configure_tx_rx_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);
	hal_i2c_configure_evt_interrupt(handle->Instance,1);

//#if 0
//	val = handle->Instance->CR2;
//	val = handle->Instance->CR1;
//	val = handle->Instance->OAR1;
//#endif 
}

//Master TX using Memory device address
HAL_StatusTypeDef HAL_I2C_Mem_Write(i2c_handle_t *handle, uint8_t slave_address, uint8_t mem_address, uint8_t mem_len, uint8_t *buffer, uint32_t len, uint32_t Timeout)
{
  uint32_t tickstart = 0x00U;
  
  /* Init tickstart for timeout management*/
  tickstart = GetMyTick();
	
	hal_i2c_enable_peripheral(handle->Instance);
	
	/* doesnt care for PE = 0 */
	while(is_bus_busy(handle->Instance) ) {}
		
	 /* Disable Pos */
  handle->Instance->CR1 &= ~I2C_CR1_POS;

	handle->State = HAL_I2C_STATE_BUSY_TX;
	
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	
 if(I2C_RequestMemoryWrite(handle, slave_address, mem_address, mem_len, Timeout, tickstart) != HAL_OK)
 {
	 return HAL_ERROR;
 }
		
 while(handle->XferSize > 0U)
    {
      /* Wait until TXE flag is set */
      if (I2C_WaitOnTXEFlagUntilTimeout(handle, Timeout, tickstart) != HAL_OK)
      {
        if(handle->ErrorCode == HAL_I2C_ERROR_AF)
        {
          /* Generate Stop */
          handle->Instance->CR1 |= I2C_CR1_STOP;
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }

      /* Write data to DR */
      handle->Instance->DR = (*handle->pBuffPtr++);
      handle->XferSize--;
      handle->XferCount--;

      if((handle->Instance->SR1 & I2C_REG_SR1_BTF_FLAG) && (handle->XferSize != 0U))
      {
        /* Write data to DR */
        handle->Instance->DR = (*handle->pBuffPtr++);
        handle->XferSize--;
        handle->XferCount--;
      }
    }
    
    /* Wait until BTF flag is set */
    if(I2C_WaitOnBTFFlagUntilTimeout(handle, Timeout, tickstart ) != HAL_OK)
    {
      if(handle->ErrorCode == HAL_I2C_ERROR_AF)
      {
        /* Generate Stop */
        handle->Instance->CR1 |= I2C_CR1_STOP;
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Generate Stop */
    handle->Instance->CR1 |= I2C_CR1_STOP;

    handle->State = HAL_I2C_STATE_READY;
    return HAL_OK;
  
}
