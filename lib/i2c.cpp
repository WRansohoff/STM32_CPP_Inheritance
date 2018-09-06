#include "i2c.h"

// Default constructor.
pI2C::pI2C() {}

// Basic constructor; simply set the base I2C registers,
// no timing control or default initialization yet.
pI2C::pI2C(I2C_TypeDef* i2c_regs) {
  i2c = i2c_regs;
  if (i2c_regs == I2C1) {
    enable_reg = STARm_RCC_APB1ENR;
    enable_bit = RCC_APB1ENR_I2C1EN;
    reset_reg  = STARm_RCC_APB1RSTR;
    reset_bit  = RCC_APB1RSTR_I2C1RST;
  }
  else {
    status = pSTATUS_ERR;
    return;
  }
  status = pSTATUS_SET;
}

/*
 * Core I/O 'Read' implementation:
 * Read a byte of data from the I2C bus.
 * (Currently not implemented for STM32F1 lines.)
 */
unsigned pI2C::read(void) {
  #if    defined(STARm_F3)
    // Wait for a byte of data to be available, then read it.
    while (!(i2c->ISR & I2C_ISR_RXNE)) {}
    return (i2c->RXDR & 0xFF);
  #elif  STARm_F1
    // TODO
    return 0x00;
  #endif
}

/*
 * Core I/O 'Write' implementation:
 * Write a byte of data to the I2C bus.
 */
void pI2C::write(unsigned dat) {
  #if    defined(STARm_F3)
    // Transmit a byte of data, and wait for it to send.
    i2c->TXDR = (i2c->TXDR & 0xFFFFFF00) | dat;
    while (!(i2c->ISR & (I2C_ISR_TXIS |
                         I2C_ISR_TC |
                         I2C_ISR_TCR))) {};
  #elif  STARm_F1
    // Transmit a byte of data, and wait for it to send.
    i2c->DR   = (i2c->DR & 0xFF00) | (uint8_t)dat;
    while (!(i2c->SR1 & I2C_SR1_TXE)) {};
  #endif
}

/*
 * Stream a whole bunch of bytes from a buffer.
 * Note: This does not handle setting the device address
 *       or required start/stop conditions.
 */
void pI2C::stream(volatile void* buf, int len) {
  volatile uint8_t *i2cbuf = (volatile uint8_t*) buf;
  #if    defined(STARm_F3)
    // The more recent chips have an internal counter to keep
    // track of how many bytes they send/receive, and it's
    // limited to 255 bytes at once, so we need to manage that.
    // NOTE: The 'RELOAD' flag must be set prior to this call.
    volatile int pos = 0;
    int nb_i = 0;
    // Send 255 bytes at a time, until there's <255B left.
    while ((len - pos) > 255) {
      set_num_bytes(255);
      for (nb_i = 0; nb_i < 255; ++nb_i) {
        write(i2cbuf[pos]);
        ++pos;
      }
    }
    // Stream the remaining bytes.
    if (pos < len) {
      int remaining_bytes = len - pos;
      set_num_bytes(remaining_bytes);
      for (nb_i = 0; nb_i < remaining_bytes; ++nb_i) {
        write(i2cbuf[pos]);
        ++pos;
      }
    }
  #elif  STARm_F1
    // The F1 series have a simpler 'transmit' process
    // for longer frames; you just send all of the bytes.
    int nb_i = 0;
    for (nb_i = 0; nb_i < len; ++nb_i) {
      write(i2cbuf[nb_i]);
    }
  #endif
}

/*
 * Initialize and enable the I2C peripheral.
 */
void pI2C::i2c_init(void) {
  if (status == pSTATUS_ERR) { return; }
  #if defined(STARm_F3)
    // First, disable the peripheral.
    i2c->CR1     &= ~(I2C_CR1_PE);
    // Clear some 'CR1' bits.
    i2c->CR1     &= ~( I2C_CR1_DNF    |
                       I2C_CR1_ANFOFF |
                       I2C_CR1_SMBHEN |
                       I2C_CR1_SMBDEN );
    // Clear some 'CR2' bits.
    i2c->CR2     &= ~( I2C_CR2_RD_WRN  |
                       I2C_CR2_NACK    |
                       I2C_CR2_RELOAD  |
                       I2C_CR2_AUTOEND );
    // Clear all 'ICR' flags.
    i2c->ICR     |=  ( I2C_ICR_ADDRCF   |
                       I2C_ICR_NACKCF   |
                       I2C_ICR_STOPCF   |
                       I2C_ICR_BERRCF   |
                       I2C_ICR_ARLOCF   |
                       I2C_ICR_OVRCF    |
                       I2C_ICR_PECCF    |
                       I2C_ICR_TIMOUTCF |
                       I2C_ICR_ALERTCF  );
    // Configure I2C timing.
    // Reset all but the reserved bits.
    i2c->TIMINGR &=  (0x0F000000);
    // set the given timing value.
    // TODO; default to...I think 1MHz @ 48MHz?
    i2c->TIMINGR |=  (0x50100103);
    // Enable the peripheral.
    i2c->CR1     |=  I2C_CR1_PE;
  #elif  STARm_F1
    // Perform a software reset.
    i2c->CR1     |=  (I2C_CR1_SWRST);
    i2c->CR1     &= ~(I2C_CR1_SWRST);
    // Disable the peripheral.
    i2c->CR1     &= ~(I2C_CR1_PE);
    // The F1 series uses a different I2C peripheral than
    // most other STM32s, so the timing parameter doesn't apply.
    // But these don't have to be precise to work, so
    // set the 'frequency' bits to...I dunno, 36MHz?
    i2c->CR2     &= ~(I2C_CR2_FREQ);
    i2c->CR2     |=  (36 << I2C_CR2_FREQ_Pos);
    // Set the CCR bits to...uh...TODO: calculte an actual value.
    i2c->CCR     &= ~(I2C_CCR_CCR);
    i2c->CCR     |=  (I2C_CCR_FS | 0x24);
    // Enable the peripheral.
    i2c->CR1     |=  (I2C_CR1_PE);
  #endif
  status = pSTATUS_RUN;
}

/*
 * Send a 'start' condition to the bus, asking to talk
 * with a device that has the provided 7-bit address.
 */
void pI2C::start(uint8_t address) {
#if    defined(STARm_F3)
  // Set the device address.
  i2c->CR2 &= ~(I2C_CR2_SADD);
  i2c->CR2 |=  (address << I2C_CR2_SADD_Pos);
  // Send a 'start' condition.
  i2c->CR2 |=  (I2C_CR2_START);
  while ((i2c->CR2 & I2C_CR2_START)) {};
#elif  STARm_F1
  // Generate a start condition to set the chip as a host.
  i2c->CR1 |=  (I2C_CR1_START);
  while (!(i2c->SR1 & I2C_SR1_SB)) {};
  // Wait for the peripheral to update its role.
  while (!(i2c->SR2 & I2C_SR2_MSL)) {};
  // Set the device address; 7-bits followed by an R/W bit.
  // Currently, we only ever write so just use 0 for R/W.
  i2c->DR   =  (address);
  // Wait for address to match.
  while (!(i2c->SR1 & I2C_SR1_ADDR)) {};
  // Read SR2 to clear the ADDR flag.
  (void) i2c->SR2;
#endif
}

/*
 * Send a 'stop' condition to the I2C bus.
 */
void pI2C::stop(void) {
#if    defined(STARm_F3)
  // Send 'Stop' condition, and wait for acknowledge.
  i2c->CR2 |=  (I2C_CR2_STOP);
  while ((i2c->CR2 & I2C_CR2_STOP)) {}
  // Reset the ICR ('Interrupt Clear Register') event flag.
  i2c->ICR |=  (I2C_ICR_STOPCF);
  while ((i2c->ICR & I2C_ICR_STOPCF)) {}
  // Ensure that the 'RELOAD' flag is un-set.
  set_reload_flag(0);
#elif  STARm_F1
  // Send 'Stop' condition, and wait for acknowledge.
  i2c->CR1 |=  (I2C_CR1_STOP);
  while (i2c->SR2 & I2C_SR2_MSL) {};
#endif
}

#if defined(STARm_F3)

/*
 * Set the number of bytes to send in the next transmission
 * block. The newer STM32 I2C peripheral can only send up to 255
 * bytes before you need to use the 'RELOAD' flag.
 */
void pI2C::set_num_bytes(uint8_t nbytes) {
  // Set number of bytes to process in the next transmission.
  i2c->CR2 &= ~(I2C_CR2_NBYTES);
  i2c->CR2 |=  (nbytes << I2C_CR2_NBYTES_Pos);
}

/*
 * Set the 'RELOAD' flag on or off.
 */
void pI2C::set_reload_flag(bool reload) {
  if (reload) {
    i2c->CR2 |=  I2C_CR2_RELOAD;
  }
  else {
    i2c->CR2 &= ~I2C_CR2_RELOAD;
  }
}

#endif
