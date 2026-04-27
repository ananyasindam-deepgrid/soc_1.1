//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include "../../../base/dgridlib.h"

static uint16_t *last_image=0;

// Load code/memory to pcore

void dgridInitPcore(uint16_t *_image) {
   uint16_t *c_p,*pcore_p;
   int c_len,pcoreLen;
   
   if(last_image==_image)
      return;
   last_image=_image;
   
   pcore_p = _image;
   pcoreLen= pcore_p[0];
   pcore_p++;
   
   c_p = pcore_p+pcoreLen/sizeof(uint16_t);
   c_len = c_p[0];
   c_p++;
   
   c_len=c_len>>1;
   pcoreLen=pcoreLen>>2;
  
   DGRIDM_GREG(0,REG_DP_VM_TOGGLE,0)=0;
   ;DGRIDM_GREG(0,5,0)=(0+(11<<3));
   c_len=((c_len+3)>>2)<<2;

   // Set pcore process1's constant memory space
   // Set pcore process1's constant memory space
   if(c_len > 0) {
      > DTYPE(INT16) PCORE[*].root.constant[0:c_len-1] <= DTYPE(INT16)MEM((uint32_t)c_p)[0:c_len-1];
   }
   DGRIDM_GREG(0,REG_DP_VM_TOGGLE,0)=0;

   // Set pcore process0's constant memory space.
   if(c_len > 0) {
      > DTYPE(INT16) PCORE[*].root.constant[0:c_len-1] <= DTYPE(INT16)MEM((uint32_t)c_p)[0:c_len-1];
   }

   // Set pcore code space
   > PROG((pcoreLen>>1)) <= DTYPE(INT16)MEM((uint32_t)pcore_p,(pcoreLen>>1)<<2)[:];
   > BARRIER;
}

// Download code to stream processor
// Code for stream processor is a table lookup to map between input and output

void dgridInitStream(uint32_t _spu) {
   int spuCnt;
   if(_spu ) {
      spuCnt=*((uint16_t *)_spu);
      _spu += sizeof(uint16_t);
      > SPU <= DTYPE(INT16)MEM(_spu,spuCnt*SPU_LOOKUP_SIZE)[:];
      > BARRIER;
   }
}

// A job is finished

void dgridJobDone(unsigned int job_id) {
   >CALLBACK(0,job_id);
}
