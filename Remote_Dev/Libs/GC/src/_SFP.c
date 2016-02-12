#include "SFP.h"



// basic functions for SFP GPIO configuration and on/off operation
void SFP_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(SFP_POWER_RRC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = SFP_POWER;
  GPIO_Init(SFP_POWER_PORT, &GPIO_InitStructure);
  /* default states for pins:  SFP Power off 
  ( 0 - disable power for SFP module ) */
  GPIO_ResetBits(SFP_POWER_PORT, SFP_POWER);
}

int SFP_Control (char control) {
  if (control==1) {
    GPIO_SetBits(SFP_POWER_PORT, SFP_POWER);
    return 1;
  }
  else if (control==0) {
    GPIO_ResetBits(SFP_POWER_PORT, SFP_POWER);
    return 0;
  }
return -1;
}


bool SFP_Init() {

  I2C_DeInit(SFP_I2C_PORT);
  TM_I2C_Init(SFP_I2C_PORT, SFP_I2C_PINS, SFP_I2C_SPEED);
  if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, SFP_I2C_ADDRESS)) {
    return true;
  }
  return false;
  
}






// read single ddmi data ((for example temp, voltage, tx power, rx power)
// before issuing this command please be sure that correct sfp port was selected
uint16_t SFP_READ_DDMI_Single_16bit (SFP_READ_DDMI_Type_t type) {
	uint8_t buffer[2];
	TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
	return 	((buffer[0] << 8) | buffer[1]);
}


float SFP_READ_DDMI_Single_float (SFP_READ_DDMI_Type_t type) {
  uint8_t buffer[2];
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
  return SFP_DDMI_RAW2Float(type,((buffer[0] << 8) | buffer[1]));
}









void SFP_READ_DDMI_All_8bit (uint8_t* ddmi_buffer) {
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, temperature, ddmi_buffer, 10);
}

void SFP_READ_DDMI_All_16bit (uint16_t* ddmi_buffer) {
    uint8_t buffer[10];
	SFP_READ_DDMI_All_8bit(buffer);
	for (int i=0; i<10; i+=2)
	*ddmi_buffer++ = (buffer[i] << 8) | buffer[i+1];	
}


//void SFP_READ_DDMI_All_float (float* ddmi_buffer) {
//    uint8_t buffer[10];
//    SFP_READ_DDMI_All_8bit(buffer);
//    *ddmi_buffer++ = SFP_DDMI_RAW2Float(temperature,((buffer[0] << 8) | buffer[1]));
//    *ddmi_buffer++ = SFP_DDMI_RAW2Float(voltage,((buffer[2] << 8) | buffer[3]));
//    *ddmi_buffer++ = SFP_DDMI_RAW2Float(current,((buffer[4] << 8) | buffer[5]));
//    *ddmi_buffer++ = SFP_DDMI_RAW2Float(tx_dbm,((buffer[6] << 8) | buffer[7]));
//    *ddmi_buffer = SFP_DDMI_RAW2Float(rx_dbm,((buffer[8] << 8) | buffer[9]));
//}



void SFP_READ_DDMI_All_float (float* ddmi_buffer) {
    uint8_t buffer[10];
    long tmp_tx=0;
    long tmp_rx=0;
    for (int i=0;i<64;i++) {
      SFP_READ_DDMI_All_8bit(buffer);
      tmp_tx+= (buffer[6] << 8) | buffer[7];
      tmp_rx+= (buffer[8] << 8) | buffer[9];
    }
    tmp_tx/=64;
    tmp_rx/=64;
    *ddmi_buffer++ = SFP_DDMI_RAW2Float(temperature,((buffer[0] << 8) | buffer[1]));
    *ddmi_buffer++ = SFP_DDMI_RAW2Float(voltage,((buffer[2] << 8) | buffer[3]));
    *ddmi_buffer++ = SFP_DDMI_RAW2Float(current,((buffer[4] << 8) | buffer[5]));
    *ddmi_buffer++ = SFP_DDMI_RAW2Float(tx_dbm,tmp_tx);
    *ddmi_buffer = SFP_DDMI_RAW2Float(rx_dbm,tmp_rx);
}

void SFP_READ_DDMI (SFP_Data* ddmi) {
  if(SFP_Init()==0){ 
    ddmi->connected = 0;
    ddmi->temp = -500.0;
    ddmi->current = -500.0;
    ddmi->rx_power = -500.0;
    ddmi->rx_power_dbm = -500.0;
    ddmi->tx_power = -500.0;
    ddmi->tx_power_dbm = -500.0;
    ddmi->voltage = -500.0;
  }
  else {
    ddmi->connected = 1;
    ddmi->temp = SFP_READ_DDMI_Single_float(temperature);
    ddmi->voltage = SFP_READ_DDMI_Single_float(voltage);
    ddmi->current = SFP_READ_DDMI_Single_float(current);
    ddmi->tx_power = SFP_READ_DDMI_Single_float(tx_mw);
    ddmi->rx_power = SFP_READ_DDMI_Single_float(rx_mw);
  }
}



float SFP_DDMI_RAW2Float (SFP_READ_DDMI_Type_t data_type, uint16_t raw_value) {
	float value =  -50000.0;
	switch (data_type) {
		case temperature: {
			if (raw_value > 32767)
				raw_value -= 65536;
			value=raw_value/256.000;
			break;
			
		}
		case voltage: {
			value = raw_value/10000.000;
			break;
		}
		case current: {
			value = raw_value/500.000;
			break;
		}
		case tx_mw: {
			value = raw_value/10000.000;
			break;
		}
		case rx_mw: {
			value = raw_value/10000.000;
			break;
		}		
		case tx_dbm: {
			value = 10 * (log10(raw_value/10000.000));
			break;
		}
		case rx_dbm: {
			value = 10*(log10(raw_value/10000.000));
			break;
		}		
	}
return value;
}
