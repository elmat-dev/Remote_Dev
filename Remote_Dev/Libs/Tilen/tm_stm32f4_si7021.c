/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
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
 */
#include "tm_stm32f4_si7021.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>

uint8_t lib_initialized = 0;


TM_SI7021_Result_t TM_SI7021_Init(TM_SI7021_t* SI7021_Data) {
	/* Initialize I2C */
        I2C_DeInit(SI7021_I2C);
        TM_I2C_Init(SI7021_I2C, SI7021_I2C_PINSPACK, SI7021_I2C_SPEED);
	/* Test if device is connected */
	if (!TM_I2C_IsDeviceConnected(SI7021_I2C, SI7021_I2C_ADDRESS)) {
		/* Device is not connected */
		return TM_SI7021_Result_DeviceNotConnected;
	}
	TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Transmitter,1);
	TM_I2C_WriteData(SI7021_I2C, SI7021_SOFT_RESET);
	Delayms(15);
	SI7021_Data->Delay=50;
	lib_initialized = 1;
	/* Return OK */
	return TM_SI7021_Result_Ok;
}


/* measurment request functions */
TM_SI7021_Result_t TM_SI7021_RequestTemperature(void) {

	/* Check for library initialization */
	if (!lib_initialized) {
		return TM_SI7021_Result_LibraryNotInitialized;
	}
	/* Send to device */
	TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Transmitter,1);
	TM_I2C_WriteData(SI7021_I2C, SI7021_TRIGGER_TEMP_MEASURE_NOHOLD);
	
	return TM_SI7021_Result_Ok;
}

TM_SI7021_Result_t TM_SI7021_RequestHumidity(void) {
	
	/* Check for library initialization */
	if (!lib_initialized) {
		return TM_SI7021_Result_LibraryNotInitialized;
	}
	/* Send to device */
	TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Transmitter,1);
	TM_I2C_WriteData(SI7021_I2C, SI7021_TRIGGER_HUMD_MEASURE_NOHOLD);

	return TM_SI7021_Result_Ok;
}


/* used to read data from last request, assumes that received data is a result of humidity measurment*/ 
/* must be called after measurment processing is finished, so can be invoked 50msec after  TM_SI7021_RequestHumidity or TM_SI7021_RequestTemperature*/ 
uint16_t TM_SI7021_readDatafromLastRequest(void) {
        uint8_t data[3];
        uint16_t helper;
         /* Check for library initialization */
	if (!lib_initialized) {
		return TM_SI7021_Result_LibraryNotInitialized;
	}
        TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Receiver,1);

        data[0]=TM_I2C_ReadAck(SI7021_I2C); //data
        data[1]=TM_I2C_ReadAck(SI7021_I2C); // data
        data[2]=TM_I2C_ReadNack(SI7021_I2C); // crc
        TM_I2C_Stop(SI7021_I2C);
        helper = ((unsigned)data[0] << 8) | (unsigned)data[1] ;
        helper &= 0xFFFC; 

        return helper;
}



/**
void TM_SI7021_Conv_2_Humidity(TM_SI7021_t * si7021, uint16_t rawValue) {
  &si7021->Humid = -6.0 + 125.0 * rawValue / 65536.0; 
}

void TM_SI7021_Conv_2_Temperature(TM_SI7021_t * si7021, uint16_t rawValue) {
  si7021->Temp = -46.85 + 175.72 * rawValue / 65536.0; 
}

**/







/* compact version of request_and_read functions */
TM_SI7021_Result_t TM_SI7021_requestAndReadTemperature2(TM_SI7021_t* SI7021_Data) {
  uint16_t tmp;
  if (!TM_SI7021_RequestTemperature()== TM_SI7021_Result_Ok) {
    printf ("Lib not initialized from Read temp 2\r\n");
    return TM_SI7021_Result_LibraryNotInitialized;
  }
    Delayms(50);
   tmp=TM_SI7021_readDatafromLastRequest();
   SI7021_Data->Temp= -46.85 + 175.72 * tmp / 65536.0; 

return TM_SI7021_Result_Ok;
}

TM_SI7021_Result_t TM_SI7021_requestAndReadTemperature(TM_SI7021_t* SI7021_Data) {
	uint8_t data[3];
	uint16_t rawValue;
	float calc;
	/* Check for library initialization */
	if (TM_SI7021_RequestTemperature()==TM_SI7021_Result_Ok) {
          Delayms(SI7021_Data->Delay);
          /* Read temperature from Si7021 */
          TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Receiver,0);
          data[0]=TM_I2C_ReadAck(SI7021_I2C);
          data[1]=TM_I2C_ReadAck(SI7021_I2C);
          data[2]=TM_I2C_ReadNack(SI7021_I2C);
          TM_I2C_Stop(SI7021_I2C);
          rawValue = ((unsigned)data[0] << 8) | (unsigned)data[1] ;
          rawValue &= 0xFFFC;
          calc = -46.85 + 175.72 * rawValue / 65536.0;
	
          /* Get temperature in degrees */
          SI7021_Data->Temp=calc;
          SI7021_Data->crc=data[2];
	}
        else return TM_SI7021_Result_LibraryNotInitialized;
	/* Return OK */
	return TM_SI7021_Result_Ok;
}





TM_SI7021_Result_t TM_SI7021_requestAndReadHumidity2(TM_SI7021_t* SI7021_Data) {
  uint16_t tmp;
  if (!TM_SI7021_RequestHumidity()== TM_SI7021_Result_Ok) {
    //printf ("Lib not initialized from Reat temp 2\r\n");
    return TM_SI7021_Result_LibraryNotInitialized;
  }
    Delayms(50);
  tmp=TM_SI7021_readDatafromLastRequest();
   SI7021_Data->Humid= -6.0 + 125.0 * tmp / 65536.0; 

return TM_SI7021_Result_Ok;
}

TM_SI7021_Result_t TM_SI7021_requestAndReadHumidity(TM_SI7021_t* SI7021_Data) {
	uint8_t data[3];
	uint16_t rawValue;
	float calc;
	/* Check for library initialization */
	if (TM_SI7021_RequestTemperature()==TM_SI7021_Result_Ok) {
          Delayms(SI7021_Data->Delay);
          /* Read humidity from Si7021 */
          TM_I2C_Start(SI7021_I2C,SI7021_I2C_ADDRESS,I2C_Direction_Receiver,0);
          data[0]=TM_I2C_ReadAck(SI7021_I2C);
          data[1]=TM_I2C_ReadAck(SI7021_I2C);
          data[2]=TM_I2C_ReadNack(SI7021_I2C);
          TM_I2C_Stop(SI7021_I2C);
          rawValue = ((unsigned)data[0] << 8) | (unsigned)data[1] ;
          rawValue &= 0xFFFC;
          calc = -6.0 + 125.0 * rawValue / 65536.0;
	
          /* Get temperature in degrees */
          SI7021_Data->Humid=calc;
          SI7021_Data->crc=data[2];
	}
        else return TM_SI7021_Result_LibraryNotInitialized;
	/* Return OK */
	return TM_SI7021_Result_Ok;
}

        
