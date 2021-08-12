# LoRa-E5-Mini-EndNode

This is a LoRaWAN end node example for the Seeed LoRa-E5 Mini dev board using
v1.1.0 of the FW. The code provided by Seeed is based on v1.0.0 and
running the STM32 code generation tool Stm32CubeMx undoes some of the
necessary changes to the code which renders the code with compile errors.

I have set up this project for the Nucleo STM32WL55 board from zero
like it is done in v.1.1.0. There are some important changes with
respect to v.1.0.0, CubeMX now only creates a skeleton and the user
adds all necessary code as "user code" to get a functional application.

This renders the code generation tool more versatil and user code and
code automatically generated are clearly separated.

I then have included the necessary changes for the custom board which
differs from the Nucleo board mainly in number of LEDs (1), 
buttons (1) and pins
necessary to control the radio switch (2 instead of 3).

These code changes have been obtained from the original Seeed 
v.1.0.0 version at
https://github.com/seeed-lora/LoRa-E5-LoRaWAN-End-Node.git
and have been included as "user code". 

It should now be easy to add new peripherals like I2C to talk
to sensors etc. It should also be
possible to use the skeleton for creating a "AT Slave" compatible
with the Seeed board.

This software component is licensed by ST under Ultimate Liberty license
SLA0044, the "License";  https://www.st.com/SLA0044
