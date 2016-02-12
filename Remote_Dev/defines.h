/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
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
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Put your global defines for all libraries here used in your project */
 #define USART_BUFFER_SIZE			128

 // decide which timer to use for Delay library
 #define TM_DELAY_TIM				TIM2
 #define TM_DELAY_TIM_IRQ			TIM2_IRQn
 #define TM_DELAY_TIM_IRQ_HANDLER	TIM2_IRQHandler
 

 #define MENU_USART               	USART6
 #define MENU_USART_PINS          	TM_USART_PinsPack_1
 #define MENU_USART_SPEED           115200

 #define GPS_USART                  USART3
 #define GPS_USART_PINSPACK         TM_USART_PinsPack_2
 #define GPS_USART_SPEED            9600
 
 #define GSM_USART                  USART1
 #define GSM_USART_PINS            	TM_USART_PinsPack_2
 #define GSM_USART_SPEED           	115200



 #define SI7021_I2C_PORT            I2C3
 #define SI7021_I2C_PINS            TM_I2C_PinsPack_1
 #define SI7021_I2C_SPEED           200000
 #define SI7021_ADDRESS             (0x40 << 1)
 #define TM_I2C_TIMEOUT             200
 #define TM_I2C3_ACK				I2C_Ack_Enable

 #define SFP_I2C_PORT				I2C3
 #define SFP_I2C_PINS				TM_I2C_PinsPack_1
 #define SFP_I2C_ADDRESS			0xA2
 #define SFP_I2C_SPEED				200000
   
#endif
