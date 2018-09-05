#ifndef _STARm_GLOBAL_H
#define _STARm_GLOBAL_H

#include <stdio.h>

// Core system headers.
#ifdef STARm_F0
  #include "stm32f0xx.h"
#elif STARm_F1
  #include "stm32f1xx.h"
#elif STARm_F3
  #include "stm32f3xx.h"
#elif STARm_L0
  #include "stm32l0xx.h"
#endif

// FreeRTOS includes.
extern "C" {
  #include "FreeRTOS.h"
  #include "task.h"
}

// Project includes.
#include "core.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"

/* Global variables and defines. */

// Peripheral macro definitions.
// (Platform-agnostic)
#define I2C_BANK   (GPIOB)
#define SDA_PIN    (7)
#define SCL_PIN    (6)
// (Platform-specific)
#ifdef STARm_F1
  #define LED_BANK (GPIOB)
  #define LED_PIN  (12)
#else
  #define LED_BANK (GPIOA)
  #define LED_PIN  (1)
#endif

// Core system clock speed; initial value depends on the chip.
extern volatile uint32_t core_clock_hz;
// Value to show on the display.
extern volatile uint16_t count_val;
// How long to delay between LED blinks (ms).
extern const    int      led_delay;
// OLED-related delay lengths in milliseconds.
extern const    int      count_delay;
extern const    int      display_delay;

// Global peripheral structs.
extern pGPIO     led_gpio;
extern pGPIO     i2c_gpio;
extern pGPIO_pin board_led;
extern pGPIO_pin sda_gpio;
extern pGPIO_pin scl_gpio;
extern pI2C      i2c1;
extern pSSD1306  oled;

#endif
