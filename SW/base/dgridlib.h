//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

//-----------
// This file defined the API functions for host applications to communicate with
// dgrid
//------------

#ifndef __DGRIDLIB_H__
#define __DGRIDLIB_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "dgrid.h"

// Functions that are implemented in task.S

extern void _taskYield(void);

extern void _taskSpawn(uint32_t,uint32_t,uint32_t,uint32_t);

// Memory blocks to be shared between dgrid and host

typedef void * DGRID_SHARED_MEM;

#define DGRID_SHARED_MEM_VIRTUAL(p)   ((void *)(p))

#define DGRID_SHARED_MEM_PHYSICAL(p)  ((uint32_t)p)

// Function call back for SPU function generator

typedef int16_t (*SPU_FUNC)(int16_t,void *pparm,uint32_t parm,uint32_t parm2);

// Task function API

void dgridInit();

void dgridDualHartExecute(void(*func)(void *,int),void *pparm);

uint32_t dgridBuildKernelFunc(uint32_t _func,int num_pcore,int num_tid);

#define dgridTaskYield() {DGRIDM_GREG(0,REG_DP_VM_TOGGLE,0)=0;_taskYield();}

// Shared memory allocation API

extern DGRID_SHARED_MEM dgridAllocSharedMem(int _size);

extern void dgridFreeSharedMem(DGRID_SHARED_MEM);

// SPU builder API

extern DGRID_SHARED_MEM dgridBuildSpuBundle(int numSpuImg,...);

// Software download for pcore and streamer

extern void dgridInitPcore(uint16_t *_image);

extern void dgridInitStream(uint32_t _spu);

extern void dgridJobDone(unsigned int job_id);

extern bool dgridReadResponse(uint32_t *resp);

// Abort execution

extern void dgridAbort(int _errorCode);

#ifdef __cplusplus
}
#endif

#endif
