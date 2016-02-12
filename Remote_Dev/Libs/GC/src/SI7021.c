#include "SI7021.h"



// I2C commands
uint8_t RH_READ[]           = { 0xE5 };
uint8_t TEMP_READ[]         = { 0xE3 };
uint8_t POST_RH_TEMP_READ[] = { 0xE0 };
uint8_t SI7021_RESET[]      = { 0xFE };
uint8_t USER1_READ[]        = { 0xE7 };
uint8_t USER1_WRITE[]       = { 0xE6 };
uint8_t SERIAL1_READ[]      = { 0xFA, 0x0F };
uint8_t SERIAL2_READ[]      = { 0xFC, 0xC9 };

bool _si_exists = false;



bool SI7021_begin() {
  _si_exists = false;
  I2C_DeInit(SI7021_I2C_PORT);
  TM_I2C_Init(SI7021_I2C_PORT, SI7021_I2C_PINS, SI7021_I2C_SPEED);
  
  if (TM_I2C_IsDeviceConnected(SI7021_I2C_PORT, SI7021_ADDRESS)) {
    _si_exists = true;
  }
  
  return _si_exists;
}



bool SI7021_sensorExists() {
    return _si_exists;
}

float SI7021_getFahrenheitHundredths() {
    unsigned int c = SI7021_getCelsiusHundredths();
    return (1.8 * c) + 3200;
}


int SI7021_getCelsiusHundredths() {
    uint8_t tempbytes[2];
    SI7021_writeReg(TEMP_READ, sizeof TEMP_READ);
    Delayms(50);
    SI7021_readReg(tempbytes, sizeof tempbytes);
    long tempraw = (long)tempbytes[0] << 8 | tempbytes[1];
    return ((17572 * tempraw) >> 16) - 4685;
}

int SI7021_getCelsiusPostHumidity() {
    uint8_t tempbytes[2];
    SI7021_writeReg(POST_RH_TEMP_READ, sizeof POST_RH_TEMP_READ);
    SI7021_readReg(tempbytes, sizeof tempbytes);
    long tempraw = (long)tempbytes[0] << 8 | tempbytes[1];
    return ((17572 * tempraw) >> 16) - 4685;
}


unsigned int SI7021_getHumidityPercent() {
    uint8_t humbytes[2];
    SI7021_writeReg(RH_READ, sizeof RH_READ);
    Delayms(50);
    SI7021_readReg(humbytes, sizeof humbytes);
    long humraw = (long)humbytes[0] << 8 | humbytes[1];
    return ((125 * humraw) >> 16) - 6;
}

unsigned int SI7021_getHumidityBasisPoints() {
    uint8_t humbytes[2];
    SI7021_writeReg(RH_READ, sizeof RH_READ);
    Delayms(50);
    SI7021_readReg(humbytes, sizeof humbytes);
    long humraw = (long)humbytes[0] << 8 | humbytes[1];
    return ((12500 * humraw) >> 16) - 600;
}

void SI7021_command(uint8_t * cmd, uint8_t * buf ) {
    SI7021_writeReg(cmd, sizeof cmd);
    Delayms(20);
    SI7021_readReg(buf, sizeof buf);
}

void SI7021_writeReg(uint8_t * reg, int reglen) {
    TM_I2C_WriteMultiNoRegister(SI7021_I2C_PORT, SI7021_ADDRESS, reg, reglen);
}

int SI7021_readReg(uint8_t * reg, int reglen) {
	TM_I2C_ReadMultiNoRegister(SI7021_I2C_PORT, SI7021_ADDRESS, reg, reglen);
    return 1;
}

//note this has crc bytes embedded, per datasheet, so provide 12 byte buf
int SI7021_getSerialBytes(uint8_t * buf) {
    SI7021_writeReg(SERIAL1_READ, sizeof SERIAL1_READ);
    SI7021_readReg(buf, 6);
 
    SI7021_writeReg(SERIAL2_READ, sizeof SERIAL2_READ);
    SI7021_readReg(buf + 6, 6);
    
    return 1;
}

int SI7021_getDeviceId() {
    uint8_t serial[12];
    SI7021_getSerialBytes(serial);
    int id = serial[6];
    return id;
}

// get humidity, then get temperature reading from humidity measurement
int SI7021_getHumidityAndTemperature(si7021_env * sensor_data ) {
  if(SI7021_begin()==0) {
    sensor_data->connected = 0;
    sensor_data->humidityBasisPoints = 50000;
    sensor_data->celsiusHundredths = -50000;
    sensor_data->fahrenheitHundredths = -50000;
  }
  else {
    sensor_data->humidityBasisPoints      = SI7021_getHumidityBasisPoints();
    sensor_data->celsiusHundredths        = SI7021_getCelsiusPostHumidity();
    sensor_data->fahrenheitHundredths     = (1.8 * sensor_data->celsiusHundredths) + 3200;
  }
    return 1;
}
