#include "uart_communication.h"
#include "command_parser.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

enum CommState {
    WAIT_RST,
    WAIT_OK
};

static enum CommState comm_state = WAIT_RST;
static uint32_t ADC_value = 0;
static char tx_packet[30];

void uart_communication_fsm(void) {
    switch (comm_state) {
        case WAIT_RST:
            if (command_flag == 1) {
                command_flag = 0;
                if (command_data == CMD_RST) {
                    ADC_value = HAL_ADC_GetValue(&hadc1);
                    sprintf(tx_packet, "\r\n!ADC=%lu#", ADC_value);
                    HAL_UART_Transmit(&huart2, (uint8_t*)tx_packet, strlen(tx_packet), 100);
                    setTimer1(3000);
                    comm_state = WAIT_OK;
                }
            }
            break;

        case WAIT_OK:
            if (command_flag == 1) {
                command_flag = 0;
                if (command_data == CMD_OK) {
                    comm_state = WAIT_RST;
                }
            } else if (timer1_flag == 1) {
                HAL_UART_Transmit(&huart2, (uint8_t*)tx_packet, strlen(tx_packet), 100);
                setTimer1(3000);
            }
            break;

        default:
            comm_state = WAIT_RST;
            break;
    }
}
