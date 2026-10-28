#include "led_display.h"
#include "main.h"

void setLight1(int color) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, (color == RED) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (color == YELLOW) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (color == GREEN) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void setLight2(int color) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (color == RED) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, (color == YELLOW) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (color == GREEN) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
