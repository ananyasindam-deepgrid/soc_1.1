//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include "../../../base/dgrid.h"

// Current main module has no assiated pcore
// Create a dummy one anyway since the build now expects a pcore
// image for each mcore image

_kernel_ void main::dummy(int16 x)
{
   x=1;
}

