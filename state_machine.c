#include <stdio.h>
#include "main.h"
#include "keypad.h"
#include "servo.h"
#include "lcd.h"
#include "dht11.h"
#include "actuator.h"

typedef enum {
    STATE_WAIT_PASSWORD,
    STATE_SET_TIMER_1,
    STATE_SET_TIMER_2,
    STATE_MONITOR,
    STATE_ACTIVE_BOX_1,
    STATE_ACTIVE_BOX_2
} SystemState;

static SystemState current_state = STATE_WAIT_PASSWORD;
static uint32_t last_box1_activation = 0;
static uint32_t last_box2_activation = 0;
static char password[4];
static uint32_t timer_box1_ms = 0;
static uint32_t timer_box2_ms = 0;
static DHT11_HandleTypeDef dht11;

static uint32_t parse_timer_input(void) {
    char key;
    uint32_t value = 0;
    int digit_count = 0;

    while (digit_count < 3) {
        key = Keypad_GetKey();
        if (key >= '0' && key <= '9') {
            value = value * 10 + (key - '0');
            LCD_WriteChar(key);
            digit_count++;
            while (Keypad_GetKey() != 0) {
                HAL_Delay(10);
            }
        }
    }
    return value * 1000;
}

static void wait_for_keypad_release(void) {
    while (Keypad_GetKey() != 0) {
        HAL_Delay(10);
    }
}

static void handle_password_entry(void) {
    LCD_Clear();
    LCD_Print("Enter password:");
    LCD_SetCursor(0, 1);

    for (int i = 0; i < 4; i++) {
        char key = 0;
        while (key == 0) {
            key = Keypad_GetKey();
            HAL_Delay(10);
        }
        password[i] = key;
        LCD_WriteChar(key);
        HAL_Delay(200);
    }

    HAL_Delay(500);
    current_state = STATE_SET_TIMER_1;
}

static void handle_timer_1_setup(void) {
    LCD_Clear();
    LCD_Print("Box A (seconds):");
    LCD_SetCursor(0, 1);

    timer_box1_ms = parse_timer_input();
    wait_for_keypad_release();

    HAL_Delay(250);
    current_state = STATE_SET_TIMER_2;
}

static void handle_timer_2_setup(void) {
    LCD_Clear();
    LCD_Print("Box B (seconds):");
    LCD_SetCursor(0, 1);

    timer_box2_ms = parse_timer_input();
    wait_for_keypad_release();

    HAL_Delay(250);
    LCD_Clear();

    last_box1_activation = HAL_GetTick();
    last_box2_activation = HAL_GetTick();
    current_state = STATE_MONITOR;
}

static void display_remaining_time(uint32_t remaining_ms, int row) {
    uint32_t minutes = remaining_ms / 60000;
    uint32_t seconds = (remaining_ms % 60000) / 1000;

    LCD_SetCursor(5, row);
    if (minutes < 100) LCD_Print(" ");
    if (minutes < 10) LCD_Print(" ");

    char buf[16];
    snprintf(buf, sizeof(buf), "%lu:%02lu", minutes, seconds);
    LCD_Print(buf);
}

static void handle_monitor(void) {
    uint32_t current = HAL_GetTick();

    if (current - last_box1_activation >= timer_box1_ms) {
        current_state = STATE_ACTIVE_BOX_1;
        return;
    }
    if (current - last_box2_activation >= timer_box2_ms) {
        current_state = STATE_ACTIVE_BOX_2;
        return;
    }

    static uint32_t last_display_update = 0;
    if (current - last_display_update >= 500) {
        last_display_update = current;

        int dht_status = DHT11_Read(&dht11);

        LCD_Clear();

        LCD_SetCursor(0, 0);
        LCD_Print("A:");
        uint32_t rem1 = timer_box1_ms - (current - last_box1_activation);
        display_remaining_time(rem1, 0);

        LCD_SetCursor(0, 1);
        LCD_Print("B:");
        uint32_t rem2 = timer_box2_ms - (current - last_box2_activation);
        display_remaining_time(rem2, 1);

        if (dht_status == DHT11_OK) {
            LCD_SetCursor(11, 0);
            char buf[8];
            snprintf(buf, sizeof(buf), "T=%d", dht11.temperature);
            LCD_Print(buf);

            LCD_SetCursor(11, 1);
            snprintf(buf, sizeof(buf), "H=%d", dht11.humidity);
            LCD_Print(buf);
        }
    }

    HAL_Delay(100);
}

static int verify_password(void) {
    char input[4];
    LCD_Clear();
    LCD_Print("Enter PIN:");
    LCD_SetCursor(0, 1);

    for (int i = 0; i < 4; i++) {
        char key = 0;
        while (key == 0) {
            key = Keypad_GetKey();
            HAL_Delay(10);
        }
        input[i] = key;
        LCD_WriteChar('*');
        HAL_Delay(200);
    }

    for (int i = 0; i < 4; i++) {
        if (input[i] != password[i]) {
            return 0;
        }
    }
    return 1;
}

static void handle_active_box(uint8_t box_id) {
    uint8_t servo_ch = (box_id == BOX_1) ? SERVO_CHANNEL_1 : SERVO_CHANNEL_2;

    Servo_Write(servo_ch, 60);
    Box_LED_On(box_id);

    uint32_t start = HAL_GetTick();
    int authenticated = 0;

    while ((HAL_GetTick() - start < 30000) && !authenticated) {
        if (verify_password()) {
            authenticated = 1;
        }
        HAL_Delay(100);
    }

    LCD_Clear();
    LCD_Print("Press * to close");
    char key = 0;
    while (key != '*') {
        key = Keypad_GetKey();
        HAL_Delay(10);
    }

    Servo_Write(servo_ch, 150);
    Box_LED_Off(box_id);

    if (box_id == BOX_1) {
        last_box1_activation = HAL_GetTick();
    } else {
        last_box2_activation = HAL_GetTick();
    }
    current_state = STATE_MONITOR;
}

void state_machine_init(void) {
    DHT11_Init(&dht11);
}

void state_machine_run(void) {
    switch (current_state) {
        case STATE_WAIT_PASSWORD:
            handle_password_entry();
            break;
        case STATE_SET_TIMER_1:
            handle_timer_1_setup();
            break;
        case STATE_SET_TIMER_2:
            handle_timer_2_setup();
            break;
        case STATE_MONITOR:
            handle_monitor();
            break;
        case STATE_ACTIVE_BOX_1:
            handle_active_box(BOX_1);
            break;
        case STATE_ACTIVE_BOX_2:
            handle_active_box(BOX_2);
            break;
    }
}