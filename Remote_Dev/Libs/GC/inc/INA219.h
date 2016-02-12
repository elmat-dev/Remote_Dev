/*
 * INA219.h - Header file for the Zero-Drift, Bi-directional Current/Power Monitor Arduino Library.
 * Version: 1.0.0
 * (c) 2014 Korneliusz Jarzebski
 * www.jarzebski.pl
 */


#ifndef INA219_h
#define INA219_h

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "tm_stm32f4_i2c.h"

#define INA219_I2C_PORT             I2C3
#define INA219_I2C_PINS             TM_I2C_PinsPack_1
#define INA219_I2C_SPEED            200000
#define INA219_ADDRESS              (0x42 << 1)


#define INA219_CMD_READ             (0x01)
#define INA219_REG_CONFIG           (0x00)
#define INA219_REG_SHUNTVOLTAGE     (0x01)
#define INA219_REG_BUSVOLTAGE       (0x02)
#define INA219_REG_POWER            (0x03)
#define INA219_REG_CURRENT          (0x04)
#define INA219_REG_CALIBRATION      (0x05)

typedef enum
{
    INA219_RANGE_16V            = 0x00,
    INA219_RANGE_32V            = 0x01
} ina219_range_t;

typedef enum
{
    INA219_GAIN_40MV            = 0x00,
    INA219_GAIN_80MV            = 0x01,
    INA219_GAIN_160MV           = 0x02,
    INA219_GAIN_320MV           = 0x03
} ina219_gain_t;

typedef enum
{
    INA219_BUS_RES_9BIT         = 0x00,
    INA219_BUS_RES_10BIT        = 0x01,
    INA219_BUS_RES_11BIT        = 0x02,
    INA219_BUS_RES_12BIT        = 0x03
} ina219_busRes_t;

typedef enum
{
    INA219_SHUNT_RES_9BIT_1S    = 0x00,
    INA219_SHUNT_RES_10BIT_1S   = 0x01,
    INA219_SHUNT_RES_11BIT_1S   = 0x02,
    INA219_SHUNT_RES_12BIT_1S   = 0x03,
    INA219_SHUNT_RES_12BIT_2S   = 0x05,
    INA219_SHUNT_RES_12BIT_4S   = 0x0A,
    INA219_SHUNT_RES_12BIT_8S   = 0x0B,
    INA219_SHUNT_RES_12BIT_16S  = 0x0C,
    INA219_SHUNT_RES_12BIT_32S  = 0x0D,
    INA219_SHUNT_RES_12BIT_64S  = 0x0E,
    INA219_SHUNT_RES_12BIT_128S = 0x0F
} ina219_shuntRes_t;

typedef enum
{
    INA219_MODE_POWER_DOWN      = 0x00,
    INA219_MODE_SHUNT_TRIG      = 0x01,
    INA219_MODE_BUS_TRIG        = 0x02,
    INA219_MODE_SHUNT_BUS_TRIG  = 0x03,
    INA219_MODE_ADC_OFF         = 0x04,
    INA219_MODE_SHUNT_CONT      = 0x05,
    INA219_MODE_BUS_CONT        = 0x06,
    INA219_MODE_SHUNT_BUS_CONT  = 0x07,
} ina219_mode_t;




	extern float currentLSB, powerLSB;
	extern float vShuntMax, vBusMax, rShunt;

	bool INA219_begin();
	bool INA219_configure(ina219_range_t, ina219_gain_t, ina219_busRes_t, ina219_shuntRes_t, ina219_mode_t);
	bool INA219_calibrate(float, float);

	ina219_range_t INA219_getRange(void);
	ina219_gain_t INA219_getGain(void);
	ina219_busRes_t INA219_getBusRes(void);
	ina219_shuntRes_t INA219_getShuntRes(void);
	ina219_mode_t INA219_getMode(void);

	float INA219_readShuntCurrent(void);
	float INA219_readShuntVoltage(void);
	float INA219_readBusPower(void);
	float INA219_readBusVoltage(void);

	float INA219_getMaxPossibleCurrent(void);
	float INA219_getMaxCurrent(void);
	float INA219_getMaxShuntVoltage(void);
	float INA219_getMaxPower(void);
    void  INA219_checkConfig(void);

	void writeRegister16(uint8_t reg, uint16_t val);
	int16_t readRegister16(uint8_t reg);

#endif
