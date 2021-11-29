/*
 * aht20.c
 *
 *  Created on: Aug 28, 2021
 *      Author: claus
 *
 *  I2C interface for the AHT20 sensor, ported from https://github.com/adafruit/Adafruit_AHTX0.
 *  After initializing I2C, set the I2C interface to use by calling for example
 *    setI2CHandle(&hi2c2);
 */
#include "aht20.h"

// pointer to I2C handle, set with setI2CHandle
I2C_HandleTypeDef *pI2CHandle = 0;

// set module pointer to I2C handle address
void setI2CHandle(I2C_HandleTypeDef *pHandle)
{
	pI2CHandle = pHandle;
}

// helper to get status byte from AHT20
uint8_t getAHT20Status()
{
	if (HAL_OK != HAL_I2C_Master_Receive(pI2CHandle, AHT20_I2C_ADDRESS<<1, aRxBuffer, 1, 100))
	{
		return 0xFF;
	}
	return aRxBuffer[0];
}

// reset and calibrate unit, returns true if successful
bool initAHT20()
{
	if (HAL_OK !=  HAL_I2C_IsDeviceReady(pI2CHandle, AHT20_I2C_ADDRESS<<1, 3, 100))
	{
		return false;
	}

	// send soft reset command
	aTxBuffer[0] = AHT20_CMD_SOFTRESET;
	if (HAL_OK != HAL_I2C_Master_Transmit(pI2CHandle, AHT20_I2C_ADDRESS<<1, aTxBuffer, 1, 100))
	{
		return false;
	}
	HAL_Delay(20);

	// send calibration command
	aTxBuffer[0] = AHT20_CMD_CALIBRATE;
	aTxBuffer[1] = 0x08;
	aTxBuffer[2] = 0x00;
	if (HAL_OK != HAL_I2C_Master_Transmit(pI2CHandle, AHT20_I2C_ADDRESS<<1, aTxBuffer, 3, 100))
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
	// trigger measurement command
	aTxBuffer[0]=AHT20_CMD_TRIGGER;
	aTxBuffer[1]=0x33;
	aTxBuffer[2]=0x00;
	if (HAL_OK != HAL_I2C_Master_Transmit(pI2CHandle, AHT20_I2C_ADDRESS<<1, aTxBuffer, 3, 100))
	{
		return false;
	}

	// wait while busy
	while (getAHT20Status() & AHT20_STATUS_BUSY) {
		HAL_Delay(10);
	}

	// read 6 bytes
	if (HAL_OK != HAL_I2C_Master_Receive(pI2CHandle, AHT20_I2C_ADDRESS<<1, aRxBuffer, 6, 100))
	{
		return false;
	}

	// convert to float types
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
