#include "utilities_new.h"





/* GPIO configuration functions for particular devices
argument: [void]
returned value: [void]
TODO: none
*/

void GC_GPS_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(GPS_POWER_RRC | GPS_RESET_RRC , ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Pin = GPS_POWER;
  GPIO_Init(GPS_POWER_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPS_RESET;
  GPIO_Init(GPS_RESET_PORT, &GPIO_InitStructure);
  
  /* default states for pins: Power off, !Reset desserted*/
  GPIO_ResetBits(GPS_POWER_PORT, GPS_POWER);
  GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
}


void SFP_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  /* default states for pins:  SFP Power off
  ( 0 - disable power for SFP module ) */
  GPIO_SetBits(GPIOE,GPIO_Pin_1 );
}


void GSM_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(GSM_POWER_RRC | GSM_RESET_RRC | GSM_WAKEUP_RRC | GSM_ON_RRC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Pin = GSM_POWER;
  GPIO_Init(GSM_POWER_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GSM_RESET;
  GPIO_Init(GSM_RESET_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GSM_WAKEUP;
  GPIO_Init(GSM_WAKEUP_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GSM_ON;
  GPIO_Init(GSM_ON_PORT, &GPIO_InitStructure);
  
  /* default states for pins: GPS Power off ( 0 - disable power for GSM module ) */
  GPIO_SetBits(GSM_POWER_PORT, GSM_POWER);
  
}

void PHY_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(PHY_RRC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Pin = PHY_POWER | PHY_CLOCK;
  GPIO_Init(PHY_PORT, &GPIO_InitStructure);
  /* default states for pins:  PHY and Clock Power off
 ( 0 - disable power for PHY / shutdown 50MHz clock ) */
  GPIO_ResetBits(PHY_PORT, PHY_POWER | PHY_CLOCK);
}


void GC_ZSPM_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(ZSPM_POWER_RRC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Pin = ZSPM_POWER;
  GPIO_Init(ZSPM_POWER_PORT, &GPIO_InitStructure);
  /* default states for pins:  Enable ZSPM */
  GPIO_SetBits(ZSPM_POWER_PORT, ZSPM_POWER);
}



/* Device control funtions:
arguments[char]: GC_ON (1), GC_OFF (0), GC_RESET (2) [Reset possible for GPS and GSM modules]
returns [int]: value of argument used
TODO: check device state and return value according to current state of device
*/

int GPS_Control (char control) {
  if (control==1) {
    GPIO_SetBits(GPS_POWER_PORT, GPS_POWER);
    GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
    return 1;
  }
  else if (control==0) {
    GPIO_ResetBits(GPS_POWER_PORT, GPS_POWER);
    GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
    return 0;
  }
    else if (control==2) {
    GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
    Delayms(50);
    GPIO_ResetBits(GPS_RESET_PORT, GPS_RESET);
    return 2;
  }
return -1;
}
    





/* returns value of main battery voltage
argument is of [void] type, returned value if of [float] type */
float GC_BatVoltage (void)  {
  float voltage=0;
  int adc_read=0;
  float reference=3.3/4096.0;
  TM_ADC_Init(ADC1, ADC_Channel_4);
  Delay(470);
  for (int i=0;i<5;i++) {
	  adc_read=TM_ADC_Read(ADC1, ADC_Channel_4);
	  voltage += 2.0516*adc_read*reference;
  }
  return voltage/5;
}


/* function check for GPS position with 60sec timeout,
argument is a pointer to TM_GPS_Data_t structure, 
returned vaule is of TM_GPS_Result_t type */
TM_GPS_Result_t GC_CheckPosition(TM_GPS_Data_t * GPS_Data, uint32_t timeout) {
   uint32_t time;
   GC_GPS_GPIO_Config();
   TM_USART_Init(GPS_USART, GPS_USART_PINSPACK, GPS_USART_SPEED);
   TM_GPS_Result_t result;
   dev_control(dev_gps,dev_on);
   time = TM_DELAY_Time();
   GPS_Data->Fix = 0;
   GPS_Data->Latitude = 0.0;
   GPS_Data->Longitude = 0.0;
   TM_GPS_Init(GPS_Data, 9600);
      do {
        result = TM_GPS_Update(GPS_Data);
        if (GPS_Data->Fix==1) break;
      } while (TM_DELAY_Time() < time + timeout);
  if(GPS_Data->Fix == 0) printf("Couldn't find satellites, unknown position!\n");
  dev_control(dev_gps,dev_off);
  USART_DeInit(GPS_USART);
  return result;
}





uint8_t isLaserOn (void) {
  TM_I2C_Init(ZSPM_I2C, ZSPM_I2C_PINSPACK, ZSPM_I2C_SPEED); 
  if (!TM_I2C_IsDeviceConnected(ZSPM_I2C, ZSPM_I2C_ADDRESS)) {                  // ZSPM not detected - it is either no power in the battery or there is no sufficient power from laser receiver, assuming laser fiber problem,        
    I2C_DeInit(ZSPM_I2C);
    return 0;
    #ifdef SERIAL_DEBUG
      TM_USART_Puts(MENU_USART,"ZSPM4521 not detected! There is communication problem or laser beam receiver doesn't get power!\r\n");
    #endif
  }
  else {                                                                        // ZSPM is detected, Battery and Laser Input are OK.
    I2C_DeInit(ZSPM_I2C);
    return 1;
  }
}













  



short parseIP (char* ip_str, uint8_t *ip_array, uint8_t size) {
    int i = 0;
    char* buff = malloc(size);
    buff = strtok(ip_str,".:");
    while (buff != NULL)
    {
       //if you want to print its value
       //printf("%s\n",buff);
       *ip_array++ = (uint8_t)atoi(buff);
       buff = strtok(NULL,".:");
       i++;
    }
    free(buff);
    return 0;
}





















uint8_t loadIP(IPAddress* address, short position) {
#ifdef RAM_CONFIG  
  address->ip1 = TM_BKPSRAM_Read8(position++);
  address->ip2 = TM_BKPSRAM_Read8(position++);
  address->ip3 = TM_BKPSRAM_Read8(position++);
  address->ip4 = TM_BKPSRAM_Read8(position++);
  address->port = TM_BKPSRAM_Read8(position);
#endif   
#ifdef FLASH_CONFIG
  u16_bytes tmp;
  tmp.uint16 = UB_EE_FLASH_Read(position++);
  address->ip1 = tmp.bytes[0];
  address->ip2 = tmp.bytes[1];
  tmp.uint16 = UB_EE_FLASH_Read(position++);
  address->ip3 = tmp.bytes[0];
  address->ip4 = tmp.bytes[1];
  tmp.uint16 = UB_EE_FLASH_Read(position++);
  address->port = tmp.bytes[0];
#endif 
return 0; 
}


//////////////// IP ADDRESS FUNCTIONS //////////////////////////////


/* Reads IP addresses from BKP SRAM  to IP structures*/
void loadIPaddress(IPAddress* local, IPAddress* mask, IPAddress* gw, IPAddress* serv) {
  loadIP(local,GC_LOCAL_IP_LOCATION);
  loadIP(serv,GC_REMOTE_IP_LOCATION);
  loadIP(mask,GC_NETMASK_LOCATION);
  loadIP(gw,GC_GATEWAY_LOCATION); 
}










void bkpram(uint8_t operation, uint8_t *data, uint8_t start, uint8_t size) {
  TM_BKPSRAM_Init();
  uint8_t tmp=0;
  switch (operation) {
    case GC_LOAD: // load
      {
        for (int i = start; i < start + size; i++)
          *data++=TM_BKPSRAM_Read8(i);
        break;
      }
    case GC_SAVE: //save
      {
        for (int i = start; i < start + size; i++)
          TM_BKPSRAM_Write8(i,*data++);
        break;
      }
    case GC_LOAD_AND_PRINT: //load and display
      {
        printf("RAW config data:\n\r");
        for (int i = start; i < start + size; i++) {
          tmp = TM_BKPSRAM_Read8(i);
          *data++ = tmp;
          printf("Addr: 0x%03x(hex)|%04d(dec) = 0x%02x(hex) | %03d(dec) | %c(char)\n\r",i,i,tmp,tmp,tmp);
        }
        break;
      }      
  }
  
}


void config (uint8_t operation, uint8_t *data, uint8_t type) {
  switch (type) {
    case GC_ID: 
      {
        bkpram(operation, data, GC_ID_LOCATION, GC_ID_LENGTH);
        break;
      }  
    
    case GC_MAC: 
      {
        //bkpram(operation, data, GC_MAC_LOCATION, GC_MAC_LENGTH);
        for (int i=0;i<6;i++){
        	*data++ = TM_ID_GetUnique8(i);

        }
        break;
      }
    case GC_LOCAL_IP: 
      {
        bkpram(operation, data, GC_LOCAL_IP_LOCATION, GC_LOCAL_IP_LENGTH);
        break;
      }   
    case GC_LOCAL_PORT: 
      {
        bkpram(operation, data, GC_LOCAL_PORT_LOCATION, GC_LOCAL_PORT_LENGTH);
        break;
      }
    case GC_REMOTE_IP: 
      {
        bkpram(operation, data, GC_REMOTE_IP_LOCATION, GC_REMOTE_IP_LENGTH);
        break;
      }
    case GC_REMOTE_PORT: 
      {
        bkpram(operation, data, GC_REMOTE_PORT_LOCATION, GC_REMOTE_PORT_LENGTH);
        break;
      }
    case GC_NETMASK: 
      {
        bkpram(operation, data, GC_NETMASK_LOCATION, GC_NETMASK_LENGTH);
        break;
      } 
    case GC_GATEWAY: 
      {
        bkpram(operation, data, GC_GATEWAY_LOCATION, GC_GATEWAY_LENGTH);
        break;
      }
    case GC_LATITUDE: 
      {
        bkpram(operation, data, GC_LATITUDE_LOCATION, GC_LATITUDE_LENGTH);
        break;
      }     
    case GC_LONGITUDE: 
      {
        bkpram(operation, data, GC_LONGITUDE_LOCATION, GC_LONGITUDE_LENGTH);
        break;
      } 	
    case GC_PHONE: 
      {
        bkpram(operation, data, GC_PHONE_LOCATION, GC_PHONE_LENGTH);
        break;
      }
    case GC_CALIB_TX: 
      {
        bkpram(operation, data, GC_CALIBRATION_TX, GC_CALIBRATION_LENGTH);
        break;
      }
    case GC_CALIB_RX: 
      {
        bkpram(operation, data, GC_CALIBRATION_RX, GC_CALIBRATION_LENGTH);
        break;
      }	
  }
}


void conf_load (config_t * conf) {
  config(GC_LOAD,conf->id,GC_ID);
  config(GC_LOAD,conf->mac, GC_MAC);
  config(GC_LOAD,conf->local_ip,GC_LOCAL_IP);
  config(GC_LOAD,conf->remote_ip,GC_REMOTE_IP);
  config(GC_LOAD,conf->mask,GC_NETMASK);
  config(GC_LOAD,conf->gateway,GC_GATEWAY);
  config(GC_LOAD,conf->local_port,GC_LOCAL_PORT);
  config(GC_LOAD,conf->remote_port,GC_REMOTE_PORT);
  config(GC_LOAD,conf->position_lat.fInt,GC_LATITUDE);
  config(GC_LOAD,conf->position_long.fInt,GC_LONGITUDE);
  config(GC_LOAD,conf->phone,GC_PHONE);
  config(GC_LOAD,conf->tx_calibration.fInt,GC_CALIBRATION_TX);
  config(GC_LOAD,conf->rx_calibration.fInt,GC_CALIBRATION_RX);
}


void conf_save (config_t conf) {
  config(GC_SAVE,conf.id,GC_ID);
  config(GC_SAVE,conf.mac, GC_MAC);
  config(GC_SAVE,conf.local_ip,GC_LOCAL_IP);
  config(GC_SAVE,conf.remote_ip,GC_REMOTE_IP);
  config(GC_SAVE,conf.mask,GC_NETMASK);
  config(GC_SAVE,conf.gateway,GC_GATEWAY);
  config(GC_SAVE,conf.local_port,GC_LOCAL_PORT);
  config(GC_SAVE,conf.remote_port,GC_REMOTE_PORT);
  config(GC_SAVE,conf.position_lat.fInt,GC_LATITUDE);
  config(GC_SAVE,conf.position_long.fInt,GC_LONGITUDE);
  config(GC_SAVE,conf.phone,GC_PHONE);
  config(GC_SAVE,conf.tx_calibration.fInt,GC_CALIBRATION_TX);
  config(GC_SAVE,conf.rx_calibration.fInt,GC_CALIBRATION_RX);
}

int load_defaults(config_t* conf) {
  uint8_t buffer[47];
  if (TM_I2C_IsDeviceConnected(I2C3, 0xA0)) {
    TM_I2C_ReadMulti(I2C3, 0xA0, 96, buffer, 47);
    conf->id[0]=buffer[0];
    for (int i=0;i<4;i++) conf->local_ip[i]=buffer[i+1];
    conf->local_port[0]=buffer[5];
    for (int i=0;i<4;i++) conf->remote_ip[i]=buffer[i+6];
    conf->remote_port[0]=buffer[10];
    for (int i=0;i<4;i++) conf->mask[i]=buffer[i+11];
    for (int i=0;i<4;i++) conf->gateway[i]=buffer[i+15];
    for (int i=0;i<4;i++) conf->position_lat.fInt[i]=buffer[i+19];
    for (int i=0;i<4;i++) conf->position_long.fInt[i]=buffer[i+23];
    for (int i=0;i<13;i++) conf->phone[i]=buffer[i+27];
    for (int i=0;i<4;i++) conf->tx_calibration.fInt[i]=buffer[i+40];
    for (int i=0;i<4;i++) conf->rx_calibration.fInt[i]=buffer[i+44];
    return 0;
  }
  else return -1;
}



int save_defaults(config_t conf) {
  uint8_t buffer[47];
  if (TM_I2C_IsDeviceConnected(I2C3, 0xA0)) {
    buffer[0] = conf.id[0];
    for (int i=0;i<4;i++) buffer[i+1] = conf.local_ip[i];
    buffer[5] = conf.local_port[0];
    for (int i=0;i<4;i++) buffer[i+6] = conf.remote_ip[i];
    buffer[10] = conf.remote_port[0];
    for (int i=0;i<4;i++) buffer[i+11] = conf.mask[i];
    for (int i=0;i<4;i++) buffer[i+15] = conf.gateway[i];
    for (int i=0;i<4;i++) buffer[i+19] = conf.position_lat.fInt[i];
    for (int i=0;i<4;i++) buffer[i+23] = conf.position_long.fInt[i];
    for (int i=0;i<13;i++) buffer[i+27] = conf.phone[i];
    for (int i=0;i<4;i++) buffer[i+40] = conf.tx_calibration.fInt[i];
    for (int i=0;i<4;i++) buffer[i+44] = conf.rx_calibration.fInt[i];
    
    TM_I2C_WriteMulti(I2C3, 0xA0, 96, buffer , 47);
  return 0;
  }
  
  else return -1; 
}





void dev_control (uint8_t device, uint8_t state) {
switch (device) {
  case dev_sfp: {
    switch (state) {
      case dev_on: {
        GPIO_SetBits(SFP_POWER_PORT, SFP_POWER);
        break;
      }
      case dev_off: {
        GPIO_ResetBits(SFP_POWER_PORT, SFP_POWER);
        break;
      }
      case dev_reset: break;
      case dev_emergency_shutdown: break;
    }
    break;
  } 

case dev_gsm: {
      switch (state) {
        case dev_on: {
          GPIO_ResetBits(GSM_POWER_PORT, GSM_POWER);
	      GPIO_ResetBits(GSM_ON_PORT, GSM_ON);
          Delayms(220);
          GPIO_SetBits(GSM_ON_PORT, GSM_ON);
	  break;
        }
        case dev_off: {
          GPIO_ResetBits(GSM_ON_PORT, GSM_ON);
          Delayms(3200);
          GPIO_SetBits(GSM_ON_PORT, GSM_ON);
          GPIO_SetBits(GSM_POWER_PORT, GSM_POWER);
          break;
	}
	case dev_reset: {
          GPIO_SetBits(GSM_RESET_PORT, GSM_RESET);
          Delayms(50);
          GPIO_ResetBits(GSM_RESET_PORT, GSM_RESET);
          break;
	}
	case dev_emergency_shutdown: {
          GPIO_SetBits(GSM_POWER_PORT, GSM_POWER);
          break;
	}
	}	
      break;
}


case dev_phy: {
	switch (state) {
        case dev_on: {
          GPIO_SetBits(PHY_PORT, PHY_POWER | PHY_CLOCK);
          break;
        }
	case dev_off: {
          GPIO_ResetBits(PHY_PORT, PHY_POWER | PHY_CLOCK);
          break;
        }
        case dev_reset: break;
	case dev_emergency_shutdown: {
          GPIO_ResetBits(PHY_PORT, PHY_POWER | PHY_CLOCK);
          break;
        }
      }
    break;
}

case dev_zspm: {
      switch (state) {
        case dev_on: {
          GPIO_SetBits(ZSPM_POWER_PORT, ZSPM_POWER);
          break;
       }
        case dev_off: {
          GPIO_ResetBits(ZSPM_POWER_PORT, ZSPM_POWER);
          break;
       }
        case dev_reset: break;
        case dev_emergency_shutdown: {
          GPIO_ResetBits(ZSPM_POWER_PORT, ZSPM_POWER);
          break;
        }
     }
    break;
}

case dev_gps: {
      switch (state) {
        case dev_on: {
          GPIO_SetBits(GPS_POWER_PORT, GPS_POWER);
          GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
          break;
        }
        case dev_off: {
          GPIO_ResetBits(GPS_POWER_PORT, GPS_POWER);
          GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
          break;
        }
        case dev_reset: {
          GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
          Delayms(50);
          GPIO_ResetBits(GPS_RESET_PORT, GPS_RESET);
          break;
        }
        case dev_emergency_shutdown: {
          GPIO_ResetBits(GPS_POWER_PORT, GPS_POWER);
          GPIO_SetBits(GPS_RESET_PORT, GPS_RESET);
          break;
        }
      }
    break;
   }
   
}

}
