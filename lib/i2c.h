#ifndef __STARm_I2C_H
#define __STARm_I2C_H

// Project includes.
#include "core.h"
#include "gpio.h"

/*
 * Class representing an I2C interface.
 * Currently, not much functionality is supported; it's
 * more or less host-only, no SMBus, no 10-bit addressing.
 * TODO: Figure out timing calculations and accept an
 * interface speed in Hz or KHz.
 */
class pI2C : public pIO {
public:
  // Constructors.
  pI2C();
  pI2C(I2C_TypeDef* i2c_regs);
  // Common r/w methods from the core I/O class.
  unsigned read(void);
  void     write(unsigned dat);
  void     stream(volatile void* buf, int len);
  // I2C-specific methods.
  void     i2c_init(void); /* TODO: Timing */
  void     start(uint8_t address);
  void     stop(void);
  #if   defined(STARm_F3)
    void   set_num_bytes(uint8_t nbytes);
    void   set_reload_flag(bool reload);
  #endif
protected:
  // I2C struct from the device header files.
  I2C_TypeDef* i2c = NULL;
private:
};

#endif
