#include "main.h"

/**
 * 'Blink LED' task.
 */
static void led_task(void *args) {
  int delay_ms = *(int*)args;

  while (1) {
    board_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
  }
}

/**
 * 'Count value' task.
 */
static void count_task(void *args) {
  int delay_ms = *(int*)args;

  while (1) {
    // Count up and draw to the display.
    ++count_val;
    oled.draw_rect(68, 28, 34, 8, 0, 0);
    oled.draw_letter_i(70, 29, count_val, 1, 'S');
    // Delay for a second-ish.
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
  };
}

/**
 * 'Draw to OLED Display' task.
 */
static void oled_display_task(void *args) {
  int delay_ms = *(int*)args;

  while(1) {
    // Stream the framebuffer to the display, then
    // wait a bit before refreshing again.
    oled.draw_framebuffer();
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
  };
}

/**
 * Main program.
 */
int main(void) {
  // Call C++ static initializers.
  // ('preinit_array' functions are unlikely if the user
  //  doesn't define any, I think. But check for them anyways.)
  int cpp_count = 0;
  int cpp_size = &(_epreinit_array[0]) - &(_spreinit_array[0]);
  for (cpp_count = 0; cpp_count < cpp_size; ++cpp_count) {
    _spreinit_array[cpp_count]();
  }
  // ('init_array' sections call static constructors)
  cpp_size = &(_einit_array[0]) - &(_sinit_array[0]);
  for (cpp_count = 0; cpp_count < cpp_size; ++cpp_count) {
    _sinit_array[cpp_count]();
  }

  // Initial clock setup.
  setup_clocks();

  // Initialize the LED pin's GPIO bank.
  led_gpio = pGPIO(LED_BANK);
  led_gpio.clock_en();
  // Initialize the LED pin.
  board_led = pGPIO_pin(&led_gpio, LED_PIN, pGPIO_OUT_PP);
  // Initialize the I2C GPIO bank.
  if (I2C_BANK == LED_BANK) {
    // Use an already-initialized GPIO class if available.
    i2c_gpio = led_gpio;
  }
  else {
    // Otherwise, create a new one.
    i2c_gpio = pGPIO(I2C_BANK);
    i2c_gpio.clock_en();
  }
  // Initialize the I2C GPIO pins.
  #if   defined(STARm_F3)
    sda_gpio = pGPIO_pin(&i2c_gpio, SDA_PIN, pGPIO_AF_OD_PULLUP);
    scl_gpio = pGPIO_pin(&i2c_gpio, SCL_PIN, pGPIO_AF_OD_PULLUP);
    sda_gpio.set_alt_func(4);
    scl_gpio.set_alt_func(4);
  #elif STARm_F1
    sda_gpio = pGPIO_pin(&i2c_gpio, SDA_PIN, pGPIO_AF_OD);
    scl_gpio = pGPIO_pin(&i2c_gpio, SCL_PIN, pGPIO_AF_OD);
  #endif
  // Initialize the I2C peripheral.
  i2c1 = pI2C(I2C1);
  i2c1.reset();
  i2c1.clock_en();
  i2c1.i2c_init();
  // Initialize the SSD1306 OLED display.
  oled = pSSD1306(&i2c1, 0x78, 128, 64);
  oled.init_display();
  // Draw an initial display image to the framebuffer.
  oled.draw_rect(0, 0, 128, 64, 0, 0);
  oled.draw_rect(0, 0, 128, 64, 4, 1);
  oled.draw_text(28, 29, "Count:\0", 1, 'S');

  // Create a blinking LED task for the on-board LED.
  xTaskCreate(led_task, "Blink_LED", 128, (void*)&led_delay,
              configMAX_PRIORITIES-7, NULL);
  // Create the OLED counting/display tasks.
  xTaskCreate(count_task, "Count_Up",
              128, (void*)&count_delay,
              configMAX_PRIORITIES-6, NULL);
  xTaskCreate(oled_display_task, "OLED_Display",
              128, (void*)&display_delay,
              configMAX_PRIORITIES-5, NULL);
  // Start the scheduler.
  vTaskStartScheduler();

  // This should never be reached; the FreeRTOS scheduler should
  // be in charge of the program's execution after starting.
  while (1) {}
  return 0;
}
