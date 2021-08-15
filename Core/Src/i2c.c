/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "sys_app.h"
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20303E5D;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */
  /** Initializes the peripherals clocks
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PA15     ------> I2C2_SDA
    PB15     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PA15     ------> I2C2_SDA
    PB15     ------> I2C2_SCL
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

// helper to get status byte from AHT20
uint8_t getAHT20Status()
{
	if (HAL_OK != HAL_I2C_Master_Receive(&hi2c2, AHT20_I2C_ADDRESS<<1, aRxBuffer, 1, 100))
	{
		return 0xFF;
	}
	return aRxBuffer[0];
}

// reset and calibrate unit. Returns true if successful
bool initAHT20()
{
	//APP_LOG(TS_ON, VLEVEL_L, "CHECK DEVICE READY\r\n");
	if (HAL_OK !=  HAL_I2C_IsDeviceReady(&hi2c2, AHT20_I2C_ADDRESS<<1,3,100))
	{
		return false;
	}

	// send soft reset command
	aTxBuffer[0] = AHT20_CMD_SOFTRESET;
	//APP_LOG(TS_ON, VLEVEL_L, "SOFT RESET\r\n");
	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, AHT20_I2C_ADDRESS<<1, aTxBuffer, 1, 100))
	{
		return false;
	}
	HAL_Delay(20);
	//APP_LOG(TS_ON, VLEVEL_L, "CALIBRATE\r\n");
	// send calibration command
	aTxBuffer[0]=AHT20_CMD_CALIBRATE;
	aTxBuffer[1]=0x08;
	aTxBuffer[2]=0x00;
	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, AHT20_I2C_ADDRESS<<1, aTxBuffer, 3, 100))
	{
		return false;
	}

	// wait while calibrating
	while (getAHT20Status() & AHT20_STATUS_BUSY) {
		HAL_Delay(10);
	}

	// check if calibrated
	if (! (aRxBuffer[0] & AHT20_STATUS_CALIBRATED)) {
		return false;
	}
	return true;
}

// read humidity and temperature from sensor
bool readAHT20(float *humidity, float *temperature)
{
	//APP_LOG(TS_ON, VLEVEL_L, "PREPARE CMD\r\n");
	// send calibration command
	aTxBuffer[0]=AHT20_CMD_TRIGGER;
	aTxBuffer[1]=0x33;
	aTxBuffer[2]=0x00;
	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, AHT20_I2C_ADDRESS<<1, aTxBuffer, 3, 100))
	{
		return false;
	}
	// wait while reading
	//APP_LOG(TS_ON, VLEVEL_L, "WAIT\r\n");
	while (getAHT20Status() & AHT20_STATUS_BUSY) {
		HAL_Delay(10);
	}
	//APP_LOG(TS_ON, VLEVEL_L, "WAIT DONE\r\n");

	// read 6 bytes
	if (HAL_OK != HAL_I2C_Master_Receive(&hi2c2, AHT20_I2C_ADDRESS<<1, aRxBuffer, 6, 100))
	{
		return false;
	}
	uint32_t h = aRxBuffer[1];
	h <<= 8;
	h |= aRxBuffer[2];
	h <<= 4;
	h |= aRxBuffer[3] >> 4;
	*humidity = ((float)h*100) / 0x100000;

	uint32_t t = aRxBuffer[3] & 0x0F;
	t <<= 8;
	t |= aRxBuffer[4];
	t <<= 8;
	t |= aRxBuffer[5];
	*temperature = ((float)t*200 / 0x100000) - 50;

	return true;
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
