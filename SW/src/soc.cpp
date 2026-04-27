//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "soc.h"
#include "../base/dgrid.h"
#include "../base/util.h"

#define NUM_CAMERA_CAPTURE  4

#define NUM_VIDEO_FRAME     4

// Memory mapped of APB bus

#define APB ((volatile unsigned int *)0xC0000000)

static void *buffer_vga[NUM_VIDEO_FRAME];

static void *buffer_camera[NUM_CAMERA_CAPTURE];

static void *curr_camera_capture=0;

static int curr_video=0;

extern unsigned int *vgabuf;

static int camera_last_read=0;

uint8_t *DisplayCanvas=0;


//----------------------------
// Initialize VGA display driver
// The driver configures the Xilinx's VDMA IP
//-----------------------------

DgridStatus DisplayInit(int w,int h) {
   int r;
   static bool init=false;
   if(init)
      return DgridStatusOk;
   if(w!=WEBCAM_WIDTH || h!=WEBCAM_HEIGHT)
      return DgridStatusFail;
   // Configure VGA display output stream
   for(r=0;r < NUM_VIDEO_FRAME;r++) {
      buffer_vga[r]=malloc(WEBCAM_WIDTH*3*(WEBCAM_HEIGHT+2));
      APB[APB_VIDEO_BUFFER+r]=(unsigned int)buffer_vga[r];
   }
   APB[APB_VIDEO_ENABLE]=1;
   DisplayCanvas=(uint8_t *)buffer_vga[curr_video];
   init=true;
   return DgridStatusOk;
}

//---------------------------------
// Make the current working display buffer returned from DisplayGetBuffer()
// to be the active screen display buffer
//-----------------------------------

DgridStatus DisplayUpdateBuffer() {
   APB[APB_VIDEO_BUFFER]=(uint32_t)buffer_vga[curr_video];
   curr_video++;
   if(curr_video >= NUM_VIDEO_FRAME)
      curr_video=0;
   DisplayCanvas=(uint8_t *)buffer_vga[curr_video];
   return DgridStatusOk;
}

//------------------------------------
// Initialize the camera driver
// The driver configures the Xilinx's VDMA IP core
//------------------------------------

DgridStatus CameraInit(int w,int h) {
   int r;
   static bool init=false;
   if(init)
      return DgridStatusOk;
   if(w!=WEBCAM_WIDTH || h!=WEBCAM_HEIGHT)
      return DgridStatusFail;
   curr_camera_capture=malloc(WEBCAM_WIDTH*3*(WEBCAM_HEIGHT+2));
   // Configure camera input stream
   for(r=0;r < NUM_CAMERA_CAPTURE;r++) {
      buffer_camera[r]=malloc(WEBCAM_WIDTH*3*(WEBCAM_HEIGHT+2));
      APB[APB_CAMERA_BUFFER+r]=(unsigned int)buffer_camera[r];
   }
   APB[APB_CAMERA_ENABLE]=1;
   init=true;
   return DgridStatusOk;
}

// Check if capture from camera is ready

bool CameraCaptureReady() {
   int next_read,curr_read;
   unsigned int vv;
   void *temp;

   next_read=APB[APB_CAMERA_CURR_FRAME];
   if(next_read != camera_last_read) {
      camera_last_read=next_read;
      curr_read=(camera_last_read==0)?(NUM_CAMERA_CAPTURE-1):camera_last_read-1;
      // Swap current camera buffer for the current capture buffer
      temp=buffer_camera[curr_read];
      buffer_camera[curr_read]=curr_camera_capture;
      curr_camera_capture=temp;
      APB[APB_CAMERA_BUFFER+curr_read]=(unsigned int)buffer_camera[curr_read];
      return true;
   }
   else
      return false;
}

//---------------------------------------
// Get latest camera capture if available
//---------------------------------------

uint8_t *CameraGetCapture() {
   return (uint8_t *)curr_camera_capture;
}

//----------------------------------------
// Set LED state (on/off)
//----------------------------------------

void LedSetState(uint32_t ledState) {
   APB[APB_LED]=ledState;
}

//-----------------------------------------
// Get push button current state
//-----------------------------------------

uint32_t PushButtonGetState() {
   return APB[APB_PB];
}

//-----------------------------------------
// Read UART characters
//-----------------------------------------

uint8_t UartRead() {
   return (uint8_t)APB[APB_UART_READ];
}

//-------------------------------------------
// Write UART character
//---------------------------------------------

void UartWrite(uint8_t ch) {
   APB[APB_UART_WRITE]=(uint32_t)ch;
}

//-----------------------------------------------
// Return number of available UART characters for
// reading
//-----------------------------------------------

int UartReadAvailable() {
   return (int)APB[APB_UART_READ_AVAIL];
}

//-----------------------------------------------
// Return number of spaces available for UART
// transmission FIFO
//-----------------------------------------------

int UartWriteAvailable() {
   return (int)APB[APB_UART_WRITE_AVAIL];
}

//-----------------------------------------
// UART1 (CAN bridge) driver functions
//-----------------------------------------

uint8_t Uart1Read() {
   return (uint8_t)APB[APB_UART1_READ];
}

void Uart1Write(uint8_t ch) {
   APB[APB_UART1_WRITE]=(uint32_t)ch;
}

int Uart1ReadAvailable() {
   return (int)APB[APB_UART1_READ_AVAIL];
}

int Uart1WriteAvailable() {
   return (int)APB[APB_UART1_WRITE_AVAIL];
}

