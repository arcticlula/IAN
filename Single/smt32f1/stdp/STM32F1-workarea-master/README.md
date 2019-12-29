# STM32F1 working directory template

This repo provides a full workspace to get you started with the STM32F10x family of controllers. It is aimed at people who prefer to work without an IDE in a simple text editor and want to compile from the command line.

I have added a fixed Makefile which will enable the use of the math functions and sprintf()
and other libc functions from newlib.

This working directory, especially the Makefile is base upon the [STM32F4 Discovery Firmware](http://github.com/nabilt/STM32F4-Discovery-Firmware) by nabilt.

Information on how to set up an arm-none-eabi-gcc toolchain required to compile code for the STM32F microcontrollers can be found [here](http://eliaselectronics.com/stm32f4-tutorials/setting-up-the-stm32f4-arm-development-toolchain/)

Documentation on the peripheral library can be found in the file **stm32f10x_stdperiph_lib_um.chm**

##Subfolders
+ **Project**: contains the example code, place your projects here
+ **Libraries**: contains the STM32F10x peripheral library and CMSIS files
+ **Utilities**: contains more libraries for the STM32F10x evaluation boards

Examples:

+ **GPIO** example featuring setting and reading from IO pins
+ **WS2812** example on how to drive a WS2812 RGB LED using PWM and DMA

###This code is licensed under the Creative Commons Attribution license v3.0

**This only applies to my example code, all other code by ST and ARM are property of their respective owners and are released under a different license**

This means that you are free:
+ **to Share** — to copy, distribute and transmit the work
+ **to Remix** — to adapt the work
+ to make commercial use of the work

Under the following conditions:
**Attribution** — You must attribute the work in the manner specified by the author or licensor

**Attribution** in this case means that you should please provide a link to [Elia's Electronics](http://eliaselectronics.com/ "My blog")

More information on this license can be found [here](http://creativecommons.org/licenses/by/3.0/ "CC BY")

![CC BY](http://i.creativecommons.org/l/by/3.0/88x31.png)
