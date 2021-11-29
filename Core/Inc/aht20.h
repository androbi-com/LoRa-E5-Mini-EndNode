/*
 * aht20.h
 *
 *  Created on: Aug 28, 2021
 *      Author: claus
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include "i2c.h"
#include "stdint.h"
#include <stdbool.h>

// buffer sizes
#define TXBUFFERSIZE 3
#define RXBUFFERSIZE 6

// I2C constants
#define AHT20_I2C_ADDRESS 0x38
#define AHT20_CMD_TRIGGER 0xAC
#define AHT20_CMD_SOFTRESET 0xBA
#define AHT20_CMD_CALIBRATE 0xBE
#define AHT20_STATUS_BUSY 0x80
#define AHT20_STATUS_CALIBRATED 0x08

// transmission buffers
uint8_t aTxBuffer[TXBUFFERSIZE];
uint8_t aRxBuffer[RXBUFFERSIZE];

// public interface
void setI2CHandle(I2C_HandleTypeDef *pHandle);
bool initAHT20();
bool readAHT20(float *humidity, float *temperature);

#endif /* INC_AHT20_H_ */
