#ifndef STRING_H
#define STRING_H


#include <common/types.h>

#define MAX_BUFFER 256

uint8_t *strconcat(uint8_t* str_1, uint8_t* str_2); 
uint32_t strlen(uint8_t* str);

#endif