//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _SOC_H_
#define _SOC_H_

#include <stdint.h>
#include "../base/types.h"
#include "../base/dgrid.h"

#define DISPLAY_WIDTH       640

#define DISPLAY_HEIGHT      480

#define WEBCAM_WIDTH        640

#define WEBCAM_HEIGHT       480

// Memory mapped of APB bus

#define APB ((volatile unsigned int *)0xC0000000)

// APB register map

#define APB_LED               0

#define APB_PB                2

#define APB_VIDEO_BUFFER      11

#define APB_VIDEO_ENABLE      9

#define APB_CAMERA_BUFFER     5

#define APB_CAMERA_ENABLE     3

#define APB_CAMERA_CURR_FRAME 4

#define APB_UART_READ         15 

#define APB_UART_WRITE        16 

#define APB_UART_READ_AVAIL   17 

#define APB_UART_WRITE_AVAIL  18 

#define APB_TIME_GET          19

#define APB_TIME2_GET         20

// UART1 (CAN bridge) register map
#define APB_UART1_READ        21

#define APB_UART1_WRITE       22

#define APB_UART1_READ_AVAIL  23

#define APB_UART1_WRITE_AVAIL 24


// Flush data cache with DgsRiscv
// This is dependent on the Riscv implementation since flushing datacache
// is not defined in official Riscv specs

#ifdef __WIN32__
#define FLUSH_DATA_CACHE() {}
#else
#define FLUSH_DATA_CACHE()  {asm(".word 0x500F");}
#endif

extern uint8_t *DisplayCanvas;

#ifdef __cplusplus
extern "C" {
#endif
DgridStatus DisplayInit(int w,int h);

inline uint8_t *DisplayGetBuffer(void) {return DisplayCanvas;}

DgridStatus DisplayUpdateBuffer(void);

DgridStatus CameraInit(int w,int h);

bool CameraCaptureReady();

uint8_t *CameraGetCapture(void);

void LedSetState(uint32_t ledState);

uint32_t PushButtonGetState();

uint8_t UartRead();

void UartWrite(uint8_t ch);

int UartReadAvailable();

int UartWriteAvailable();

// UART1 (CAN bridge) driver functions
uint8_t Uart1Read();

void Uart1Write(uint8_t ch);

int Uart1ReadAvailable();

int Uart1WriteAvailable();
#ifdef __cplusplus
}
#endif

#define TimeGet() (APB[APB_TIME_GET])

#define Time2Get() (APB[APB_TIME2_GET])

#endif
