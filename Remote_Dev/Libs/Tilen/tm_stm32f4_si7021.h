/**
 *	Si7021 Humidity & Temperature sensor library for STM32F4
 *
 *	@author 	Grzegogrz Chmiel
 *	@email		dashko@o2.pl	
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-37-bmp180-pressure-sensor-stm32f4
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Grzegorz Chmiel, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 * 
 * You can read humidity & temperature with this sensor.
 * 
 * 
 * Default pinout:
 * 	
 * 	Si7021		STM32F4		DESCRIPTION
 * 	
 * 	SCL			PA8			I2C3 Serial clock
 * 	SDA			PC9			I2C3 Serial data
 * 
 * Make sure, that you connect VCC to 3.3V and not to 5V, because you can blow device.
 * If you want to change default I2C settings, open defines.h file and edit lines below:
 * 
 *	//Select custom I2C
 *	//#define SI7021_I2C				I2C1
 *	//#define SI7021_I2C_PINSPACK		        TM_I2C_PinsPack_2
 * 	
 */
#ifndef TM_SI7021_H
#define TM_SI7021_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM I2C
 * - math.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_i2c.h"
#include "defines.h"
#include "math.h"

/* Default I2C pin */
#ifndef SI7021_I2C
#define SI7021_I2C				I2C3
#define SI7021_I2C_PINSPACK			TM_I2C_PinsPack_1
#endif

/* Default I2C speed */
#ifndef SI7021_I2C_SPEED
#define SI7021_I2C_SPEED			200000
#endif

/* Si7021 I2C address */
#ifndef SI7021_I2C_ADDRESS
#define SI7021_I2C_ADDRESS			0x40 << 1
#endif

  


/* Commands */
#define SI7021_TRIGGER_HUMD_MEASURE_HOLD        0xE5
#define SI7021_TRIGGER_HUMD_MEASURE_NOHOLD      0xF5 
#define SI7021_TRIGGER_TEMP_MEASURE_HOLD        0xE3
#define SI7021_TRIGGER_TEMP_MEASURE_NOHOLD      0xF3
#define SI7021_READ_TEMP_PREV_MEASURMENT        0xE0
#define SI7021_SOFT_RESET                       0xFE
#define SI7021_WRITE_USER_REG_1                 0xE6
#define SI7021_READ_USER_REG_1                  0xE7




/**
 * Si7021 result typedef enumerations
 * Used for result from functions
 * 
 * Parameters:
 * 	- Si7021_Result_Ok:
 * 		Everything is OK
 * 	- Si7021_Result_DeviceNotConnected:
 * 		Device is not connected to I2C
 * 	- Si7021_Result_LibraryNotInitialized
 * 		Library is not initialized
 */
typedef enum {
	TM_SI7021_Result_Ok = 0x00,
	TM_SI7021_Result_DeviceNotConnected,
	TM_SI7021_Result_LibraryNotInitialized
} TM_SI7021_Result_t;

/**
 * SI7021 working struct
 * 
 * Parameters:
 * 	- float Temp:
 * 		Calculated temperature
 * 	- float Humid:
 * 		Calculated humidity
 * 	- uint8_t crc:
 * 		CRC check value
 * 	- uint16_t Delay:
 * 		Number of microseconds, that sensor needs to calculate data that you request to
 */
typedef struct {
	float Temp;
	float Humid;
	uint8_t crc;
	uint16_t Delay;
        uint8_t lib_status;
} TM_SI7021_t;


extern TM_SI7021_Result_t TM_SI7021_Init(TM_SI7021_t* );
/* Just request to make temperature measurment, processing takes about 50ms, so user CPU can do something else and then go back when data will be ready*/
extern TM_SI7021_Result_t TM_SI7021_RequestTemperature(void);
/* Just request to make humidity measurment, processing takes about 50ms, so user CPU can do something else and then go back when data will be ready*/
extern TM_SI7021_Result_t TM_SI7021_RequestHumidity(void);


//extern void TM_SI7021_Conv_2_Humidity(TM_SI7021_t *, uint16_t );
//extern void TM_SI7021_Conv_2_Temperature(TM_SI7021_t *, uint16_t);

extern TM_SI7021_Result_t TM_SI7021_requestAndReadTemperature(TM_SI7021_t* );
extern TM_SI7021_Result_t TM_SI7021_requestAndReadTemperature2(TM_SI7021_t* );
extern TM_SI7021_Result_t TM_SI7021_requestAndReadHumidity(TM_SI7021_t* );
extern TM_SI7021_Result_t TM_SI7021_requestAndReadHumidity2(TM_SI7021_t* );


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
