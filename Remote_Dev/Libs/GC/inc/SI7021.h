#ifndef SI7021_h
#define SI7021_h

#include <stdbool.h>
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"




typedef struct si7021_env {
    int celsiusHundredths;
    float fahrenheitHundredths;
    unsigned int humidityBasisPoints;
    int connected;
} si7021_env;


/* @brief: This function is used to initialize variables for Si7021 library
 * @param: none
 * @retval: boolean
 * 			true: Si7021 module is available
 * 			false Si7021 module was not detected
 */
bool SI7021_begin();
    
/* @brief: This function is used to return variable which was set during Si7021_begin()
 * @param: none
 * @retval: boolean
 * 			true: Si7021 module is available
 * 			false Si7021 module was not detected
 */
bool SI7021_sensorExists();

/* @brief: This function returns temperature readings expressed in Farentheit degrees
 * @param: none
 * @retval: temperature reading expressed in Farentheit degrees
 */
float SI7021_getFahrenheitHundredths();

/* @brief: This function returns temperature readings expressed in Celsjus degrees
 * @param: none
 * @retval: temperature reading expressed in Celsjus degrees
 */
int SI7021_getCelsiusHundredths();

/* @brief: This function returns humidity readings expressed in %
 * @param: none
 * @retval: humidity reading expressed in %
 */
unsigned int SI7021_getHumidityPercent();

/* @brief: This function returns humidity readings expressed in basis points
 * @param: none
 * @retval: humidity reading expressed in basis points
 */
unsigned int SI7021_getHumidityBasisPoints();

/* @brief: This function returns temperature readings expressed in Celsjus degrees
 * @param: none
 * @retval: temperature reading expressed in Celsjus degrees
 * @note: this is not live reading, this value was quantized during humidity reading
 */
int SI7021_getCelsiusPostHumidity();

/* @brief: This function returns actual temperature and humidity reading
 * @param: pointer to si7021_env structure which store temperature and humidity
 * @retval: temperature reading expressed in Celsjus degrees
 */
int SI7021_getHumidityAndTemperature(si7021_env*);

/* @brief: This function is used to return serial number written in Si7021 memory)
 * @param: pointer to uint8_t buffer to which serial number will be written, must be at least 12 bytes long
 * @retval: always 1, errors are not handled in current version
 */
int SI7021_getSerialBytes(uint8_t * buf);

/* @brief: This function is used to return Si7021 ID stored in it's memory
 * @param: none
 * @retval: ID of Si7021 device
 */
int SI7021_getDeviceId();
	
/* Library internal functions */
void SI7021_command(uint8_t * cmd, uint8_t * buf );
void SI7021_writeReg(uint8_t * reg, int reglen);
int SI7021_readReg(uint8_t * reg, int reglen);


#endif

