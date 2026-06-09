#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "keypad.h"
#include "servo.h"
#include "lcd.h"
#include "dht11.h"

UART_HandleTypeDef uart2;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim3;

void clock_init(void);

static void MX_I2C1_Init(void) {
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio.Mode = GPIO_MODE_AF_OD;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
        while(1);
}

static void MX_TIM3_Init(void) {
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 20000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
        while(1);

    Servo_Init(&htim3);
    HAL_TIM_Base_Start(&htim3);
}

void main(void)
{
    HAL_Init();
    clock_init();
    SystemCoreClockUpdate();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = LED_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &gpio_init);

    gpio_init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    uart2.Instance = USART2;
    uart2.Init.BaudRate = 115200;
    uart2.Init.Mode = UART_MODE_TX;
    uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart2.Init.WordLength = UART_WORDLENGTH_8B;
    uart2.Init.StopBits = UART_STOPBITS_1;
    uart2.Init.Parity = UART_PARITY_NONE;
    if (HAL_UART_Init(&uart2) != HAL_OK)
        while(1);

    USART2->DR = 0;
    while (!(USART2->SR & USART_SR_TC));

    MX_I2C1_Init();
    MX_TIM3_Init();
    Keypad_Init();
    LCD_Init(&hi2c1);
    state_machine_init();

    while(1)
    {
        state_machine_run();
    }
}

void clock_init(void)
{
    RCC_OscInitTypeDef osc_init = {0};
    osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_init.HSEState = RCC_HSE_ON;
    osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    osc_init.PLL.PLLState = RCC_PLL_ON;
    osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_init.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
        while(1);

    RCC_ClkInitTypeDef clock_init = {0};
    clock_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_HCLK;
    clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clock_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clock_init.APB1CLKDivider = RCC_HCLK_DIV2;
    clock_init.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clock_init, FLASH_LATENCY_2) != HAL_OK)
        while(1);
}
