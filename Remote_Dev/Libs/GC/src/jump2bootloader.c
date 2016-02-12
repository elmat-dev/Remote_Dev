void (*SysBootLoader) (void);

void bootloader (void) {
  
  SysBootLoader = (void (*)(void)) (*((u32 *) 0x1fff0004));

  RCC_DeInit();
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;
  __set_PRIMASK(1);
  __set_MSP(0x20001000);
  SysBootLoader();
}