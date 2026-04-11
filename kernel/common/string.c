#include <common/string.h>
#include <stdint.h>


uint32_t strlen(uint8_t *str) {

uint8_t end = '\0';
uint32_t index = 0;
uint32_t count = 0;

while (str[index] != end) {
    count++;
    index++;
}
return count;
}

uint8_t* strconcat(uint8_t dst[], uint8_t* str_1, uint8_t* str_2) {
    uint32_t len_1 = strlen(str_1); 
    uint32_t len_2 = strlen(str_2);
    uint32_t index;
    for (index = 0; index < len_1 + len_2; index++) {
        if (index >= len_1) {
            dst[index] = str_2[index - len_1];
        } else { 
            dst[index] = str_1[index];
        }
    }
    dst[index] = '\0';
    return dst;
}

uint8_t* strcpy(uint8_t* dst, uint8_t* src) {
uint8_t* save = dst;
while((*dst++ = *src++));
return save;
}
