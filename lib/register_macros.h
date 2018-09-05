#ifndef __STARm_REGMACROS_H
#define __STARm_REGMACROS_H

/*
 * These addresses are provided by ST's device headers already,
 * but their 'typedef' syntax can be tough to use with class vars.
 * So while these macros are a bit ungainly, we can hide them
 * away in a header file and pretend they don't exist as usual.
 *
 * This is very incomplete; I only add values as I use them.
 */
#define __REG(x) (__IO uint32_t*)((x))

#if   defined(STARm_F1)
  /* Peripheral Domain Bases */
  #define STARm_PERIPHS      (0x40000000U)
  #define STARm_APB1         (STARm_PERIPHS)
  #define STARm_APB2         (STARm_PERIPHS + 0x00010000U)
  #define STARm_AHB          (STARm_PERIPHS + 0x00020000U)
  /* RCC */
  #define STARm_RCC          (STARm_AHB + 0x00001000U)
  #define STARm_RCC_APB2RSTR __REG(STARm_RCC + 0x0000000CU)
  #define STARm_RCC_APB1RSTR __REG(STARm_RCC + 0x00000010U)
  #define STARm_RCC_AHBENR   __REG(STARm_RCC + 0x00000014U)
  #define STARm_RCC_APB2ENR  __REG(STARm_RCC + 0x00000018U)
  #define STARm_RCC_APB1ENR  __REG(STARm_RCC + 0x0000001CU)
  #define STARm_RCC_AHBRSTR  __REG(STARm_RCC + 0x00000028U)
#elif defined(STARm_F3)
  /* Peripheral Domain Bases */
  #define STARm_PERIPHS      (0x40000000U)
  #define STARm_APB1         (STARm_PERIPHS)
  #define STARm_APB2         (STARm_PERIPHS + 0x00010000U)
  #define STARm_AHB          (STARm_PERIPHS + 0x00020000U)
  /* RCC */
  #define STARm_RCC          (STARm_AHB + 0x00001000U)
  #define STARm_RCC_APB2RSTR __REG(STARm_RCC + 0x0000000CU)
  #define STARm_RCC_APB1RSTR __REG(STARm_RCC + 0x00000010U)
  #define STARm_RCC_AHBENR   __REG(STARm_RCC + 0x00000014U)
  #define STARm_RCC_APB2ENR  __REG(STARm_RCC + 0x00000018U)
  #define STARm_RCC_APB1ENR  __REG(STARm_RCC + 0x0000001CU)
  #define STARm_RCC_AHBRSTR  __REG(STARm_RCC + 0x00000028U)
#endif

#endif
