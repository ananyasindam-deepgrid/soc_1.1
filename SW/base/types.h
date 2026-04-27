//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_TYPES_H_
#define _DGRID_TYPES_H_

#include <stdint.h>

typedef enum {
   DgridStatusOk=0,
   DgridStatusFail=-1,
   DgridStatusBusy=1,
   DgridStatusPending=2
} DgridStatus;

typedef uint16_t float16_t;

#endif
