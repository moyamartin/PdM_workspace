#include <stdio.h>
#include <string.h>

#include "main.h"

#include "API_cmdparser.h"

static cmd_fsm_state_t cmdParserFSM;			//< implementation of a simple command parser that receives commands from UART and executes them
static char rx_buffer[CMD_MAX_LINE]; 			//< buffer to store received characters until we receive EOL char
static char curr_cmd[CMD_MAX_LINE / 3]; 		//< buffer to store the current command
static char args[CMD_MAX_ARGS][CMD_MAX_LINE/3]; //< buffer to store command arguments
static uint8_t arg_counter;						//< counter to keep track of number of arguments received for the current command/
static uint16_t rx_buffer_idx;					//< index to keep track of the position in rx_buffer where the next received character will be stored
static int8_t cmd_idx;							//< index to keep track of the position of the current command in cmds array. It is set in CMD_PROCESS state and used in CMD_EXEC state
static cmd_status_t cmdParserStatus;			//< variable to keep track of the status of the command parser. It is set in CMD_ERROR state and used to print the error message in that state
static bool_t cmdParserInitialized = false;		//< flag to check if cmd parser has been initialized. It is set in cmdParserInit() and checked in cmdPoll()
static cmd_t cmds[CMD_MAX_SIZE];								//< pointer to the array of cmd_t that contains the list of commands. It is set in cmdParserInit() and used in cmdProcessLine() to find the command to execute
static uint16_t cmd_count;						//< variable to store the number of commands in cmds array. It is set in cmdParserInit() and used in cmdProcessLine() to iterate through cmds array


static void cmdParserReset() {
	rx_buffer_idx = 0;
	arg_counter = 0;
	memset(rx_buffer, 0, sizeof(rx_buffer)/sizeof(rx_buffer)[0]);
	memset(curr_cmd, 0, sizeof(curr_cmd)/sizeof(curr_cmd)[0]);
	memset(args, 0, sizeof(args));
	cmdParserFSM = CMD_IDLE;
}

static void tokenizeLine() {
	char *delimiter_ptr = strchr(rx_buffer, CMD_DELIMITER);
	if(delimiter_ptr == NULL) {
		memcpy(curr_cmd, rx_buffer, strlen(rx_buffer) + 1);
	} else {
		memcpy(curr_cmd, rx_buffer, delimiter_ptr - rx_buffer);
		curr_cmd[delimiter_ptr - rx_buffer + 1] = EOL;
		// check if there are no characters after delimiter
		if(*(delimiter_ptr + sizeof(char)) == EOL) {
			return;
		}
		// if there are characters, find for the first SPACE char
		char *old_ptr = delimiter_ptr;
		char *space_ptr = strchr(old_ptr, SPACE);

		// if space is found, then we have more than one argument and we need to parse them all
		// iterate until space char is not found or we reached max args
		while(space_ptr != NULL && arg_counter <= CMD_MAX_ARGS ) {
			memcpy(args[arg_counter++], old_ptr + 1, space_ptr - old_ptr);
			// update old_ptr with current space
			old_ptr = space_ptr;
			space_ptr = strchr(old_ptr, SPACE);
		}

		// if space not found, then we have only one argument
		if(space_ptr == NULL && arg_counter <= CMD_MAX_ARGS) {
			memcpy(args[arg_counter++], old_ptr + 1, rx_buffer_idx -  (old_ptr - rx_buffer));
		}
	}
}

/**
 * @brief process a line from rx_bufferA
 *
 * @param[out] arg, pointer to char buffer where the extracted argument
 * @return index of command in @ref cmds otherwise returns -1 if not found
 */
static int8_t cmdProcessLine() {
	/// split rx_buffer into cmd + arg1 + arg2
	/// extract command from rx_buffer
	size_t i;

	tokenizeLine();
	for(i = 0; i < cmd_count; i++) {
		if(strcmp(strupr(curr_cmd), cmds[i].cmd) == 0) {
			// return found index from cmds
			return i;
		}
	}
	/// not found
	return -1;
}

/**
 * @brief helper function to convert cmd_status_t error codes to human readable strings
 * @param[in] error cmd_status_t error code
 * @return pointer to string literal with the error message corresponding to the input error code
 */
inline static char * err_to_str(cmd_status_t error) {
	switch(error) {
	case CMD_ERR_OVERFLOW:
		return "ERROR: line too long\r\n";
	case CMD_ERR_IGNORED:
		return "ERROR: comments starting with # or // are not accepted\r\n";
	case CMD_ERR_SYNTAX:
		return "ERROR: wrong syntax\r\n";
	case CMD_ERR_UNKNOWN:
		return "ERROR: unknown command\r\n";
	case CMD_ERR_EOL_RECEIVED:
		return "ERROR: received end-of-line\r\n";
	case CMD_ERR_ARG:
		return "ERROR: bad arguments\r\n";
	default:
		return "ERROR: unknown error\r\n";
	}
}

void cmdParserInit(const cmd_t *custom_cmds, size_t num_cmds)
{
	if(custom_cmds == NULL) {
		uartSendString("Error: cmdParserInit requires a non-null pointer to an array of cmd_t and the number of commands in that array\r\n");
		return;
	}
	if(num_cmds == 0) {
		uartSendString("Error: cmdParserInit requires at least one command in the array of cmd_t\r\n");
		return;
	}
	if(num_cmds > CMD_MAX_SIZE) {
		uartSendString("Error: cmdParserInit received more commands than the maximum allowed. Max number of commands is 32\r\n");
		return;
	}
	cmdParserFSM = CMD_IDLE;
	cmdParserStatus = CMD_OK;
	rx_buffer_idx = 0;
	arg_counter = 0;
	cmdParserInitialized = true;
	memcpy(cmds, custom_cmds, num_cmds * sizeof(cmd_t));
	cmd_count = num_cmds;
}

void cmdPoll(void)
{
	// check that cmd state machine has been initialized
	if(cmdParserInitialized == false) { return; }

	uint8_t iterations = 16;
	char c;
	while(iterations--) {
		switch(cmdParserFSM){
		case CMD_IDLE:
			// if there are no bytes to read, return immediately
			if(uartReceiveStringSize(&c, 1) != true) return;
			// EOL, Carriage return, line feed and comment should be ignored
			if(c != EOL && c != CARRIAGE_RETURN && c!= LINE_FEED) {
				cmdParserFSM = CMD_RECEIVING;
				rx_buffer[rx_buffer_idx++] = c;
			}
			break;
		case CMD_RECEIVING:
			// if there are no bytes to read, return immediately
			if(uartReceiveStringSize(&c, 1) != true) return;
			// ignore comments. We could ignore '#' at CMD_IDLE, but in that case it wouldn't make
			// sense to change from IDLE to ERROR, so we are checking that in this instance
			if((rx_buffer_idx == 1 && rx_buffer[0] == SLASH && c == SLASH) || rx_buffer[0] == NUMERAL) {
				cmdParserFSM = CMD_ERROR;
				cmdParserStatus = CMD_ERR_IGNORED;
				break;
			}

			// reached carriage return or line feed go to CMD_PROCESS
			if(c == CARRIAGE_RETURN || c == LINE_FEED) {
				// add null terminator
				rx_buffer[rx_buffer_idx] = EOL;
				cmdParserFSM = CMD_PROCESS;
				break;
			}

			// we did not expect EOL at this stage... or never actually
			if(c == EOL) {
				cmdParserFSM = CMD_ERROR;
				cmdParserStatus = CMD_ERR_EOL_RECEIVED;
				break;
			}

			/// at this stage, the character is valid
			// we need to check if we have not reached max buffer size
			if(rx_buffer_idx == CMD_MAX_LINE - 1) {
				cmdParserFSM = CMD_ERROR;
				cmdParserStatus = CMD_ERR_OVERFLOW;
				break;
			}

			/// ignore multiple tabs or spaces
			if((c == SPACE && rx_buffer[rx_buffer_idx - 1] == SPACE) ||
			    (c == TAB && rx_buffer[rx_buffer_idx - 1] == TAB)) {
				break;
			}

			// we passed all our tests!! add it to rx_buffer
			rx_buffer[rx_buffer_idx++] = c;
			break;
		case CMD_PROCESS:
			// process receved line
			cmd_idx = cmdProcessLine();
			// check if we have a valid cmd index
			if(cmd_idx == -1) {
				cmdParserStatus = CMD_ERR_UNKNOWN;
				cmdParserFSM = CMD_ERROR;
			} else {
				cmdParserFSM = CMD_EXEC;
			}
			break;
		case CMD_EXEC:
			// execute command from list of cmd_t
			cmdParserStatus = cmds[cmd_idx].func(args, arg_counter);
			// check if cmd failed
			if(cmdParserStatus != CMD_OK) {
				cmdParserFSM = CMD_ERROR;
				break;
			}
			cmdParserReset();
			break;
		case CMD_ERROR:
			// print error msg
			uartSendString(err_to_str(cmdParserStatus));
			cmdParserReset();
			break;
		}
	}
}
