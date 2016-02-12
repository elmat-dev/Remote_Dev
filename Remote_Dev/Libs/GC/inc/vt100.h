#include <stdio.h>
#include <string.h>
#include "tm_stm32f4_usart.h"



#define VT100_ATTR_OFF		        0
#define VT100_BRIGTH		        1
#define VT100_DIM		        2
#define VT100_USCORE		        4
#define VT100_BLINK		        5
#define VT100_REVERSE		        7
#define VT100_HIDDEN		        8
#define VT100_BOLD_OFF		        21
#define VT100_USCORE_OFF	        24
#define VT100_BLINK_OFF		        25
#define VT100_REVERSE_OFF	        27

#define VT100_FOREGROUND_BLACK          30
#define VT100_FOREGROUND_RED            31
#define VT100_FOREGROUND_GREEN          32
#define VT100_FOREGROUND_YELLOW         33
#define VT100_FOREGROUND_BLUE           34
#define VT100_FOREGROUND_MAGENTA        35
#define VT100_FOREGROUND_CYAN           36
#define VT100_FOREGROUND_WHITE          37




void vt100Init(USART_TypeDef*);
void vt100ClearScreen(USART_TypeDef*);
void vt100CursorHome(USART_TypeDef*);
void vt100SetAttr(USART_TypeDef*, uint8_t attr);
void vt100SetCursorMode(USART_TypeDef*, uint8_t visible);
void vt100SetCursorPos(USART_TypeDef*, uint8_t line, uint8_t col);