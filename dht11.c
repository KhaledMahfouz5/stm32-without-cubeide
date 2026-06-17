#include "dht11.h"
#include "main.h"
#include "delay.h"

#define DHT11_RESPONSE_TIMEOUT_US 100
#define DHT11_BIT_THRESHOLD_US 40

static void DHT11_SetPinOutput(DHT11_HandleTypeDef* hdht11) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hdht11->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(hdht11->port, &GPIO_InitStruct);
}

static void DHT11_SetPinInput(DHT11_HandleTypeDef* hdht11) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hdht11->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(hdht11->port, &GPIO_InitStruct);
}

static int DHT11_WaitForPin(DHT11_HandleTypeDef* hdht11, GPIO_PinState state, uint32_t timeout_us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = timeout_us * (SystemCoreClock / 1000000);
    while (HAL_GPIO_ReadPin(hdht11->port, hdht11->pin) != state) {
        if ((DWT->CYCCNT - start) >= cycles) {
            return DHT11_ERROR_TIMEOUT;
        }
    }
    return DHT11_OK;
}

void DHT11_Init(DHT11_HandleTypeDef* hdht11) {
    hdht11->port = DHT11_PORT;
    hdht11->pin = DHT11_PIN;

    DHT11_SetPinOutput(hdht11);
    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_SET);
}

int DHT11_Read(DHT11_HandleTypeDef* hdht11) {
    uint8_t data[5] = {0};

    DHT11_SetPinOutput(hdht11);
    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_RESET);
    HAL_Delay(18);

    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_SET);
    delay_us(30);

    DHT11_SetPinInput(hdht11);

    if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    if (DHT11_WaitForPin(hdht11, GPIO_PIN_SET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    for (int byte = 0; byte < 5; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (DHT11_WaitForPin(hdht11, GPIO_PIN_SET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
                return DHT11_ERROR_TIMEOUT;
            }

            uint32_t high_start = DWT->CYCCNT;
            if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
                return DHT11_ERROR_TIMEOUT;
            }
            uint32_t high_duration = (DWT->CYCCNT - high_start) / (SystemCoreClock / 1000000);

            data[byte] <<= 1;
            if (high_duration > DHT11_BIT_THRESHOLD_US) {
                data[byte] |= 1;
            }
        }
    }

    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        return DHT11_ERROR_CHECKSUM;
    }

    hdht11->humidity = data[0];
    hdht11->temperature = data[2];
    hdht11->checksum = data[4];

    return DHT11_OK;
}
