#include "main.h"
#include "input_reading.h"

#define NO_OF_BUTTONS 3
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

#define HOLD_TIME_1S 100
#define AUTO_REPEAT_TIME_0_5S 50

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];

static uint8_t button_flag[NO_OF_BUTTONS] = {0, 0, 0};

static uint16_t counterForButtonHold[NO_OF_BUTTONS] = {0, 0, 0};

uint16_t button_pins[NO_OF_BUTTONS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2};

unsigned char is_button_pressed(uint8_t index) {
    if(index >= NO_OF_BUTTONS) return 0;
    if (button_flag[index] == 1) {
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, button_pins[i]);
        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            if (buttonBuffer[i] != debounceButtonBuffer1[i]) {
                buttonBuffer[i] = debounceButtonBuffer1[i];
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    button_flag[i] = 1;
                    counterForButtonHold[i] = HOLD_TIME_1S;
                }
            } else {
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    counterForButtonHold[i]--;
                    if (counterForButtonHold[i] == 0) {
                        button_flag[i] = 1;
                        counterForButtonHold[i] = AUTO_REPEAT_TIME_0_5S;
                    }
                }
            }
        }
    }
}
