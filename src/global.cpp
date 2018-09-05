#include "global.h"

// Global definitions.
// Core system clock speed; initial value depends on the chip.
volatile uint32_t core_clock_hz = 8000000;
// Delay between LED blinks.
const    int      led_delay = 500;
// Delay length in milliseconds for counting.
const    int      count_delay = 100;
const    int      display_delay = 50;
// 'Count' number to draw to the OLED display as a test.
volatile uint16_t count_val = 0;

// Peripheral structs.
// On-board LED.
pGPIO     led_gpio;
pGPIO_pin board_led;
// I2C peripheral.
pGPIO     i2c_gpio;
pGPIO_pin sda_gpio;
pGPIO_pin scl_gpio;
pI2C      i2c1;
// SSD1306 OLED display.
pSSD1306  oled;
