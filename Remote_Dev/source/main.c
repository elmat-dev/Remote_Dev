#include "main.h"

#include "cmdline_functions.h"                                                  // Command Line Interface (CLI) implementation */
#include "udp_echoclient.h"                                                     /* ethernet communication */


/*  to output printf to selected USART please edit _write function in syscalls.c file,
 *  currently set to USART6 (MENU_USART)
 */



bool interLockON = false;                                                       // check if LaserLock is enabled
short gps_request = 0;                                                          // server requested gps update (not implemented now)
config_t conf;                                                                  // structure for storing configuration data
uint8_t bt_cnt;                                                                 // boot counter loaded from BKP SRAM memory
void BUTTON1_EventHandler(TM_BUTTON_PressType_t type);							// Declaration of Menu Button handler function



int main(void)
{

  /* initialize libraries */
  SystemInit();                                                                 // clock and PLL initialization
  TM_RTC_Init(TM_RTC_ClockSource_External);                                     // initialize RTC clock, with external crystal as source
  TM_USART_Init(MENU_USART, MENU_USART_PINS, MENU_USART_SPEED);              	// initialize UART interface, DEBUG (printf) and CLI Menu output
  TM_DELAY_Init();                                                              // delay library initialization
  TM_DELAY_SetTime(0);                                                          // reset runtime counter
  TM_BKPSRAM_Init();                                                            // initialize BKP SRAM memory

  SFP_GPIO_Config();                                                         	// configure GPIOs for SFP control
  PHY_GPIO_Config();                                                         	// configure GPIOs used for PHY control
  GSM_GPIO_Config();

  TM_BUTTON_Init(GPIOA, GPIO_PIN_0, 1, BUTTON1_EventHandler);					// Set GPIOA pin 0 as input using TM_Button library

  TM_I2C_Init(SFP_I2C_PORT, SFP_I2C_PINS, SFP_I2C_SPEED);

  bt_cnt = TM_BKPSRAM_Read8(0xF0);                                              // load boot counter variable from BKP SRAM memory
  printf("Wake counter: %d \n\r", bt_cnt);
  bt_cnt++;                                                                     // increase boot counter
  TM_BKPSRAM_Write8(0xF0,bt_cnt);                                               // save boot counter to BKP SRAM

  TM_BUTTON_Update();                                                   		/* Menu service, enabled when low state is applied on PD13, exit when CTRL+C is pressed*/


    if ((isLaserOn()==0) && (TM_BKPSRAM_Read8(0xF00)==0xEB)) {                  // interlock: if there is no signal from ZSPM4521 charger controller, then assume that power fiber is broken and send laser shutdown request to central device
    	if (TM_BKPSRAM_Read8(1000) == 0x02)
    		gsm();                                								// send SMS with request to disable laser (command: Off(id), ex. Off6 to disable laser number 6 (counted from 0)
    	Send_Data();                                       						// send data with lock status via ethernet
     }
   
    if (bt_cnt > 10) {                                                          // after each Nth boot collect data and send it over ethernet
      TM_BKPSRAM_Write8(0xf0,0);                                                // reset boot counter and write to BKP SRAM
      Send_Data();                                         						// collect data and send it to central device, called from udp_echoclient.c file
    }

    printf("Run Time: %ldms\n\r",TM_DELAY_Time());                              // uncomment this line if you want to check program execution time
    goSleep();                                                                  // everything is done, go sleep
} 
                  
 







// to used really
void SysTick_Handler (void) {

}


// handler function for user button actions. If pressed then program goes to Configuration menu.
void BUTTON1_EventHandler(TM_BUTTON_PressType_t type) {
    if (type == TM_BUTTON_PressType_OnPressed)
    	Config_Menu();
}


// if HardFault Excetion is in effect then error information will be printed to user terminal
void HardFault_Handler (void) {
 printf("System has encountered Hard Fault Exception. Please check and debug your code\n");
 while(1);
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
