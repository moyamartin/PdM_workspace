#ifndef API_CMDPARSER_H_
#define API_CMDPARSER_H_

#include "API_uart.h"

#define CMD_MAX_LINE 64 	// include EOL character ('\0')
#define CMD_MAX_ARGS 2		// Max args per command
#define CMD_MAX_SIZE 32		// Max number of commands that can be registered in cmds array
#define LINE_FEED 		'\n'
#define CARRIAGE_RETURN '\r'
#define EOL				'\0'
#define SPACE			' '
#define TAB				'\t'
#define NUMERAL			'#'
#define SLASH			'/'
#define CMD_DELIMITER	'='

/**
 * @brief representation of cmd parser status
 */
typedef enum {
	CMD_OK = 0,				//< Command received and processed successfully
	CMD_ERR_NONE,			//< No command received yet
	CMD_ERR_TOO_LONG,		//< Received command exceeds maximum length
	CMD_ERR_OVERFLOW,		//< Received command exceeds buffer capacity
	CMD_ERR_IGNORED,		//< Received command is a comment and will be ignored
	CMD_ERR_SYNTAX,			//< Received command has syntax error
	CMD_ERR_UNKNOWN,		//< Received command is not recognized
	CMD_ERR_EOL_RECEIVED,	//< Received unexpected end of line character
	CMD_ERR_ARG,			//< Received command has invalid argument(s)
} cmd_status_t;

/**
 * @brief representation of cmd parser FSM states
 */
typedef enum {
	CMD_IDLE = 0,	//< Waiting for a new command
	CMD_RECEIVING,	//< Receiving characters of the current command
	CMD_PROCESS,	//< Processing the received command line
	CMD_EXEC,		//< Executing the matched command function
	CMD_ERROR,		//< An error occurred during command reception, processing, or execution
} cmd_fsm_state_t;

/**
 * @brief representation of a cmd
 */
typedef struct {
	char cmd[CMD_MAX_LINE];		//< Command string (e.g., "LED ON")
	char desc[MAX_TX_SIZE];		//< Command description for help text
	cmd_status_t (*func)(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);	//< Pointer to the function that executes the command, returns cmd_status_t
} cmd_t;

/**
 * @brief initialize commander parser system
 */
void cmdParserInit(const cmd_t *custom_cmds, size_t num_cmds);

/**
 * @brief updates parser's state machine.
 *
 * @note must be called periodically from main's superloop.
 * It process up to 64 bytes per invocation (non-blocking).
 */
void cmdPoll(void);

#endif /* API_CMDPARSER_H_ */
