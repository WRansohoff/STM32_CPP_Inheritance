/*
 * Minimal linker script for an STM32F030K6.
 */

/* Label for the program's entry point */
ENTRY(reset_handler)

/* End of RAM/Start of stack */
/* (12KB SRAM, 4KB CCMRAM not included) */
_estack = 0x20003000;

/* Set minimum size for stack and dynamic memory. */
/* (The linker will generate an error if there is
 * less than this much RAM leftover.) */
/* (1KB) */
_Min_Leftover_RAM = 0x400;

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 64K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 12K
    CCMRAM ( rw )     : ORIGIN = 0x10000000, LENGTH = 4K /* (TODO) */
}

SECTIONS
{
  /* The vector table goes to the start of flash. */
  .vector_table :
  {
    . = ALIGN(4);
    KEEP (*(.vector_table))
    . = ALIGN(4);
  } >FLASH

  /* The 'text' section contains the main program code. */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    . = ALIGN(4);
  } >FLASH

  /* The 'rodata' section contains read-only data,
   * constants, strings, information that won't change. */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* The 'data' section is space set aside in RAM for
   * things like variables, which can change. */
  _sidata = .;
  .data :
  {
    . = ALIGN(4);
    /* Mark start/end locations for the 'data' section. */
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
    . = ALIGN(4);
  } >RAM AT >FLASH
  _data_addr = LOADADDR(.data);

  /* These '...array' sections hold arrays of function pointers
   * which are used by C++ inheritance. The 'init_array' section
   * has pointers to the constructors of statically-allocated
   * while the 'fini_array' section has the same for destructors.
   * I think 'preinit_array' holds methods that should be called
   * before the 'init_array' ones.
   * These are probably required if you plan to use derived
   * classes with virtual methods. */
  .preinit_array :
  {
    . = ALIGN(4);
    _spreinit_array = .;
    KEEP (*(.preinit_array))
    KEEP (*(.preinit_array*))
    _epreinit_array = .;
    . = ALIGN(4);
  } >FLASH
  .init_array :
  {
    . = ALIGN(4);
    _sinit_array = .;
    KEEP (*(.init_array))
    KEEP (*(.init_array*))
    _einit_array = .;
    . = ALIGN(4);
  } >FLASH
  .fini_array :
  {
    . = ALIGN(4);
    _sfini_array = .;
    KEEP (*(.fini_array))
    KEEP (*(.fini_array*))
    _efini_array = .;
    . = ALIGN(4);
  } >FLASH

  /* The 'bss' section is similar to the 'data' section,
   * but its space is initialized to all 0s at the
   * start of the program. */
  .bss :
  {
    . = ALIGN(4);
    /* Also mark the start/end of the BSS section. */
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >RAM

  /* Space set aside for the application's heap/stack. */
  .dynamic_allocations :
  {
    . = ALIGN(4);
    _ssystem_ram = .;
    . = . + _Min_Leftover_RAM;
    . = ALIGN(4);
    _esystem_ram = .;
  } >RAM
}
