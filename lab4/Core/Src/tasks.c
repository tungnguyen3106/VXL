#include "tasks.h"
#include "main.h"
#include "scheduler.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void print_task_info(const char* task_name) {
    char msg[60];
    sprintf(msg, "Time: %lu ms - %s\r\n", get_time(), task_name);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void task_led_05s(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    print_task_info("Task 1 (0.5s)");
}

void task_led_10s(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    print_task_info("Task 2 (1.0s)");
}

void task_led_15s(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
    print_task_info("Task 3 (1.5s)");
}

void task_led_20s(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
    print_task_info("Task 4 (2.0s)");
}

void task_led_25s(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
    print_task_info("Task 5 (2.5s)");
}

void task_oneshot_print(void) {
    print_task_info("One-shot task");
}

void task_print_10ms(void) {
    char msg[30];
    sprintf(msg, "Tick: %lu ms\r\n", get_time());
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}
