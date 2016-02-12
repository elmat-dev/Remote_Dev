#ifndef __UDP_ECHOCLIENT_H__
#define __UDP_ECHOCLIENT_H__


#include "cmdline_functions.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "tcp.h"
#include "pbuf.h"
#include "udp.h"





/* @brief: Initialize UDP Server
 * @param: None
 * @retval: None
 */
void udp_echoserver_init(void);


/* @brief: Initialize UDP Server with given port number
 * @param: port number on which data will be send
 * @retval: None
 */
void udp_echoserver_initWithPort(short);


/* @brief: Send data to server at given IP address
 * @param: data - buffer containing the data which should be sent
 * @param: length - lsength of data which should be send, must be lsee than PKT_SIZE
 * @retval: None
 */
void sendToServer(char*, uint16_t, IPAddress);


/* @brief: Send collected data to server
 * @param: None
 * @retval: None
 */
void Send_Data(void);


/* @brief: Send data to server at given IP address
 * @param: *data - buffer containing the data which should be sent, must be shorter than PKT_SIZE
 * @retval: None
 */
void ethernetSendString(char* message);


/* @brief: Read power received by PoF, INA219 must be installed in the device
 * @param: None
 * @retval: level of power received by PoF module, given in mW, read error can reach 30%
 * 			if module is not present, then -500.0 is returned
 */
float input_power(void);






#endif /* __UDP_ECHOCLIENT_H__ */
