#include "command_parser.h"
#include <string.h>

enum ParserState {
    WAIT_START,
    READ_CMD
};

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint8_t command_flag = 0;
uint8_t command_data = CMD_NONE;

static enum ParserState parser_state = WAIT_START;
static uint8_t read_index = 0;
static char cmd_str[10];
static uint8_t cmd_idx = 0;

void command_parser_fsm(void) {
    while (read_index != index_buffer) {
        uint8_t ch = buffer[read_index++];
        if (read_index >= MAX_BUFFER_SIZE) {
            read_index = 0;
        }

        switch (parser_state) {
            case WAIT_START:
                if (ch == '!') {
                    cmd_idx = 0;
                    parser_state = READ_CMD;
                }
                break;

            case READ_CMD:
                if (ch == '!') {
                    cmd_idx = 0;
                } else if (ch == '#') {
                    cmd_str[cmd_idx] = '\0';
                    if (strcmp(cmd_str, "RST") == 0) {
                        command_data = CMD_RST;
                        command_flag = 1;
                    } else if (strcmp(cmd_str, "OK") == 0) {
                        command_data = CMD_OK;
                        command_flag = 1;
                    }
                    parser_state = WAIT_START;
                } else {
                    if (cmd_idx < sizeof(cmd_str) - 1) {
                        cmd_str[cmd_idx++] = (char)ch;
                    } else {
                        parser_state = WAIT_START;
                    }
                }
                break;

            default:
                parser_state = WAIT_START;
                break;
        }
    }
}
