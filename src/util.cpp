#include "util.h"

/*
 * Setup the core system clock.
 */
void setup_clocks(void) {
  #ifdef STARm_F1
    // Set 2 wait states in flash and enable the prefetch buffer.
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    FLASH->ACR |=  (FLASH_ACR_LATENCY_2 |
                    FLASH_ACR_PRFTBE);
    // Enable the HSE oscillator.
    // (This will be an infinite loop if your board doesn't have
    //  an HSE oscillator, but most cheap F103C8 boards seem to have one.)
    RCC->CR    |=  (RCC_CR_HSEON);
    while (!(RCC->CR & RCC_CR_HSERDY)) {};
    // Set the HSE oscillator as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_HSE);
    // Set the PLL multiplication factor to 9, for 8*9=72MHz.
    RCC->CFGR  &= ~(RCC_CFGR_PLLMULL);
    RCC->CFGR  |=  (RCC_CFGR_PLLMULL9);
    // Enable the PLL.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Set the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    // The core clock is now 72MHz.
    core_clock_hz = 72000000;
  #elif STARm_F3
    // TODO.
    core_clock_hz = 8000000;
  #endif
}
