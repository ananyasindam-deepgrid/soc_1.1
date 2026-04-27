//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

//-----------
// This file defined API to retrieve run time log from dgrid
//------------

#ifndef __DGRIDLOG_H__
#define __DGRIDLOG_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "dgrid.h"

extern void dgridLogFlush();

extern void dgridLogPrint();

#ifdef __cplusplus
}
#endif

#endif
