//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include "../../../base/dgrid.h"
#include "fcn.h"

//---  Process fully-connected layer (innerproduct)

_share int16 inner_product::bot[IP_CHUNK_SIZE];
vint16 inner_product::coef[IP_CHUNK_SIZE];
vint16 inner_product::top;
int inner_product::out_scale;
vint32 inner_product::_A;
vint16 inner_product::biasHi;
vint16 inner_product::biasLo;

_kernel_ void inner_product::init(int _out_scale) {
   out_scale=_out_scale;
}

_kernel_ void inner_product::start() {
   _A = biasLo;
   _A += biasHi*1024;
}

_kernel_ void inner_product::exe() {
   int i;
#pragma unroll
   for(i=0;i < IP_CHUNK_SIZE;i++) {
      _A += bot[i]*coef[i];
   }
}

// Activation

_kernel_ void inner_product::activate_none() {
   top = _A >> out_scale;
}

// ---- Pooling layer...

vint16 max_pool::bot[POOL_BOT_SIZE];
vint32 max_pool::_A;
int max_pool::out_scale;
vint16 max_pool::top;

_kernel_ void max_pool::init(int _out_scale) {
   out_scale=_out_scale;
   _A=0;
}

// Do pooling averate.
// Since pcore cannot do division, just do addition here
// Divide for averaging is done by stream processor later
 
_kernel_ void max_pool::exe() {
   vint16 *p;
   int i;
   p=bot;
#pragma unroll
   for(i=0;i < POOL_BOT_SIZE;i++) {
      _A += p[0];
      p++;
   }
}

_kernel_ void max_pool::finish() {
   top = _A >> out_scale;
   _A = 0;
}

// --- Do concatenation layer.

_share int16 concatenate::buf[CONCATENATE_BUFSZ];

// No pcore code required for concatenation layer
// Concatenation layer is processed by mcore and stream processor only

_kernel_ void concatenate::dummy(int16 _dummy) {
   buf[0]=_dummy;
}
