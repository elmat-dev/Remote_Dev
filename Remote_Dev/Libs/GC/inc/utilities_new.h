#ifndef UTILITIES_NEW_H
#define UTILITIES_NEW_H



#include "main.h"

 #define GC_LOAD 0
 #define GC_SAVE 1
 #define GC_LOAD_AND_PRINT 2
 
 #define GC_ID		                	10
 #define GC_MAC                         11 
 #define GC_LOCAL_IP 	                0
 #define GC_LOCAL_PORT 	                1
 #define GC_REMOTE_IP                   2
 #define GC_REMOTE_PORT                 3
 #define GC_NETMASK 	                4
 #define GC_GATEWAY 	                5
 #define GC_LATITUDE 	                6
 #define GC_LONGITUDE 	                7
 #define GC_PHONE 	                	8
 #define GC_CALIB_TX 	                21
 #define GC_CALIB_RX 	                22

 


 #define dev_sfp                         0
 #define dev_gsm                         1
 #define dev_phy                         2
 #define dev_zspm                        3
 #define dev_gps                         4

 #define dev_on                          1
 #define dev_off                         0
 #define dev_reset                       2
 #define dev_emergency_shutdown          3









typedef union {
  float f;
  uint8_t fInt[4];
} FloatOrUInt;



/* @brief Structure that contains whole device configuration.
 * This configuration can be loaded or written from/to BKP RAM.
 * Changes made in RAM must be saved before reboot of the device
 * */
typedef struct  {
  uint8_t id[1];						// contain ID of the device
  uint8_t mac[6];						// contain MAC address of the device - currently replaced by Unique ID read from STM
  uint8_t local_ip[4];					// contain local IP address of the device
  uint8_t local_port[1];				// contain port number for UDP transmission
  uint8_t remote_ip[4];					// contain destination server IP address
  uint8_t remote_port[1];				// contain port number for UDP transmission
  uint8_t mask[4];						// contain network mask settings
  uint8_t gateway[4];					// contain gateway IP address - currently not used
  FloatOrUInt position_lat;				// contain GPS position - latitude
  FloatOrUInt position_long;			// contain GPS position - longitude
  uint8_t phone[GC_PHONE_LENGTH+1];		// contain phone number to which alarm SMS will be sent
  FloatOrUInt tx_calibration;			// SFP module calibration data - not implemented currently
  FloatOrUInt rx_calibration;			// SFP module calibration data - not implemented currently
} config_t;




/* @brief Structure that contains IP address and port number.
 * Used by some internal functions
 * */
typedef struct {
  uint8_t ip1;
  uint8_t ip2;
  uint8_t ip3;
  uint8_t ip4;
  uint8_t port;
} IPAddress;



typedef union {
  uint16_t uint16;
  uint8_t bytes[2];
} u16_bytes;






uint8_t isLaserOn (void);
float GC_BatVoltage (void);
void GC_GPS_GPIO_Config (void);
void GSM_GPIO_Config (void);
void PHY_GPIO_Config (void);
void SFP_GPIO_Config (void);
void GC_ZSPM_GPIO_Config (void);
TM_GPS_Result_t GC_CheckPosition(TM_GPS_Data_t *, uint32_t);







short parseIP (char* ip_str, uint8_t*, uint8_t);





void dev_control (uint8_t device, uint8_t state);

void bkpram(uint8_t , uint8_t *, uint8_t , uint8_t);
void config (uint8_t operation, uint8_t *data, uint8_t type);
void conf_load (config_t * conf);
void conf_save (config_t conf);

void loadIPaddress(IPAddress*, IPAddress*, IPAddress*, IPAddress*);


int load_defaults(config_t* conf);
int save_defaults(config_t conf);



#endif
