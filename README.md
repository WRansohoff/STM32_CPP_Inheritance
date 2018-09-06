# Overview

This is an example object-oriented C++ application for boards using some common STM32 microcontrollers. It uses the I2C peripheral to drive an SSD1306 monochrome OLED display.

This example is intended to demonstrate how to use C++ classes and inheritence in an embedded application; each peripheral and device has its own class, and communication peripherals are derived from a common 'pIO' input/output class.

One difference from C that is particularly worth noting: when you use static objects in C++, you are expected to call those objects' constructors and destructors manually, using the function pointers which the compiler places in special `[pre]init_array` and `fini_array` memory sections. The linker scripts and `main` method reflect this, although the destructors are never called in this example because the application is never expected to exit while the device is powered on.

Currently only 128x64-pixel screens with an address of 0x78 are supported with a single timing value, but I'm hoping to change that sooner or later.

The I2C peripherals are a bit different between the older F103 and the newer F303 (and F0, F4, L4, etc.) chips; I think that I've worked out how to get them running consistently on both, but the timing values are based off a mixture of guesswork and examples listed in ST's reference manuals. Fortunately, the SSD1306 is very forgiving when it comes to timings.

# Code Structure

The peripheral logic is mostly written into the C++ classes under `lib/` to demonstrate the concepts of inheritance in an embedded application. The file names reflect the peripheral or device which they are designed to interact with.

The core program logic is located in `src/`. The `main` header and source files contain the core program structure, while the `global` files contain global declarations and definitions for their initial values. The `util` files currently only hold a method for initializing the chips' core clock speeds.

Device-specific code relating to initial startup, hardware interrupts, linker configurations, and register macros are located in the `boot_s/`, `vector_tables/`, `ld/`, and `device_headers/` directories.

The project is built using the arm-none-eabi GCC toolchain, and a Makefile is provided.

# Boards

Currently, only the STM32F103C8 and STM32F303K8 are supported.

The STM32F103C8 is a high-volume and well-understood chip which recently celebrated its 10th birthday; it can be found on a variety of boards which cost less than $5 each; they are commonly called 'blue pill' or 'black pill' boards on ebay/taobao/aliexpress/amazon/etc.

The STM32F303K8 is a more modern chip with the Cortex-M4F core's hardware instructions for floating-point and DSP operations. It can be found on small 'Nucleo-32' boards sold by ST for about $11. I think that they have the same pinout as the popular 'Arduino Nano' ATMega328-based board.
