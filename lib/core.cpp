#include "core.h"

// Define starting values for the main system clock.
// (Core clock speed when the chip first boots, in Hz.)
#if   defined(STARm_F0) || defined(STARm_F1) || defined(STARm_F3)
  volatile uint32_t sys_clock_hz = 8000000;
#elif defined(STARm_L0)
  volatile uint32_t sys_clock_hz = 2000000;
#endif

/* Common Input/Output class default constructor. */
pIO::pIO() {}

/*
 * Default virtual read/write methods for the common I/O class.
 * Most of these methods don't have a default behavior, but I
 * don't want to use a pure virtual class for several reasons.
 */
// Read the peripheral's default data length. (Up to a word)
unsigned pIO::read(void) { return 0; }

// Write the peripheral's default data width (up to a word)
void pIO::write(unsigned dat) {}

// Enable the peripheral clock.
void pIO::clock_en(void) {}

// Reset the peripheral.
void pIO::reset(void) {}

// Turn the peripheral off.
void pIO::disable(void) {}

// Return the current peripheral status,
// as far as the library knows.
int pIO::get_status(void) { return status; }
