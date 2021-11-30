# LoRa-E5-Mini-EndNode with I2C

## Contents

This is a LoRaWAN end node example for the Seeed LoRa-E5 Mini dev board using
v1.1.0 of the FW. The code provided by Seeed is based on v1.0.0 and
running the STM32 code generation tool Stm32CubeMx undoes some of the
necessary changes to the code which renders the code with many compilation
errors and practically useless.

When testing this device, my test setup included 2 types of sensors:

* AHT20 temperatur & humidity  (I use the
  [Adafruit breakout](https://www.adafruit.com/product/4566)
  This sensor uses I2C.
* PMS - Particle "counter" which communicates using UART. 
  TODO: search reference for particle sensor.

These sensors can be activated in the code by defining

	#define USE_AHT20_SENSOR
	#define USE_PMS_SENSOR
	
in `Core/inc/sys_conf.h`. Without these defines the sensors are not
used and random numbers are transmitted over Lorawan instead, this
is the default setting now.

I have set up this project for the Nucleo STM32WL55 board from scratch
like it is done in v.1.1.0. There are some important changes with
respect to v.1.0.0, CubeMX now only creates a skeleton and the user
adds all necessary code as "user code" to get a functional application.
This renders the code generation tool more versatil and user code and
code automatically generated are clearly separated.

The necessary changes have then been included for the custom board which
differs from the Nucleo board mainly in number of LEDs (1), buttons (1) 
and pins necessary to control the radio switch (2 instead of 3).

These code changes have been obtained from the original Seeed 
v.1.0.0 version at https://github.com/seeed-lora/LoRa-E5-LoRaWAN-End-Node.git
and have been included as "user code". 

The I2C2 and UART peripherals have been added for communicating with 
external sensors. User code is provided for initializing and reading 
the AHT20 sensor (similar to AHT20 arduino library) and for reading out 
the PMS sensor over UART.

Data is transferred over LoRaWAN in CayenneLPP format, as 

* 10 * temperature in celsius
* 10 * relative humidity 

## Application Setup 

There are 2 ways for changing/configuring this application: 

* Automatic code generation by running CubeMX (click on the .ioc file,
  configure your project and then run code creation). This only affects
  code outside of the ranges
  
    /* USER CODE BEGIN xxx */ 
    ...
    /* USER CODE END xxx */
 
  You would do this for example if you want to add/remove some peripheral like 
  UART or I2C. You also can configure LoraWan parameters with CubeMX.
* Edit the files manually, but be careful to introduce your changes
  ONLY between `USER CODE BEGIN` and `USER CODE END`.
  This is usually your application specific code. 

This separation between generated and user code is what not works well
in the code provided by Seeed and was the reason to make this app.

A little hint: Commit your changes before you run the code generation and
check with `git status` and `git diff` what the code generation tool has 
changed. This sometimes reveals that you have written "user code" outside 
of the specified ranges and that the code generator has just removed this
code. 

When on Linux I run `dos2unix` on all text files after code generation. 
Otherwise `git status` returns all files, just because of the line endings 
generated by the tool. I have not yet found a way to configure this in 
STM32CubeIDE.

## LoRaWan settings

### Settings in "The Things Network" (TTN)
* TTN "AppEUI" corresponds to LORAWAN_JOIN_EUI in se-identity.h
* TTN "AppKEY" corresponds to LORAWAN_APP_KEY in se-identity.h
For your device, in "Payload formatters" use "CayenneLPP".

## License

This software component is licensed by ST under Ultimate Liberty license
SLA0044, the "License";  https://www.st.com/SLA0044.

## Links

* [The Board](https://wiki.seeedstudio.com/LoRa_E5_mini/)
* [Application Note AN5406](https://www.st.com/resource/en/application_note/dm00660451-how-to-build-a-lora-application-with-stm32cubewl-stmicroelectronics.pdf)
* [AT commands for stock firmware](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20AT%20Command%20Specification_V1.0%20.pdf)
* [Post in ST community about v1.1.0 vs. v1.0.0](https://community.st.com/s/question/0D53W00000zGZk4SAG/how-to-on-stm32cubefwwlv110)
* [AHT20 datasheet](https://cdn-learn.adafruit.com/assets/assets/000/091/676/original/AHT20-datasheet-2020-4-16.pdf?1591047915)
* [AHT20 arduino library](https://github.com/adafruit/Adafruit_AHTX0)
