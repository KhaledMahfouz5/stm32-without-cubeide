#include <stdint.h>
#include <stdio.h>
#include "main.h"

UART_HandleTypeDef uart2;

void clock_init();

void main(void)
{
  HAL_Init();
  clock_init();
  SystemCoreClockUpdate(); // Update the internal clock frequency variable

  // Initialize LED GPIO
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};
  gpio_init.Pin = LED_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(LED_PORT, &gpio_init);

  // Initialize UART
  uart2.Instance = USART2;
  uart2.Init.BaudRate = 115200;
  uart2.Init.Mode = UART_MODE_TX;
  uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart2.Init.WordLength = UART_WORDLENGTH_8B;
  uart2.Init.StopBits = UART_STOPBITS_1;
  uart2.Init.Parity = UART_PARITY_NONE;
  if (HAL_UART_Init(&uart2) != HAL_OK)
  {
    while(1);
  }

  // Dummy write, because the first byte seems to always be dropped
  USART2->DR = 0;
  while (!(USART2->SR & USART_SR_TC));
  
  while(1)
  {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    printf("[%.3f] Hello, World!\r\n", HAL_GetTick()/1000.0f);
    HAL_Delay(500);
  }
}

void clock_init()
{
  /* By default HSI (8 MHz RC oscillator) is selected as system clock.
   * We want to use the HSE (8 MHz crystal) through the PLL to get 72 MHz.
   */

  RCC_OscInitTypeDef osc_init = {0};
  osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  osc_init.HSEState = RCC_HSE_ON;
  osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  osc_init.PLL.PLLState = RCC_PLL_ON;
  osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  osc_init.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
  {
    while(1);
  }

  /* Set PLL output as the source for the system clock.
   * Since APB1 clock must not be more than 36 MHz, set the PCLK1 divider to 2.
   */
  RCC_ClkInitTypeDef clock_init = {0};
  clock_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_HCLK;
  clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clock_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clock_init.APB1CLKDivider = RCC_HCLK_DIV2;
  clock_init.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&clock_init, FLASH_LATENCY_2) != HAL_OK) // 72 MHz -> 2 wait states
  {
    while(1);
  }
}
