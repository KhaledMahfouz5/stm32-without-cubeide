#include "keypad.h"
#include "main.h"

// Mapping for 4x4 matrix
const char Keypad_Map[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8',  '9', 'C'},
    {'*', '0', '#', 'D'}
};

GPIO_TypeDef* Keypad_Row_Port[] = {GPIOA, GPIOA, GPIOA, GPIOA};
uint16_t Keypad_Row_Pin[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_8, GPIO_PIN_9};

GPIO_TypeDef* Keypad_Col_Port[] = {GPIOA, GPIOA, GPIOA, GPIOA};
uint16_t Keypad_Col_Pin[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

void Keypad_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Configure rows as output
    GPIO_InitStruct.Pin = KEYPAD_ROW_PINS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Drive all rows high to prevent phantom reads during idle
    GPIOA->BSRR = KEYPAD_ROW_PINS;
    
    // Configure columns as input with pull-up
    GPIO_InitStruct.Pin = KEYPAD_COL_PINS;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

char Keypad_GetKey(void) {
    for(uint8_t row = 0; row < KEYPAD_ROWS; row++) {
        // Set current row low
        HAL_GPIO_WritePin(Keypad_Row_Port[row], Keypad_Row_Pin[row], GPIO_PIN_RESET);
        
        for(uint8_t col = 0; col < KEYPAD_COLS; col++) {
            if(HAL_GPIO_ReadPin(Keypad_Col_Port[col], Keypad_Col_Pin[col]) == GPIO_PIN_RESET) {
                // Debounce delay
                HAL_Delay(10);
                
                // Verify key press
                if(HAL_GPIO_ReadPin(Keypad_Col_Port[col], Keypad_Col_Pin[col]) == GPIO_PIN_RESET) {
                    char key = Keypad_Map[row][col];
                    
                    // Return row high after reading
                    HAL_GPIO_WritePin(Keypad_Row_Port[row], Keypad_Row_Pin[row], GPIO_PIN_SET);
                    return key;
                }
            }
        }
        
        // Return row high after scanning
        HAL_GPIO_WritePin(Keypad_Row_Port[row], Keypad_Row_Pin[row], GPIO_PIN_SET);
    }
    
    return 0; // No key pressed
}