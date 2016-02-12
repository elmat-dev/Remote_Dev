#ifndef CMDLINE_H
#define CMDLINE_H



#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "tm_stm32f4_usart.h"


/* maximum number of commands the cmdline system can handle */
#define CMDLINE_MAX_COMMANDS	30

/* Maximum length (number of characters) of each command or argument string. Must include one additional byte for a null terminator */
#define CMDLINE_MAX_CMD_LENGTH	32

/* Allocated buffer size for command entry. Must be big enough to hold typed commands and the arguments that follow */
#define CMDLINE_BUFFERSIZE		64

/* Number of lines of command history to keep. Each history buffer is CMDLINE_BUFFERSIZE in size */
/* Warning: ONLY ONE LINE OF COMMAND HISTORY IS CURRENTLY SUPPORTED */
#define CMDLINE_HISTORYSIZE		1




/* @brief: Type definition for pointer to function which should be called when command has match in the database */
typedef void (*CmdlineFuncPtrType)(void);




/* @brief: Initialize the command line subsystem
 * @param: None
 * @retval: None
 */
void cmdlineInit(void);

/* @brief: Adds new command to the database of know commands
 * @param newCmdString should be a null-terminated command string with no whitespace. That's the string used to issue a command
 * @param newCmdFuncPtr should be a pointer to the function to execute when user enters corresponding command string
 * @retval: None
 * */
void cmdlineAddCommand(char* newCmdString, CmdlineFuncPtrType newCmdFuncPtr);

/* @brief: Set the function used to output character to the user terminal. In this project the TM_USART_Putc is used.
 * @param (void*(output_func)(unsigned char)) is the pointer to function which can print character to the user terminal
 * @retval: None
 */
void cmdlineSetOutputFunc(void (*output_func)(unsigned char c));

/* @brief: Call this function to pass input characters from the user terminal to cmdline "stack"
 * @param unsigned char is the character read from user terminal
 * @retval: None
 */
void cmdlineInputFunc(unsigned char c);

/* @brief: Call this function in your program's main loop to process the data to/from user terminal
 * @param: None
 * @retval: None
 */
void cmdlineMainLoop(void);







/* Internal commands */
void cmdlineRepaint(void);
void cmdlineDoHistory(uint8_t action);
void cmdlineProcessInputString(void);
void cmdlinePrintPrompt(void);
void cmdlinePrintError(void);
void cmdlineHelp(void);


/* Commands used to retrieve arguments passed from user terminal */
/* Returns a string pointer to argument number [argnum] on the command line */
char* cmdlineGetArgStr(uint8_t argnum);

/* Returns the decimal integer interpretation of argument number [argnum] */
long cmdlineGetArgInt(uint8_t argnum);

/* Returns the hex integer interpretation of argument number [argnum] */
long cmdlineGetArgHex(uint8_t argnum);

#endif
