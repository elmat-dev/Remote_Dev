#ifndef SFP_h
#define SFP_h

#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"
#include "board_conf.h"
#include <math.h>
#include <stdbool.h>



#define SFP_POWER		GPIO_Pin_1
#define SFP_POWER_RRC   RCC_AHB1Periph_GPIOE
#define SFP_POWER_PORT  GPIOE

#define GC_OFF          0
#define GC_ON           1

#define SFP_NOT_FOUND   false
#define SFP_READ_OK     true

typedef struct {
    float temp;
    float voltage;
    float current;
    float tx_power;
    float rx_power;
    float tx_power_dbm;
    float rx_power_dbm;
    int connected;
} SFP_Data;

typedef enum {
	pca_port0_7  = 0xE8,
	pca_port8_15 = 0xEC,
} SFP_PCA_Select;

typedef enum {
	port0 = 0x01,
	port1 = 0x02,
	port2 = 0x04,
	port3 = 0x08,
	port4 = 0x10,
	port5 = 0x20,
	port6 = 0x40,
	port7 = 0x80,
	port8 = 0x01,
	port9 = 0x02,
	port_none = 0x00,
} SFP_PORT_Select_t;

typedef enum {
	temperature = 0x60,
	voltage = 0x62,
	current = 0x64,
	tx_mw = 0x66,
	rx_mw = 0x68,
	tx_dbm = 0x10,
	rx_dbm = 0x20,
} SFP_READ_DDMI_Type_t;


void SFP_GPIO_Config (void);
int SFP_Control (char);

bool SFP_Init(void);

uint8_t SFP_PORT_Select (uint8_t);
uint16_t SFP_READ_DDMI_Single_16bit (SFP_READ_DDMI_Type_t);
void SFP_READ_DDMI_All_8bit (uint8_t*);
void SFP_READ_DDMI_All_16bit (uint16_t*);
void SFP_READ_DDMI_All_float (float*);
float SFP_DDMI_RAW2Float (SFP_READ_DDMI_Type_t, uint16_t);
void SFP_READ_DDMI (SFP_Data* );







#endif
