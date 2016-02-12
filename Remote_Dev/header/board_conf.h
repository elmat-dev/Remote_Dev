#ifndef BOARD_CONF_H
#define BOARD_CONF_H

#include "tm_stm32f4_usart.h"




/* ZSPM4521 Access GPIO configuration */
#define ZSPM_I2C				       	I2C3
#define ZSPM_I2C_PINSPACK			    TM_I2C_PinsPack_1
#define ZSPM_I2C_ADDRESS			    0x48 << 1
#define ZSPM_I2C_SPEED			        200000


/* GPS Control GPIO configuration */
#define GPS_POWER										GPIO_Pin_0
#define GPS_RESET										GPIO_Pin_12
#define GPS_POWER_RRC                                   RCC_AHB1Periph_GPIOD
#define GPS_RESET_RRC                                   RCC_AHB1Periph_GPIOC
#define GPS_POWER_PORT                                  GPIOD
#define GPS_RESET_PORT                                  GPIOC

/* GSM Control GPIO configuration */
#define GSM_POWER										GPIO_Pin_9
#define GSM_RESET										GPIO_Pin_7
#define GSM_WAKEUP										GPIO_Pin_6
#define GSM_ON  										GPIO_Pin_0
#define GSM_POWER_RRC                                   RCC_AHB1Periph_GPIOB
#define GSM_RESET_RRC                                   RCC_AHB1Periph_GPIOD
#define GSM_WAKEUP_RRC                                  RCC_AHB1Periph_GPIOD
#define GSM_ON_RRC                                      RCC_AHB1Periph_GPIOE
#define GSM_POWER_PORT                                  GPIOB
#define GSM_RESET_PORT                                  GPIOD
#define GSM_WAKEUP_PORT                                 GPIOD
#define GSM_ON_PORT                                     GPIOE

/* PHY Control GPIO configuration */
#define PHY_POWER										GPIO_Pin_0
#define PHY_CLOCK										GPIO_Pin_1
#define PHY_RRC                                         RCC_AHB1Periph_GPIOB
#define PHY_PORT                                        GPIOB

/* SFP Control GPIO configuration */
#define SFP_POWER										GPIO_Pin_1
#define SFP_POWER_RRC                                   RCC_AHB1Periph_GPIOE
#define SFP_POWER_PORT                                  GPIOE

/* ZSPM4521 Control GPIO configuration */
#define ZSPM_POWER										GPIO_Pin_14
#define ZSPM_POWER_RRC                                  RCC_AHB1Periph_GPIOB
#define ZSPM_POWER_PORT                                 GPIOB

#endif
