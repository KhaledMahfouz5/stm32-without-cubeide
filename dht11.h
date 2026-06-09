#ifndef DHT11_H
#define DHT11_H

#include "stm32f1xx_hal.h"

#define DHT11_PIN GPIO_PIN_3
#define DHT11_PORT GPIOA

// Error codes
#define DHT11_OK 0
#define DHT11_ERROR_TIMEOUT -1
#define DHT11_ERROR_CHECKSUM -2

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t temperature;
    uint8_t humidity;
    uint8_t checksum;
} DHT11_HandleTypeDef;

void DHT11_Init(DHT11_HandleTypeDef* hdht11);
int DHT11_Read(DHT11_HandleTypeDef* hdht11);

#endif