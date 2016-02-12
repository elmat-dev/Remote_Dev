#ifndef CMDLINE_FUNCTIONS_H
#define CMDLINE_FUNCTIONS_H



#include "main.h"

#include "udp_echoclient.h"





/* @brief: Function used by configuration Menu to print data over user terminal. It's wrapper for TM_USART_Putc function
 * @param: c - character which will be printer on MENU UART
 * @retval: none
 * */
void Menu_Output(uint8_t c);

/* @brief: Function used to initialize Configuration menu library and add all supported command list
 * @param: none
 * @retval: none
 * */
void Menu_Init(void);

/* @brief: Function used for Configuration Menu handling
 * @param: none
 * @retval: none
 * */
void Config_Menu (void);

/* @brief: Function used by CLI to print Data and Time information
 * @param: none
 * @retval: none
 * */
void getTime(void);

/* @brief: Function used by CLI to set Time on the device
 * @param: none
 * @retval: none
 * */
void setTime(void);

/* @brief: Function used by CLI to set date on the device
 * @param: none
 * @retval: none
 * */
void setDate(void);


/* @brief: Function used by CLI to print DOM TX and RX power information from SFP module
 * @param: none
 * @retval: none
 * */
void show_ddmi (void);

/* @brief: Function used by CLI to print Temperature and Humidity reading from Si7021 module (if connected)
 * @param: none
 * @retval: none
 * */
void show_temp_humidity (void);


/* @brief: Function used by CLI to print System Information: IP address, Date, Time, ID and such
 * @param: none
 * @retval: none
 * */
void show_sys (void);


/* @brief: Function used by CLI to sent test packet over ethernet
 * 		   Issuing this command also enables laser on central device
 * @param: none
 * @retval: none
 * */
void eth_test(void);

/* @brief: Function used by CLI to set Device ID
 * @param: none
 * @retval: none
 * */
void set_ID(void);

/* @brief: Function used by CLI to reboot MCU to Bootloader (firmware can be updated via GPS Usart then
 * @param: none
 * @retval: none
 * */
void bootloader(void);

/* @brief: Function used by CLI to set Device local IP address and port
 * @param: none
 * @retval: none
 * */
void set_LocalIP(void);

/* @brief: Function used by CLI to set destination IP Address and port (server)
 * @param: none
 * @retval: none
 * */
void set_RemoteIP(void);

/* @brief: Function used by CLI to set network mask
 * @param: none
 * @retval: none
 * */
void set_MaskIP(void);

/* @brief: Function used by CLI to set Phone number to which SMS message with Laser power down request will be sent
 * 		   Should be set to phone number of central device
 * @param: none
 * @retval: none
 * */
void set_Phone(void);

/* @brief: Function used by CLI to set GPS location - antenna must be installed in place which has good visivility to the sky
 *         Issuing this command will block all other processes on the device for up to 60 seconds until position will be
 *         Acquired or timeout will ocurr
 * @param: none
 * @retval: none
 * */
void set_GPS(void);

/* @brief: Function used by CLI to enable/disable Interlock mechanism
 * @param: none
 * @retval: none
 * */
void InterlockToggle(void);

/* @brief: Function used by CLI to print data from INA219 power monitor
 * @param: none
 * @retval: none
 * */
void show_ina(void);

/* @brief: Function used by CLI to test GMS module, if it is working, then SMS should be sent to phone number set in configuration
 * @param: none
 * @retval: none
 * */
void gsm(void);


/* @brief: Function used by CLI to put MCU to sleep mode
 * @param: none
 * @retval: none
 * */
void goSleep(void);

#endif
