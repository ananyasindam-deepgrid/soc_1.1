//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include "../../../base/dgrid.h"

vint16 test::_A;
vint16 test::_Z;

_kernel_ void test::add() { 
   _VMASK=-1;
   _Z=_A+1;
}
