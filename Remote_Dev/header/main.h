/*
 ******************************************************************************
 * @file    main.h
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    31-October-2011
 * @brief   This file contains all the functions prototypes for the main.c
 *          file.
 ******************************************************************************
 */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "bkpram_layout.h"
#include "tm_stm32f4_id.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_gps.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "stm32f4x7_eth_bsp.h"
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_bkpsram.h"
#include "tm_stm32f4_low_power.h"
#include "tm_stm32f4_timer_properties.h"

#include "gsm.h"                                                    /* gsm communication */
#include "sfp.h"
#include "SI7021.h"                                                 /* Temp & Humidity Sensor */
#include "INA219.h"
#include "cmdline.h"
#include "board_conf.h"
#include "bkpram_layout.h"
#include "utilities_new.h"      									// Miscellaneous utilities



/* 	Here you can decide where to store device configuration data. Currently only RAM Config is supported
 	in software, that means if you disconnect main battery then configuration will be lost and you would
 	need to configure everything from the beginning */
//#define FLASH_CONFIG
#define RAM_CONFIG   

/* 	Here you can enable GSM output to be printed on the console. Used for GSM debugging */
//#define GSM_DEBUG

/* Defines interface type used between MAC and PHY */
#define RMII_MODE

/* defines maximum size of packet */
#define PKT_SIZE 64





extern short gps_request;

void Time_Update(void);
void Delay(uint32_t nCount);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
