//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_UTIL_H_
#define _DGRID_UTIL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdint.h>
#include "types.h"

// Some common macros

#define ROUND(a,b)  ((((a)+(b)-1)/(b))*(b))

#define ABS(a)  (((a)>=0)?(a):(-(a)))

#define MAX(a,b)  (((a)>(b))?(a):(b))

#define MIN(a,b)  (((a)<(b))?(a):(b))

#define DIM(a)  (sizeof(a)/sizeof((a)[0]))

// Convert between network oder and byte order

#define H2N(h,n)  {(n)[0]=(((h)>>24)&0xff);(n)[1]=(((h)>>16)&0xff);(n)[2]=(((h)>>8)&0xff);(n)[3]=((h)&0xff);}

#define N2H(n,h)  {(h)=(((uint32_t)((n)[0]))<<24)+(((uint32_t)((n)[1]))<<16)+(((uint32_t)((n)[2]))<<8)+(((uint32_t)((n)[3]))<<0);}

#define H2N16(h,n)  {(n)[0]=(((h)>>8)&0xff);(n)[1]=((h)&0xff);}

#define N2H16(n,h)  {(h)= (((uint16_t)((n)[0]))<<8) + (((uint16_t)((n)[1]))<<0);}

// Some general utility functions...

int16_t FLOAT2INT(float in);

uint8_t *bmpRead(const char *filename,int *h,int *w);

// Convert from BFLOAT to FLOAT
inline float BF2F(float16_t x) {
    float y;
    ((uint16_t *)&y)[0] = 0;    
    ((uint16_t *)&y)[1] = x;
    return y;
}

// Convert from float to BFLOAT
inline float16_t F2BF(float x) {
    return ((uint16_t *)&x)[1];
}

// cast float to its hex presentation

#define F2HEX(x)  (*((uint32_t *)(&(x))))

#ifdef __cplusplus
}
#endif
#endif
