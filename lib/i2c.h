#ifndef __STARm_I2C_H
#define __STARm_I2C_H

// Project includes.
#include "core.h"
#include "gpio.h"

// Project macro definitions.
// I2C peripheral status.
#define pSTATUS_I2C_SET (1)
#define pSTATUS_I2C_ON  (2)
// (TODO: Support standard speeds (100KHz, 400KHz, 1MHz))

/*
 * Class representing an I2C interface.
 * Currently, not much functionality is supported; it's
 * more or less host-only, no SMBus, no 10-bit addressing.
 * TODO: Figure out timing calculations and accept an
 * interface speed in Hz or KHz.
 */
class pI2C : pIO {
public:
  // Constructors.
  pI2C();
  pI2C(I2C_TypeDef* i2c_regs);
  // Common r/w methods from the core I/O class.
  unsigned read(void);
  void     write(unsigned dat);
  // Common peripheral methods from core I/O class.
  void     clock_en(void);
  void     reset(void);
  void     disable(void);
  // I2C-specific methods.
  void     i2c_init(void); /* TODO: Timing */
  void     start(uint8_t address);
  void     stop(void);
  void     write_bytes(volatile uint8_t* buf, int len);
  #if   defined(STARm_F3)
    void   set_num_bytes(uint8_t nbytes);
  #endif

  // I2C struct from the device header files.
  I2C_TypeDef* i2c = NULL;
protected:
  // Enable/disable/reset register definitions.
  __IO uint32_t *enable_reg = 0;
  __IO uint32_t *reset_reg  = 0;
  uint32_t       enable_bit = 0;
  uint32_t       reset_bit  = 0;
private:
};

#endif
