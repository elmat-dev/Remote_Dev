#include "udp_echoclient.h"
#include "cmdline_functions.h"




u8_t   	data[PKT_SIZE];


void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);






float input_power(void)
{
	float optical_power = 0.0;
	float electrical_power = 0.0;
	if(INA219_begin())
	{
		INA219_configure(INA219_RANGE_16V, INA219_GAIN_40MV, INA219_BUS_RES_12BIT,
                    INA219_SHUNT_RES_12BIT_32S, INA219_MODE_SHUNT_BUS_CONT);
		INA219_calibrate(0.05, 0.3);
		electrical_power = 1000 *INA219_readBusPower();
		optical_power = (0.0078*pow(electrical_power,2.0)) + (0.4717*electrical_power) - 62.196;
		return optical_power;
	}
	else
	{
		printf("Current monitor not detected!\n\r");
		return -500.0;
	}
}







void udp_echoserver_init(void)
{
	struct udp_pcb *upcb;
	err_t err;
	/* Create a new UDP control block  */
	upcb = udp_new();

	if (upcb)
	{
		err = udp_bind(upcb, IP_ADDR_ANY, 7);
		if (err == ERR_OK)
		{
			/* Set a receive callback for the upcb */
			udp_recv(upcb, udp_echoserver_receive_callback, NULL);
		}
		else
		{
			printf("can not bind pcb");
		}
	}
	else
	{
		printf("can not create pcb");
	}
}



void udp_echoserver_initWithPort(short port)
{
	struct udp_pcb *upcb;
	err_t err;
	/* Create a new UDP control block  */
	upcb = udp_new();

	if (upcb)
	{
		err = udp_bind(upcb, IP_ADDR_ANY, port);
		if (err == ERR_OK)
		{
			/* Set a receive callback for the upcb */
			udp_recv(upcb, udp_echoserver_receive_callback, NULL);
		}
		else
		{
			printf("can not bind pcb");
		}
	}
	else
	{
		printf("can not create pcb");
	}
}


void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	char test[200];
	memset(test,0,200);
	sprintf(test,p->payload);
	pbuf_free(p);
}








void sendToServer(char* data, uint16_t data_length, IPAddress serverIP)
{
	struct ip_addr DstIPaddr;
	struct udp_pcb *upcbb;
	struct pbuf *pp;
	upcbb = udp_new();
	if (upcbb!=NULL)
	{
		err_t err;
		IP4_ADDR(&DstIPaddr, serverIP.ip1 , serverIP.ip2, serverIP.ip3, serverIP.ip4);
		err = udp_connect(upcbb, &DstIPaddr, serverIP.port);
		if (err == ERR_OK)
    {
      for (int i=0;i<8;i++)
      {
        	#ifdef SERIAL_DEBUG
          	  printf("\r\nETHERNET: Successfully connected with server\r\n");
        	#endif
          	  pp = pbuf_alloc(PBUF_TRANSPORT, PKT_SIZE, PBUF_POOL);
        
          	  if (ETH_CheckFrameReceived())
          	  {
          		  LwIP_Pkt_Handle();
          	  }
          	  if (pp != NULL)
          	  {
          	  	  #ifdef SERIAL_DEBUG
          		  	  printf("\r\nETHERNET: Memory for packet buffer allocated successfully\r\n");
          	  	  #endif
          		  	  pbuf_take(pp, (char*)data, data_length);
          		  	  udp_send(upcbb, pp);
          		  	  pbuf_free(pp);
          	  }
          	  else
          		  {
					#ifdef SERIAL_DEBUG
          		  	  printf("\n\rETHERNET: Can't allocate memory for packet buffer\n\r");
           	  	  #endif
          		  }
          	  Delayms(15); //add delay if you need to send greater number of packets
      	  }
    	}
    else
    {
    	#ifdef SERIAL_DEBUG
        	printf("\n\rETHERNET: can't connect to UDP server\r\n");
      	#endif
    }
  }
  else
  {
	  #ifdef SERIAL_DEBUG
      	  printf("\n\rETHERNET: can't create UDP pcb\r\n");
      #endif
  }
  Delayms(20);
  udp_remove(upcbb);
}


void Send_Data (void)
{
	/* disable RTC interrupt for time of data collection and transmission to avoid MCU reset */
	TM_RTC_Interrupts(TM_RTC_Int_Disable);


	si7021_env thsensor;
	SFP_Device sfp;
	FloatOrUInt tmp2;

	float v_bat = GC_BatVoltage();
	char buffer [128];


	/* set default addressing if settings can't be loaded from memory */
	IPAddress server={192,168,0,2,7};
	IPAddress mask={255,255,255,0,0};
	IPAddress local={192,168,0,200,7};
	IPAddress gateway={192,168,0,1,0};
	/* load addresses from BKP RAM Memory */
	loadIPaddress(&local, &mask, &gateway, &server);
	/* Enable power for SFP module */
	dev_control(dev_sfp,dev_on);
	/* Enable power for PHY module */
	dev_control(dev_phy,dev_on);
	/* Configure GPIOs for connection between MCU and PHY */
	ETH_BSP_Config();

	/* Initialize LwIP Stack */
	LwIP_InitWithIP(local,mask,gateway,server);

	/* Initialize UDP Server */
	udp_echoserver_initWithPort(server.port);


	/* Read temperature and humidity for Si7021 sensor */
	thsensor.connected = SI7021_begin();
	SI7021_getHumidityAndTemperature(&thsensor);

	Delayms(450);

	/* Read DDMI readings from SFP module */
	SFP_Read(sfp_raw_ddmi_only,&sfp);

	Delayms(500);

	config(GC_LOAD,(uint8_t*)buffer,GC_ID);

	tmp2.f = mw_2_dbm(SFP_DOM_Conv_float(dom_tx,sfp,false));
	buffer[1]=tmp2.fInt[0];
	buffer[2]=tmp2.fInt[1];
	buffer[3]=tmp2.fInt[2];
	buffer[4]=tmp2.fInt[3];

	tmp2.f = mw_2_dbm(SFP_DOM_Conv_float(dom_rx,sfp,false));
	buffer[5]= 0x30;
	buffer[6]=tmp2.fInt[0];
	buffer[7]=tmp2.fInt[1];
	buffer[8]=tmp2.fInt[2];
	buffer[9]=tmp2.fInt[3];

	tmp2.f = SFP_DOM_Conv_float(dom_temp,sfp,false);
	buffer[10]=tmp2.fInt[0];
	buffer[11]=tmp2.fInt[1];
	buffer[12]=tmp2.fInt[2];
	buffer[13]=tmp2.fInt[3];

	tmp2.f = v_bat;
	buffer[14]=tmp2.fInt[0];
	buffer[15]=tmp2.fInt[1];
	buffer[16]=tmp2.fInt[2];
	buffer[17]=tmp2.fInt[3];

	tmp2.f = thsensor.celsiusHundredths/100.00;
	buffer[18]=tmp2.fInt[0];
	buffer[19]=tmp2.fInt[1];
	buffer[20]=tmp2.fInt[2];
	buffer[21]=tmp2.fInt[3];

	tmp2.f = thsensor.humidityBasisPoints/100.00;
	buffer[22]=tmp2.fInt[0];
	buffer[23]=tmp2.fInt[1];
	buffer[24]=tmp2.fInt[2];
	buffer[25]=tmp2.fInt[3];

	buffer[26] = TM_BKPSRAM_Read8(GC_LATITUDE_LOCATION);
	buffer[27] = TM_BKPSRAM_Read8(GC_LATITUDE_LOCATION+1);
	buffer[28] = TM_BKPSRAM_Read8(GC_LATITUDE_LOCATION+2);
	buffer[29] = TM_BKPSRAM_Read8(GC_LATITUDE_LOCATION+3);

	buffer[30] = TM_BKPSRAM_Read8(GC_LONGITUDE_LOCATION);
	buffer[31] = TM_BKPSRAM_Read8(GC_LONGITUDE_LOCATION+1);
	buffer[32] = TM_BKPSRAM_Read8(GC_LONGITUDE_LOCATION+2);
	buffer[33] = TM_BKPSRAM_Read8(GC_LONGITUDE_LOCATION+3);

	/* check if laser lock is enabled, if it is, then read actual status of power fiber */
	if (TM_BKPSRAM_Read8(0xF00)==0xEB)
		/* read actual status of powering fiber and place it in 34 byte of packet */
		buffer[34] = isLaserOn();
	/* if laser lock is disabled then send information to central device that power is received, laser will be always on */
	else
		buffer[34] = 1;

	buffer[35] = data[0];
	buffer[36] = sfp.connected;
	tmp2.f = input_power();
    buffer[37]=tmp2.fInt[0];
    buffer[38]=tmp2.fInt[1];
    buffer[39]=tmp2.fInt[2];
    buffer[40]=tmp2.fInt[3];
    /* send collected data to server */
    sendToServer(buffer,60,server);
    /* disconnect power from PHY */
    dev_control(dev_phy,dev_off);
    /* wait for RX power reading at central device */
    Delayms(200);
    /* disconnect power from SFP module */
    dev_control(dev_sfp,dev_off);

    printf("Time consumption: %ldms\n\r",TM_DELAY_Time());
    /* Put MCU in sleep mode */
    goSleep();
}





void ethernetSendString(char* message)
{
	IPAddress server={192,168,0,2,7};
	IPAddress gateway={192,168,0,2,7};
	IPAddress mask={255,255,255,0,0};
	IPAddress local={192,168,0,100,7};
	loadIPaddress(&local, &mask, &gateway, &server);
	dev_control(dev_phy,dev_on);
	ETH_BSP_Config();
	LwIP_InitWithIP(local,mask,local,server);
	udp_echoserver_initWithPort(server.port);
	dev_control(dev_sfp,dev_on);
	Delayms(900);
	sendToServer(message,strlen(message),server);
	dev_control(dev_sfp,dev_off);
	dev_control(dev_phy,dev_off);
}

