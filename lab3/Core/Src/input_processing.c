#include "input_processing.h"
#include "led_display.h"
#include "timer.h"
#include "main.h"
#include "input_reading.h"

extern int led_buffer[4];

int red_value = 5;
int green_value = 3;
int yellow_value = 2;

int temp_value = 0;

int counter_tl1 = 0;
int counter_tl2 = 0;

enum TrafficState {INIT, RED_GREEN, RED_YELLOW, GREEN_RED, YELLOW_RED};
enum TrafficState traffic_state = INIT;

int system_mode = 1;

void fsm_for_input_processing(void) {
    if (is_button_pressed(0)) {
        system_mode++;
        if (system_mode > 4) {
            system_mode = 1;
        }

        if (system_mode == 1) {
            traffic_state = INIT;
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
            if (system_mode == 2) temp_value = red_value;
            if (system_mode == 3) temp_value = yellow_value;
            if (system_mode == 4) temp_value = green_value;
            setTimer3(250);
        }
    }
}

void fsm_mode_1() {
    switch (traffic_state) {
        case INIT:
            counter_tl1 = red_value;
            counter_tl2 = green_value;
            traffic_state = RED_GREEN;
            setTimer1(1000);
            break;

        case RED_GREEN:
            setLight1(RED);
            setLight2(GREEN);

            if (timer1_flag == 1) {
                setTimer1(1000);
                counter_tl1--;
                counter_tl2--;

                if (counter_tl2 <= 0) {
                    traffic_state = RED_YELLOW;
                    counter_tl2 = yellow_value;
                }
            }
            break;

        case RED_YELLOW:
            setLight1(RED);
            setLight2(YELLOW);

            if (timer1_flag == 1) {
                setTimer1(1000);
                counter_tl1--;
                counter_tl2--;

                if (counter_tl2 <= 0) {
                    traffic_state = GREEN_RED;
                    counter_tl1 = green_value;
                    counter_tl2 = red_value;
                }
            }
            break;

        case GREEN_RED:
            setLight1(GREEN);
            setLight2(RED);

            if (timer1_flag == 1) {
                setTimer1(1000);
                counter_tl1--;
                counter_tl2--;

                if (counter_tl1 <= 0) {
                    traffic_state = YELLOW_RED;
                    counter_tl1 = yellow_value;
                }
            }
            break;

        case YELLOW_RED:
            setLight1(YELLOW);
            setLight2(RED);

            if (timer1_flag == 1) {
                setTimer1(1000);
                counter_tl1--;
                counter_tl2--;

                if (counter_tl1 <= 0) {
                    traffic_state = RED_GREEN;
                    counter_tl1 = red_value;
                    counter_tl2 = green_value;
                }
            }
            break;
    }
    if(counter_tl1 > 0 && counter_tl2 > 0) {
        led_buffer[0] = counter_tl1 / 10;
        led_buffer[1] = counter_tl1 % 10;
        led_buffer[2] = counter_tl2 / 10;
        led_buffer[3] = counter_tl2 % 10;
    }
}

void fsm_mode_2() {
    led_buffer[0] = 0;
    led_buffer[1] = 2;
    led_buffer[2] = temp_value / 10;
    led_buffer[3] = temp_value % 10;
    if (timer3_flag == 1) {
        setTimer3(250);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2); // RED 1
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // RED 2
    }

    if (is_button_pressed(1)) {
        temp_value++;
        if (temp_value > 99) {
            temp_value = yellow_value + 1;
        }
    }

    if (is_button_pressed(2)) {
        red_value = temp_value;
        green_value = red_value - yellow_value;
    }
}

void fsm_mode_3() {
    led_buffer[0] = 0;
    led_buffer[1] = 3;
    led_buffer[2] = temp_value / 10;
    led_buffer[3] = temp_value % 10;

    if (timer3_flag == 1) {
        setTimer3(250);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    }

    if (is_button_pressed(1)) {
        temp_value++;
        if (temp_value > (99 - green_value)) {
            temp_value = 1;
        }
    }

    if (is_button_pressed(2)) {
        yellow_value = temp_value;
        red_value = green_value + yellow_value;
    }
}

void fsm_mode_4() {
    led_buffer[0] = 0;
    led_buffer[1] = 4;
    led_buffer[2] = temp_value / 10;
    led_buffer[3] = temp_value % 10;

    if (timer3_flag == 1) {
        setTimer3(250);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
    }
    if (is_button_pressed(1)) {
        temp_value++;
        if (temp_value > (99 - yellow_value)) {
            temp_value = 1;
        }
    }
    if (is_button_pressed(2)) {
        green_value = temp_value;
        red_value = green_value + yellow_value;
    }
}

void fsm_system(void) {
    switch(system_mode) {
        case 1:
            fsm_mode_1();
            break;
        case 2:
            fsm_mode_2();
            break;
        case 3:
            fsm_mode_3();
            break;
        case 4:
            fsm_mode_4();
            break;
    }
}
