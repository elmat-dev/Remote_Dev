#include "gsm.h"



void GSM_Init() {
	printf("GSM: Initializing module...\n");
	gsm_state = STATE_NONE;
	char buf[BUF_LENGTH];
	TM_USART_Init(GSM_USART, TM_USART_PinsPack_2, 115200);

	dev_control(dev_gsm,dev_on);

  	printf("GSM: Initialization was done successfully, trying to communicate with module ...\n");
  	GSM_requestModem("AT", 1000, false, buf);
	gsm_state |= STATE_INITIALIZED;
	printf("GSM: Communication with module was established successfully!\n");
}


char* GSM_skip(char *str, char match) {
  uint8_t c = 0;
  while (true) {
    c = *str++;
    if ((c == match) || (c == '\0')) {
      break;
    }
  }
  return str;
}


char* GSM_readToken(char *str, char *buf, char delimiter) {
  uint8_t c = 0;
  while (true) {
    c = *str++;
    if ((c == delimiter) || (c == '\0')) {
      break;
    }
    else if (c != ' ') {
      *buf++ = c;
    }
  }
  *buf = '\0';
  return str;
}



uint8_t GSM_requestModem(char *command, uint16_t timeout, bool check, char *buf)
{
  uint8_t count = 0;
  char *found = 0;
  *buf = 0;
  //printf(command);
  //printf("\n");
  TM_USART_Puts(GSM_USART,command);
  TM_USART_Puts(GSM_USART,"\r");
  count = GSM_getsTimeout(buf, timeout);
  if (count)
  {
	  if (check)
	  {
		  found = strstr(buf, "\r\nOK\r\n");
      	  if (found)
      	  {
      		  printf("GMS: 'OK' response received\n");
      	  }
      	  else
      	  {
      		  printf("GMS: 'OK' not received. Received data:\n");
      		  printf(buf);
      		  printf("\n");
      	  }
	  }
	  else
	  {
		  printf("GSM: Response from module: \n");
		  printf(buf);
		  printf("\n");
	  }
  }
  else
  {
	  printf("GSM: No response\n");
  }
  return count;
}





uint8_t GSM_requestModemWaitAnswer(char *command, uint16_t timeout, bool check, char *expected_resp, char *buf)
{
	uint8_t count = 0;
	char *found = 0;
	*buf = 0;
	//printf(command);
	//printf("\n");
	TM_USART_Puts(GSM_USART,command);
	TM_USART_Puts(GSM_USART,"\r");
	count = GSM_getsTimeout(buf, timeout);
	if (count)
	{
		if (check)
		{
			found = strstr(buf, expected_resp);
			if (found)
			{
				count = 255;
				return count;
			}
			else
			{
				printf("GSM: Incorrect response: \n");
				printf(buf);
				printf("\n");
			}
		}
		else
		{
			printf("GSM: Incorrect response: \n");
			printf(buf);
			printf("\n");

		}
	}
  else
  {
	  printf("GMS: No response\n");
  }
 return count;
}



uint8_t GSM_getsTimeout(char *buf, uint16_t timeout) {
  uint8_t count = 0;
  long timeIsOut = 0;
  char c;
  *buf = 0;
  timeIsOut = TM_DELAY_Time() + timeout;
  while (timeIsOut > TM_DELAY_Time() && count < (BUF_LENGTH - 1)) {
    if (!TM_USART_BufferEmpty(GSM_USART)) {
      count++;
      c = TM_USART_Getc(GSM_USART);
      TM_USART_Putc(MENU_USART,c);
      *buf++ = c;
      timeIsOut = TM_DELAY_Time() + timeout;
    }
  }
  if (count != 0) {
    *buf = 0;
    count++;
  }
  return count;
}



void GSM_sendSMS(char *number, char *message) {
	char buf[BUF_LENGTH];
	char cmdbuf[30] = "AT+CMGS=\"";
	printf("Trying to send SMS ...\n");
	GSM_requestModem("AT+CMGF=1", 2500, true, buf); // Set SMS mode to text
	strcat(cmdbuf, number);
  	strcat(cmdbuf, "\"");
  	GSM_requestModemWaitAnswer(cmdbuf, 2000, true, "\r\n> ", buf);
  	TM_USART_Puts(GSM_USART, message);
  	TM_USART_Putc(GSM_USART,0x1A);                 	// Send CTRL + Z as end character
  	TM_USART_Putc(GSM_USART,0x0D);
  	GSM_getsTimeout(buf, 4000);
}




void GSM_checkNetwork_Simple(int timeout) {
	gsm_state &= ~STATE_REGISTERED;
	char buf[BUF_LENGTH];
	if (GSM_requestModemWaitAnswer("AT+CREG?", timeout, true, "+CREG: 0,1", buf) == 255)
		gsm_state |= STATE_REGISTERED;
}


bool GSM_isRegistered() {
  return (gsm_state & STATE_REGISTERED);
}






/* Not used in this firmware release */
bool GSM_isOn() {
  return (gsm_state & STATE_ON);
}


/* Not used in this firmware release */
bool GSM_isInitialized() {
  return (gsm_state & STATE_INITIALIZED);
}

/* Not used in this firmware release */
void GSM_checkNetwork() {
	char buf[BUF_LENGTH];
	char result;
	GSM_requestModem("AT+CREG?", 1000, false, buf);
	result = buf[20];
	if (result == '1')
		gsm_state |= STATE_REGISTERED;
	else
		gsm_state &= ~STATE_REGISTERED;

}


/* Not used in this firmware release */
void GSM_version() {
  char buf[BUF_LENGTH];
  printf("version info ...\n");
  GSM_requestModem("AT+GMI", 1000, false, buf);
  GSM_requestModem("AT+GMM", 1000, false, buf);
  GSM_requestModem("AT+GMR", 1000, false, buf);
  GSM_requestModem("AT+CSQ", 1000, false, buf);
  printf("done\n");
}


/* Not used in this firmware release */
void GSM_Send(const char *s)
{
   while(*s)
   {
     TM_USART_Putc(GSM_USART,*s++);
   }
   TM_USART_Putc(GSM_USART,0x0D);
}


/* Not used in this firmware release */
void GSM_send(char *buf) {
  TM_USART_Puts(GSM_USART, buf);
}


/* Not used in this firmware release */
char* GSM_receive(char *buf) {
	GSM_getsTimeout(buf, 1000);
  return buf;
}


/* Not used in this firmware release */
void GSM_switchOn() {
  printf("GSM: Switching module on\n");
  if (!GSM_isOn()) {
	dev_control(dev_gsm,dev_on);
	gsm_state |= STATE_ON;
  }
  printf("GMS: Done\n");
}

/* Not used in this firmware release */
void GSM_switchOff() {
  printf("GSM: Switching module off\n");
  if (GSM_isOn()) {
    dev_control(dev_gsm,dev_off);
    gsm_state &= ~STATE_ON;
  }
  printf("GSM: Done\n");
}


/* Not used or implemented in this firmware release */
void GSM_switchModem() {}
