/**
  ******************************************************************************
  * @file    system_stm32f1xx.c
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  */

#include "stm32f1xx.h"

#if !defined(HSE_VALUE)
  #define HSE_VALUE (8000000U)
#endif
#if !defined(HSI_VALUE)
  #define HSI_VALUE (8000000U)
#endif

#if defined(USER_VECT_TAB_ADDRESS)
  #if defined(VECT_TAB_SRAM)
    #define VECT_TAB_BASE_ADDRESS SRAM_BASE
  #else
    #define VECT_TAB_BASE_ADDRESS FLASH_BASE
  #endif
  #if !defined(VECT_TAB_OFFSET)
    #define VECT_TAB_OFFSET 0x00000000U
  #endif
#endif

uint32_t SystemCoreClock = HSI_VALUE;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

void SystemInit(void)
{
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET;
#endif
}

void SystemCoreClockUpdate(void)
{
  uint32_t sysclk_source = RCC->CFGR & RCC_CFGR_SWS;
  uint32_t sysclk;

  if (sysclk_source == RCC_CFGR_SWS_HSI)
  {
    sysclk = HSI_VALUE;
  }
  else if (sysclk_source == RCC_CFGR_SWS_HSE)
  {
    sysclk = HSE_VALUE;
  }
  else
  {
    uint32_t pllmul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2U;
    uint32_t pllclk;

    if (RCC->CFGR & RCC_CFGR_PLLSRC)
    {
      if (RCC->CFGR & RCC_CFGR_PLLXTPRE)
      {
        pllclk = HSE_VALUE / 2U;
      }
      else
      {
        pllclk = HSE_VALUE;
      }
    }
    else
    {
      pllclk = HSI_VALUE / 2U;
    }

    sysclk = pllclk * pllmul;
  }

  uint32_t presc = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
  SystemCoreClock = sysclk >> AHBPrescTable[presc];
}
