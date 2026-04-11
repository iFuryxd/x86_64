#ifndef STRING_H
#define STRING_H


#include <common/types.h>

#define MAX_BUFFER 256



uint8_t* strconcat(uint8_t* dst, uint8_t* str_1, uint8_t* str_2); 
uint8_t* strcpy(uint8_t* src, uint8_t* dst);
uint32_t strlen(uint8_t* str);

#endif