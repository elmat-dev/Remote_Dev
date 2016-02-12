#include "gc_memory_helper.h"


int8_t GC_BKPSRAM_WriteMulti_Char(char* data, short address, short length) {
  if ( (address + length) < TM_BKPSRAM_GetMemorySize() ) {
    for (int i=0;i<length;i++)
      TM_BKPSRAM_Write8(address++,*data++);
    return 0;
  }
  else {
    #ifdef SERIAL_DEBUG
      TM_USART_Puts(DEBUG_USART,"\n\rBKP RAM: You can't write to BKP SRAM: Address + Length exceeds memory capacity!\n\r");
    #endif 
    return -1;
  }
  
}


int8_t GC_BKPSRAM_ReadMulti_Char(char* data, short address, short length) {
  if ( (address + length) < TM_BKPSRAM_GetMemorySize() ) {
    for (int i=0;i<length;i++)
      data[i]=TM_BKPSRAM_Read8(address++);
    return 0;
  }
  else {
    #ifdef SERIAL_DEBUG
      TM_USART_Puts(DEBUG_USART,"\n\rBKP RAM: You can't read from outside of BKP SRAM memory space!\n\r");
    #endif 
    return -1;
  }
  
}



