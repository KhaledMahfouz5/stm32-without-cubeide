#include "dht11.h"
#include "main.h"

// DHT11 timing constants (in microseconds)
#define DHT11_START_DELAY_US 18000
#define DHT11_RESPONSE_TIMEOUT_US 100
#define DHT11_BIT_THRESHOLD_US 40

// Helper function for microsecond delay
static void DHT11_DelayUs(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < cycles);
}

// Configure pin as output
static void DHT11_SetPinOutput(DHT11_HandleTypeDef* hdht11) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hdht11->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(hdht11->port, &GPIO_InitStruct);
}

// Configure pin as input
static void DHT11_SetPinInput(DHT11_HandleTypeDef* hdht11) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hdht11->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(hdht11->port, &GPIO_InitStruct);
}

// Wait for pin state with timeout (microsecond resolution via DWT)
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

    // Initialize pin as output high
    DHT11_SetPinOutput(hdht11);
    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_SET);

    // Enable DWT cycle counter for microsecond timing
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

int DHT11_Read(DHT11_HandleTypeDef* hdht11) {
    uint8_t data[5] = {0};

    // Start signal: pull low for 18ms
    DHT11_SetPinOutput(hdht11);
    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_RESET);
    HAL_Delay(18);

    // Pull high for 20-40us
    HAL_GPIO_WritePin(hdht11->port, hdht11->pin, GPIO_PIN_SET);
    DHT11_DelayUs(30);

    // Switch to input and wait for response
    DHT11_SetPinInput(hdht11);

    // Wait for DHT11 to pull low (response)
    if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    // Wait for DHT11 to pull high
    if (DHT11_WaitForPin(hdht11, GPIO_PIN_SET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    // Wait for start of data transmission
    if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
        return DHT11_ERROR_TIMEOUT;
    }

    // Read 40 bits (5 bytes)
    for (int byte = 0; byte < 5; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            // Wait for data pin to go high
            if (DHT11_WaitForPin(hdht11, GPIO_PIN_SET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
                return DHT11_ERROR_TIMEOUT;
            }

            // Measure duration of high signal
            uint32_t high_start = DWT->CYCCNT;
            if (DHT11_WaitForPin(hdht11, GPIO_PIN_RESET, DHT11_RESPONSE_TIMEOUT_US) != DHT11_OK) {
                return DHT11_ERROR_TIMEOUT;
            }
            uint32_t high_duration = (DWT->CYCCNT - high_start) / (SystemCoreClock / 1000000);

            // Shift bit into data
            data[byte] <<= 1;
            if (high_duration > DHT11_BIT_THRESHOLD_US) {
                data[byte] |= 1;
            }
        }
    }

    // Verify checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        return DHT11_ERROR_CHECKSUM;
    }

    // Store results
    hdht11->humidity = data[0];
    hdht11->temperature = data[2];
    hdht11->checksum = data[4];

    return DHT11_OK;
}
