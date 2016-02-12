#ifndef GSM_H
#define GSM_H


#include "tm_stm32f4_usart.h"
#include "utilities_new.h"
#include <string.h>
#include <stdbool.h>

#define STATE_NONE 0
#define STATE_ON 1
#define STATE_INITIALIZED 2
#define STATE_REGISTERED 4
#define STATE_POSFIX 8

#define GSM_MODEM USART1

  char pin[5];
  uint8_t gsm_state;
  uint8_t onOffPin;

#define BUF_LENGTH 100




/* @brief: Initialize GSM library, enable power to GSM module and so on
 * @param: None
 * @retval: None
 */
void GSM_Init();


/*
 * @brief Skips the string until the given char is found.
 * @param *str - pointer to character string in which you want to search for a character
 * @param match - character which you want to find in the string
 * @retval: pointer to the the character which was searched
 */
char *GSM_skip(char *str, char match);


/*
 * @brief: Reads a token from the given string. Token is seperated by the given delimiter.
 * @param: *str - pointer to character string where you will search for token
 * @param: *buf - pointer to the buffer where token will be saved
 * @param: delimiter - character which is considered as token delimiter
 * @retval: pointer to the place in the string where token was found
 */
char *GSM_readToken(char *str, char *buf, char delimiter);


/* @brief: Sends command to GSM module and wait response for given period of time
 * @param: *command - pointer to array of characters containing data which should be send to GSM module
 * @param: timeout - time for which function will wait for response from GSM module
 * @param: check
 * 			- if set to 1 function will wait for 'OK' response
 * 			- if set to 0 function will collect data which will be send before timeout will ocurr
 * @param: *buf - pointer to buffer to which data received from GSM module will be stored
 * @retval: number of characters received from GSM module
 */
uint8_t GSM_requestModem( char *command, uint16_t timeout, bool check, char *buf);


/* @brief: Sends command to GSM module and wait for defined response from GMS module
 * @param: *command - pointer to array of characters containing data which should be send to GSM module
 * @param: timeout - time for which function will wait for response from GSM module
 * @param: check - to decide if function should compare repopsne from GSM module with expected response
 * @param: *expected_resp - pointer to char array with defined expected response
 * @param: *buf - pointer to buffer to which data received from GSM module will be stored
 * @retval: number of characters received from GSM module
 */
uint8_t GSM_requestModemWaitAnswer(char *command, uint16_t timeout, bool check, char *expected_resp, char *buf);


/* @brief: Function receive data from GSM module for defined period of time
 * @param: *buf - pointer to character array to which collected data will be saved
 * @param: timeout - time (in milliseconds) for which function will wait for data from GSM module
 * @retval: number of characters received from GSM module
 */
uint8_t GSM_getsTimeout(char *buf, uint16_t timeout);


/* @brief: Sends text message via GSM network
 * @param: *number - phone number to which you want to send a message, should look like that: "+48123456789"
 * @param: *message - message which you want to sent to given phone number
 * @retval: None
 */
void GSM_sendSMS(char *number, char *message);



/* @brief: Update GSM Network registration status
 * @param: timeout - the time for which function will check for response from GSM module
 * @retval: None
 */
void GSM_checkNetwork_Simple(int timeout);

/* @brief: Check GSM Network registration status
 * @param: None
 * @retval:
 * 			true if successfully registered to the network
 * 			false if not registered in the network
 *  		Before reading the status you should update current status by issuing
 *  		void GSM_checkNetwork_Simple(int timeout) command
 */
bool GSM_isRegistered(void);



/* Not used or implemented in this firmware release */
bool GSM_isOn(void);
bool GSM_isInitialized(void);
void GSM_checkNetwork();
void send(char *buf);
char *receive(char *buf);
void GSM_switchModem(void);
void version(void);
void GSM_switchOn(void);
void GSM_switchOff(void);


#endif
