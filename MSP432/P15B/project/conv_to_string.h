//conv_to_string.h

#ifndef __CONV_TO_STRING_H__
#define __CONV_TO_STRING_H__
#include <stdint.h>
#include <math.h>

char* itoa(int16_t x, char* buffer, uint8_t base);

char* ftoa(float n, char *res, int afterpoint);


#endif //__CONV_TO_STRING_H__
