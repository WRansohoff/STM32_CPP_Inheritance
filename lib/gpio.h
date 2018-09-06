#ifndef __STARm_GPIO_H
#define __STARm_GPIO_H

// Project includes.
#include "core.h"

// Project macro definitions.
// GPIO pin state macros.
#if   defined(STARm_F0) || defined(STARm_F3) || defined(STARm_L0)
  #define pGPIO_MODE_IN        (0x00)
  #define pGPIO_MODE_OUT       (0x01)
  #define pGPIO_MODE_ALT       (0x02)
  #define pGPIO_MODE_AIN       (0x03)
  #define pGPIO_OTYPE_PP       (0x00)
  #define pGPIO_OTYPE_OD       (0x01)
  #define pGPIO_SPEED_LOW      (0x00)
  #define pGPIO_SPEED_MED      (0x01)
  #define pGPIO_SPEED_HI       (0x03)
  #define pGPIO_PUPD_NONE      (0x00)
  #define pGPIO_PUPD_UP        (0x01)
  #define pGPIO_PUPD_DOWN      (0x02)
#elif STARm_F1
  #define pGPIO_CFG_IN_FLT     (0x04)
  #define pGPIO_CFG_IN_PUPD    (0x08)
  #define pGPIO_CFG_IN_ANALOG  (0x00)
  #define pGPIO_CFG_OUT_PP     (0x02)
  #define pGPIO_CFG_OUT_OD     (0x06)
  #define pGPIO_CFG_AF_PP      (0x0A)
  #define pGPIO_CFG_AF_OD      (0x0E)
#endif
// GPIO enum for convenience instantiation.
// Output speed is not currently specified; it can
// be set after the initialization, but 2MHz /
// 'low speed' is fine for most purposes.
enum pGPIO_pin_qinit {
  pGPIO_IN_FLOATING,
  pGPIO_IN_PULLUP,
  pGPIO_IN_PULLDOWN,
  pGPIO_IN_ANALOG,
  pGPIO_OUT_PP,
  pGPIO_OUT_OD,
  pGPIO_AF_PP,
  pGPIO_AF_OD
  #if defined(STARm_F0) || defined(STARm_F3) || defined(STARm_L0)
  ,pGPIO_OUT_PP_PULLUP,
  pGPIO_OUT_PP_PULLDOWN,
  pGPIO_OUT_OD_PULLUP,
  pGPIO_OUT_OD_PULLDOWN,
  pGPIO_AF_PP_PULLUP,
  pGPIO_AF_PP_PULLDOWN,
  pGPIO_AF_OD_PULLUP,
  pGPIO_AF_OD_PULLDOWN,
  #endif
};

/*
 * GPIO peripheral class.
 */
class pGPIO : public pIO {
public:
  // Constructors.
  pGPIO();
  pGPIO(GPIO_TypeDef* bank);
  // Common r/w methods from the core I/O class.
  unsigned read(void);
  void     write(unsigned dat);
  void     stream(volatile void* buf, int len);
  // GPIO-specific methods.
  // Generic pin manipulation methods.
  bool     read_pin(unsigned pin_num);
  void     pin_on(unsigned pin_num);
  void     pins_on(uint16_t pin_mask);
  void     pin_off(unsigned pin_num);
  void     pins_off(uint16_t pin_mask);
  void     pin_toggle(unsigned pin_num);
  void     pins_toggle(uint16_t pin_mask);
  // Register modification methods; platform-specific.
  #if   defined(STARm_F0) || defined(STARm_F3) || defined(STARm_L0)
    void   set_pin_mode(unsigned pin_num, unsigned mode);
    void   set_pin_type(unsigned pin_num, unsigned otype);
    void   set_pin_speed(unsigned pin_num, unsigned ospeed);
    void   set_pin_pupd(unsigned pin_num, unsigned pupd);
    void   set_pin_af(unsigned pin_num, unsigned af);
  #elif STARm_F1
    void   set_pin_cfg(unsigned pin_num, unsigned cfg);
  #endif
protected:
  // Reference GPIO register struct.
  GPIO_TypeDef* gpio        = NULL;
private:
};

/*
 * Class for an individual GPIO pin.
 */
class pGPIO_pin {
public:
  pGPIO_pin();
  // Convenience constructor.
  // Pass in an enum value from the pin modes defined above.
  pGPIO_pin(pGPIO* pin_bank, uint8_t pin_num, pGPIO_pin_qinit q);
  // GPIO pin methods.
  // TODO: add a flag for reversing 'on/off' for e.g. a pin
  // wired to an LED's cathode.
  void on(void);
  void off(void);
  void toggle(void);
  bool read(void);
  // Getters/setters.
  int  get_status(void);
  // Platform-specific pin configuration methods.
  #if   defined(STARm_F0) || defined(STARm_F3) || defined(STARm_L0)
    void set_mode(unsigned mode);
    void set_type(unsigned type);
    void set_speed(unsigned speed);
    void set_pupd(unsigned pupd);
    void set_alt_func(unsigned af);
  #elif STARm_F1
    void set_cfg(unsigned config);
  #endif
protected:
  pGPIO*  bank   = NULL;
  uint8_t pin    = 0;
  // Expected pin status.
  int status     = pSTATUS_ERR;
private:
};

#endif
