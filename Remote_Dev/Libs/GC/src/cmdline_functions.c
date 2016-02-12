#include "cmdline_functions.h"



char buffer[128];

float currentLSB, powerLSB;
float vShuntMax, vBusMax, rShunt;




void Menu_Output(uint8_t c)
{
	TM_USART_Putc(MENU_USART,c);
}


void Config_Menu (void)
{
	char cmd;
	Menu_Init();
	printf("\n\rPress CTRL+C if you want to exit menu and continue running program\n\r");
	cmdlinePrintPrompt();
	while (1)
	{
		cmd=TM_USART_Getc(MENU_USART);
		if(cmd==0x03)
		{                                                             // check for CTRL+C keypress send from terminal emulator
			printf("\n\rCTRL+C was detected, leaving menu\n\r");
			break;
		}
    cmdlineInputFunc(cmd);
    Delay(100);
    cmdlineMainLoop();
	}
}


void help_descr (void)
{
	printf("help\t\t\t- display this help\n");
	printf("set-time\t\t- sets RTC time, eg. [set-time 13 15 45] sets time to 13h:15m:45s\n");
	printf("set-date\t\t- sets RTC date, eg. [set-date 25 12 15] sets date to 25th Dec 2015\n");
	printf("set-id\t\t\t- sets device ID, eg. [set-id 9] sets device ID to 9\n");
	printf("set-mac\t\t\t- sets device MAC address, eg. [set-mac 10 12 00 20 F7 A0 BC] sets MAC to 10:12:00:20:F7:A0:BC\n");
	printf("set-localip\t\t- sets device IP address and UDP port, eg. [set-localip 192.168.0.109:7] sets device IP to 192.168.0.109 and UDP port to 7\n");
	printf("set-serverip\t\t- sets destination IP address and UDP port, eg. [set-serverip 192.168.0.2:7] sets device IP to 192.168.0.2 and UDP port to 7\n");
	printf("set-mask\t\t- sets network mask, eg. [set-mask 255.255.255.0] sets network mask to 255.255.255.0\n");
	printf("set-phone\t\t- sets phone number to which SMS will be sent , eg. [set-phone +48123456789], + and country code is mandatory\n");
	printf("set-gps\t\t\t- sets device location according to GPS readings, eg. set-gsp\n");
	printf("test-gsm\t\t- conducts GSM module test by sending sampple message to phone defined in system, eg. test-gsm\n");
	printf("test-ethernet\t\t- conducts Ethernet module test, eg. set-ethernet\n");
	printf("interlock\t\t- toggle Interlock checking, eg. interlock\n");
	printf("system\t\t\t- display information about system, eg. system\n");
	printf("bootloader\t\t- switch MCU to bootloader, now you can download new firmware using GPS USART port, eg. go-bootloader\n");
}



void Menu_Init(void)
{
	cmdlineSetOutputFunc(Menu_Output);
	cmdlineInit();
	cmdlineAddCommand("help",help_descr);
	cmdlineAddCommand("set-time",setTime);
	cmdlineAddCommand("set-date",setDate);
	cmdlineAddCommand("set-id",set_ID);
	cmdlineAddCommand("set-localip", set_LocalIP);
	cmdlineAddCommand("set-serverip", set_RemoteIP);
	cmdlineAddCommand("set-mask", set_MaskIP);
	cmdlineAddCommand("set-phone",set_Phone);
	cmdlineAddCommand("set-gps",set_GPS);
	cmdlineAddCommand("test-gsm",gsm);
	cmdlineAddCommand("test-ethernet", eth_test);
	cmdlineAddCommand("interlock", InterlockToggle);
	cmdlineAddCommand("system", show_sys);
	cmdlineAddCommand("bootloader", bootloader);
	cmdlineAddCommand("ina", show_ina);
}



void show_ddmi (void)
{
	SFP_Device sfp;
	dev_control (dev_sfp, dev_on);
	Delayms(50);
	if(SFP_DOM_Reading_Update(&sfp) == SFP_DOM_Read_OK)
	{
		printf("SFP TX power:         %2.3fdBm\n",mw_2_dbm(SFP_DOM_Conv_float(dom_tx,sfp,false)));
		printf("SFP RX power:         %2.3fdBm\n",mw_2_dbm(SFP_DOM_Conv_float(dom_rx,sfp,false)));
	}
	dev_control (dev_sfp, dev_off);
}



void show_temp_humidity (void)
{
	si7021_env test;
	if(SI7021_begin())
	{
		dev_control (dev_sfp, dev_on);			// SFP module power must also be enabled, it's a hardware bug
		SI7021_getHumidityAndTemperature(&test);
		printf("Temperature:          %02.2f\n\r",test.celsiusHundredths/100.00);
		printf("Humidity:             %02.2f\n\r",test.humidityBasisPoints/100.00);
		dev_control (dev_sfp, dev_off);				// SFP module power can be disabled after making I2C reading from other devices
	}
  else printf("Si7021 sensor not detected!\n\r");
}


void show_sys (void)
{
	config_t conf;
	TM_RTC_Time_t datatime;
	conf_load(&conf);
	conf.phone[GC_PHONE_LENGTH]=0;
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	printf("\n\r------------------System data------------------\n\r");
	printf("ID:                   %d\n\r", conf.id[0]);
	printf("Local IP:             %d.%d.%d.%d, port: %d\n\r", conf.local_ip[0], conf.local_ip[1], conf.local_ip[2], conf.local_ip[3], conf.local_port[0]);
	printf("Remote IP:            %d.%d.%d.%d, port: %d\n\r", conf.remote_ip[0], conf.remote_ip[1], conf.remote_ip[2], conf.remote_ip[3], conf.remote_port[0]);
	printf("Current time:         %02d:%02d:%02d\n\r", datatime.hours, datatime.minutes, datatime.seconds);
	printf("Current date:         %02d.%02d.%02dr.\n\r", datatime.date, datatime.month, datatime.year);
	printf("Central phone number: %s\n\r",conf.phone);
	printf("Battery voltage:      %2.3fV\n\r",GC_BatVoltage());
	printf("Latitude:             %2.6f\n\r",conf.position_lat.f);
	printf("Longitude:            %2.6f\n\r",conf.position_long.f);
	show_ddmi();
	show_temp_humidity();
}

void gsm(void)
{
	char msg[20];
	char number[13];
	number[12]=0;
	config(GC_LOAD,(uint8_t *)number,GC_PHONE);
	sprintf(msg,"Off%d",TM_BKPSRAM_Read8(0));

	GSM_Init();
	for (int i=0;i<10;i++) {
		GSM_checkNetwork_Simple(10000);
		if (GSM_isRegistered()) break;
	}
   printf("Successfully registered into GMS network\n");
   if(GSM_isRegistered())
	   GSM_sendSMS(number,msg);
   dev_control(dev_gsm,dev_off);
   TM_BKPSRAM_Write8(1000,2);
}



void eth_test(void)
{
	ethernetSendString("Test message\n");
}
 


void set_ID(void)
{
	uint8_t id[]= {cmdlineGetArgInt(1)};
	config(GC_SAVE,id,GC_ID);
}






void set_LocalIP(void)
{
	uint8_t ipaddr_port[5];
	parseIP(cmdlineGetArgStr(1),ipaddr_port,5);
	config(GC_SAVE,ipaddr_port,GC_LOCAL_IP);
	config(GC_SAVE,&ipaddr_port[4],GC_LOCAL_PORT);
}

void set_RemoteIP(void)
{
	uint8_t ipaddr_port[5];
	parseIP(cmdlineGetArgStr(1),ipaddr_port,5);
	config(GC_SAVE,ipaddr_port,GC_REMOTE_IP);
	config(GC_SAVE,&ipaddr_port[4],GC_REMOTE_PORT);
}

void set_MaskIP(void)
{
	uint8_t ipaddr_port[4];
	parseIP(cmdlineGetArgStr(1),ipaddr_port,4);
	config(GC_SAVE,ipaddr_port,GC_NETMASK);
}



void set_Phone(void)
{
	char number[GC_PHONE_LENGTH+1]; // 12 for number with + and country code, one for ending \0
	number[GC_PHONE_LENGTH]=0;
	int i;
	int valid_number=0;
	strncpy(number,cmdlineGetArgStr(1),GC_PHONE_LENGTH);
	for (i=1;i<GC_PHONE_LENGTH;i++)
	{
		if (number[i]>0x29 && number[i]<0x40) {
			valid_number=1;
    }
    else valid_number=0;
	}
	if (valid_number)
	{
		config(GC_SAVE,(uint8_t*)number,GC_PHONE);
	}
  else printf("invalid phone number\n\r");
}

void set_GPS(void)
{
	TM_GPS_Data_t position;
  	FloatOrUInt tmp;
  	uint32_t timeout = 60000;
  	sprintf(buffer,"Trying to update GPS position with %ld seconds timeout\n\r",timeout/1000);
  	TM_USART_Puts(MENU_USART, buffer);
  	GC_CheckPosition(&position, timeout);
  	sprintf(buffer,"GPS Position:\n\r             Longitude: %3.6f\n\r             Latitude:  %3.6f\n\r",
  			position.Longitude,
  			position.Latitude);
  	TM_USART_Puts(MENU_USART,buffer);
  
  	tmp.f=position.Latitude;
  	config(GC_SAVE,tmp.fInt,GC_LATITUDE);
  	tmp.f=position.Longitude;
  	config(GC_SAVE,tmp.fInt,GC_LONGITUDE);
}


void getTime(void)
{
	TM_RTC_Time_t datatime;
  	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
  	sprintf(buffer,"Current time: %02d:%02d:%02d, %02d.%02d.%02dr.\n\r",
  			datatime.hours, datatime.minutes, datatime.seconds,
  			datatime.date, datatime.month, datatime.year);
  	TM_USART_Puts(MENU_USART,buffer);
}


void setTime(void)
{
	TM_RTC_Time_t time;
	int tmp=0;
	TM_RTC_GetDateTime(&time, TM_RTC_Format_BIN);
	tmp = cmdlineGetArgInt(1);
	if (tmp > 0 && tmp < 25)
		time.hours=tmp;
	tmp = cmdlineGetArgInt(2);
	if (tmp > 0 && tmp < 61)
		time.minutes=tmp;
	tmp = cmdlineGetArgInt(3);
	if (tmp > 0 && tmp < 61)
		time.seconds=tmp;
	TM_RTC_SetDateTime(&time, TM_RTC_Format_BIN);
	TM_USART_Puts(MENU_USART,"Time has been sucessfully set!\n\r");
}


void setDate(void)
{
	TM_RTC_Time_t time;
	int tmp=0;
	TM_RTC_GetDateTime(&time, TM_RTC_Format_BIN);
	tmp = cmdlineGetArgInt(1);
	if (tmp > 0 && tmp < 32)
		time.date=tmp;
	tmp = cmdlineGetArgInt(2);
	if (tmp > 0 && tmp < 13)
		time.month=tmp;
	tmp = cmdlineGetArgInt(3);
	if (tmp >= 0 && tmp <= 99)
		time.year=tmp;
	TM_RTC_SetDateTime(&time, TM_RTC_Format_BIN);
	TM_USART_Puts(MENU_USART,"Date has been sucessfully set!\n\r");
}


void InterlockToggle ()
{
	uint8_t tmp = TM_BKPSRAM_Read8(0xF00);
	if (tmp==0xEB)
	{
		TM_BKPSRAM_Write8(0xF00,0x00);
		TM_BKPSRAM_Write8(1000,0xFA);
		printf("Interlock is now DISABLED\n\r");
	}
	else
	{
		TM_BKPSRAM_Write8(0xF00,0xEB);
		TM_BKPSRAM_Write8(1000,0x02);
		printf("Interlock in now ENABLED\n\r");
	}
}


void show_ina(void)
{
	dev_control(dev_sfp,dev_off);
	float electrical_power = 0.0;
	if(INA219_begin())
	{
		INA219_configure(INA219_RANGE_16V, INA219_GAIN_40MV, INA219_BUS_RES_12BIT,
                     INA219_SHUNT_RES_12BIT_32S, INA219_MODE_SHUNT_BUS_CONT);
		INA219_calibrate(0.05, 0.4);
		electrical_power = 1000 *INA219_readBusPower();
		printf("Power consumption:   %2.3f mW\n", electrical_power);
		printf("Battery voltage:   %2.3f V\n", INA219_readBusVoltage());
		printf("Current consumption:   %2.3f A\n", INA219_readShuntCurrent());
	}
	else
	{
	  printf("Current monitor not detected!\n\r");
	}
}


void goSleep(void)
{
	TM_RTC_Interrupts(TM_RTC_Int_Disable);
	TM_USART_Puts(MENU_USART,"Going to sleep state now!\n\r");
  	USART_DeInit(MENU_USART);
  	USART_DeInit(GSM_USART);
  	USART_DeInit(GPS_USART);
  	I2C_DeInit(I2C3);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, DISABLE);
  	TM_RTC_Interrupts(TM_RTC_Int_1s); // 500ms;1,2,5,10,15,30,60 seconds to choose from
  	TM_LOWPOWER_Standby();
}





void (*SysBootLoader) (void);

void bootloader (void)
{
	SysBootLoader = (void (*)(void)) (*((u32 *) 0x1fff0004));
	RCC_DeInit();
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	__set_PRIMASK(1);
	__set_MSP(0x20001000);
	SysBootLoader();
}

