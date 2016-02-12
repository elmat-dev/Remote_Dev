/*
INA219.cpp - Class file for the INA219 Zero-Drift, Bi-directional Current/Power Monitor Arduino Library.

Version: 1.0.0
(c) 2014 Korneliusz Jarzebski
www.jarzebski.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "INA219.h"

bool INA219_begin()
{   
    I2C_DeInit(INA219_I2C_PORT);
    TM_I2C_Init(INA219_I2C_PORT, INA219_I2C_PINS, INA219_I2C_SPEED);
	if(TM_I2C_IsDeviceConnected(INA219_I2C_PORT,INA219_ADDRESS)) {
		return true;
		}
	else return false;
}

bool INA219_configure(ina219_range_t range, ina219_gain_t gain, ina219_busRes_t busRes, ina219_shuntRes_t shuntRes, ina219_mode_t mode)
{
    uint16_t ina_config = 0;

    ina_config |= (range << 13 | gain << 11 | busRes << 7 | shuntRes << 3 | mode);

    switch(range)
    {
        case INA219_RANGE_32V:
            vBusMax = 32.0f;
            break;
        case INA219_RANGE_16V:
            vBusMax = 16.0f;
            break;
    }

    switch(gain)
    {
        case INA219_GAIN_320MV:
            vShuntMax = 0.32f;
            break;
        case INA219_GAIN_160MV:
            vShuntMax = 0.16f;
            break;
        case INA219_GAIN_80MV:
            vShuntMax = 0.08f;
            break;
        case INA219_GAIN_40MV:
            vShuntMax = 0.04f;
            break;
    }

    writeRegister16(INA219_REG_CONFIG, ina_config);

    return true;
}

bool INA219_calibrate(float rShuntValue, float iMaxExpected)
{
    uint16_t calibrationValue;
    rShunt = rShuntValue;

    float minimumLSB;
    //float iMaxPossible = vShuntMax / rShunt;

    minimumLSB = iMaxExpected / 32767;

    currentLSB = (uint16_t)(minimumLSB * 100000000);
    currentLSB /= 100000000;
    currentLSB /= 0.0001;
    currentLSB = ceil(currentLSB);
    currentLSB *= 0.0001;

    powerLSB = currentLSB * 20;

    calibrationValue = (uint16_t)((0.04096) / (currentLSB * rShunt));

    writeRegister16(INA219_REG_CALIBRATION, calibrationValue);

    return true;
}

float INA219_getMaxPossibleCurrent(void)
{
    return (vShuntMax / rShunt);
}

float INA219_getMaxCurrent(void)
{
    float maxCurrent = (currentLSB * 32767);
    float maxPossible = INA219_getMaxPossibleCurrent();

    if (maxCurrent > maxPossible)
    {
        return maxPossible;
    } else
    {
        return maxCurrent;
    }
}

float INA219_getMaxShuntVoltage(void)
{
    float maxVoltage = INA219_getMaxCurrent() * rShunt;

    if (maxVoltage >= vShuntMax)
    {
        return vShuntMax;
    } else
    {
        return maxVoltage;
    }
}

float INA219_getMaxPower(void)
{
    return (INA219_getMaxCurrent() * vBusMax);
}

float INA219_readBusPower(void)
{
    return (readRegister16(INA219_REG_POWER) * powerLSB);
}

float INA219_readShuntCurrent(void)
{
    return (readRegister16(INA219_REG_CURRENT) * currentLSB);
}

float INA219_readShuntVoltage(void)
{
    float voltage;

    voltage = readRegister16(INA219_REG_SHUNTVOLTAGE);

    return (voltage / 100000.0000);
}

float INA219_readBusVoltage(void)
{
    int16_t voltage;

    voltage = readRegister16(INA219_REG_BUSVOLTAGE);
    voltage >>= 3;

    return (voltage * 0.004);
}

ina219_range_t INA219_getRange(void)
{
    uint16_t value;

    value = readRegister16(INA219_REG_CONFIG);
    value &= 0x2000;
    value >>= 13;

    return (ina219_range_t)value;
}

ina219_gain_t INA219_getGain(void)
{
    uint16_t value;

    value = readRegister16(INA219_REG_CONFIG);
    value &= 0x1800;
    value >>= 11;

    return (ina219_gain_t)value;
}

ina219_busRes_t INA219_getBusRes(void)
{
    uint16_t value;

    value = readRegister16(INA219_REG_CONFIG);
    value &= 0x0780;
    value >>= 7;

    return (ina219_busRes_t)value;
}

ina219_shuntRes_t INA219_getShuntRes(void)
{
    uint16_t value;

    value = readRegister16(INA219_REG_CONFIG);
    value &= 0x0078;
    value >>= 3;

    return (ina219_shuntRes_t)value;
}

ina219_mode_t INA219_getMode(void)
{
    uint16_t value;

    value = readRegister16(INA219_REG_CONFIG);
    value &= 0x0007;

    return (ina219_mode_t)value;
}

int16_t readRegister16(uint8_t reg)
{
    int16_t value;
    uint8_t tmp[2];
	
	TM_I2C_ReadMulti(INA219_I2C_PORT, INA219_ADDRESS, reg, tmp, 2);
    TM_I2C_Stop(INA219_I2C_PORT);
    value = tmp[0] << 8 | tmp[1];

    return value;
}

void writeRegister16(uint8_t reg, uint16_t val)
{
  uint8_t vla;
  vla = (uint8_t)val;  
  val >>= 8;
  uint8_t tmp[2] = {val, vla };
	TM_I2C_WriteMulti(INA219_I2C_PORT, INA219_ADDRESS, reg, tmp, 2);
		
}



//for internal use:
void INA219_checkConfig()
{
  printf("Mode:                 ");
  switch (INA219_getMode())
  {
    case INA219_MODE_POWER_DOWN:      printf("Power-Down\n\r"); break;
    case INA219_MODE_SHUNT_TRIG:      printf("Shunt Voltage, Triggered\n\r"); break;
    case INA219_MODE_BUS_TRIG:        printf("Bus Voltage, Triggered\n\r"); break;
    case INA219_MODE_SHUNT_BUS_TRIG:  printf("Shunt and Bus, Triggered\n\r"); break;
    case INA219_MODE_ADC_OFF:         printf("ADC Off\n\r"); break;
    case INA219_MODE_SHUNT_CONT:      printf("Shunt Voltage, Continuous\n\r"); break;
    case INA219_MODE_BUS_CONT:        printf("Bus Voltage, Continuous\n\r"); break;
    case INA219_MODE_SHUNT_BUS_CONT:  printf("Shunt and Bus, Continuous\n\r"); break;
    default: printf("unknown\n\r");
  }
  
  printf("Range:                ");
  switch (INA219_getRange())
  {
    case INA219_RANGE_16V:            printf("16V\n\r"); break;
    case INA219_RANGE_32V:            printf("32V\n\r"); break;
    default: printf("unknown\n\r");
  }

  printf("Gain:                 ");
  switch (INA219_getGain())
  {
    case INA219_GAIN_40MV:            printf("+/- 40mV\n\r"); break;
    case INA219_GAIN_80MV:            printf("+/- 80mV\n\r"); break;
    case INA219_GAIN_160MV:           printf("+/- 160mV\n\r"); break;
    case INA219_GAIN_320MV:           printf("+/- 320mV\n\r"); break;
    default: printf("unknown\n\r");
  }

  printf("Bus resolution:       ");
  switch (INA219_getBusRes())
  {
    case INA219_BUS_RES_9BIT:         printf("9-bit\n\r"); break;
    case INA219_BUS_RES_10BIT:        printf("10-bit\n\r"); break;
    case INA219_BUS_RES_11BIT:        printf("11-bit\n\r"); break;
    case INA219_BUS_RES_12BIT:        printf("12-bit\n\r"); break;
    default: printf("unknown\n\r");
  }

  printf("Shunt resolution:     ");
  switch (INA219_getShuntRes())
  {
    case INA219_SHUNT_RES_9BIT_1S:    printf("9-bit / 1 sample\n\r"); break;
    case INA219_SHUNT_RES_10BIT_1S:   printf("10-bit / 1 sample\n\r"); break;
    case INA219_SHUNT_RES_11BIT_1S:   printf("11-bit / 1 sample\n\r"); break;
    case INA219_SHUNT_RES_12BIT_1S:   printf("12-bit / 1 sample\n\r"); break;
    case INA219_SHUNT_RES_12BIT_2S:   printf("12-bit / 2 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_4S:   printf("12-bit / 4 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_8S:   printf("12-bit / 8 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_16S:  printf("12-bit / 16 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_32S:  printf("12-bit / 32 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_64S:  printf("12-bit / 64 samples\n\r"); break;
    case INA219_SHUNT_RES_12BIT_128S: printf("12-bit / 128 samples\n\r"); break;
    default: printf("unknown\n\r");
  }

  printf("Max possible current: ");
  printf("%2.3f",INA219_getMaxPossibleCurrent());
  printf(" A\n\r");

  printf("Max current:          ");
  printf("%2.3f",INA219_getMaxCurrent());
  printf(" A\n\r");

  printf("Max shunt voltage:    ");
  printf("%2.3f",INA219_getMaxShuntVoltage());
  printf(" V\n\r");

  printf("Max power:            ");
  printf("%2.3f",INA219_getMaxPower());
  printf(" W\n\r");
}