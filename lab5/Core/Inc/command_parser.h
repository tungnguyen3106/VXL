#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include "main.h"

#define MAX_BUFFER_SIZE 30

#define CMD_NONE 0
#define CMD_RST  1
#define CMD_OK   2

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

extern uint8_t command_flag;
extern uint8_t command_data;

void command_parser_fsm(void);

#endif /* INC_COMMAND_PARSER_H_ */
