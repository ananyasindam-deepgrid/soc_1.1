//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <stdbool.h>
#include "../../../../SW/base/dgridlib.h"
#include "../../../../SW/src/soc.h"
#include "test.p.img"

#define BUFSZ (16*8*NUM_PCORE*2)

static volatile uint16_t inbuf[BUFSZ];
static volatile uint16_t outbuf[BUFSZ];

typedef struct {
   uint32_t in_p;
   uint32_t out_p;
   int len;
} REQUEST;


// Each thread is doing half the tensor
// First thread is processing the top half of tensor
// Second thread is processing the bottom half of tensor

static void test(void *_p,int pid) {
   REQUEST *req=(REQUEST *)_p;
   uint32_t from,to;

   if(pid==0) {
      from=0;
      to=req->len/2-1;
   } else {
      from=req->len/2;
      to=req->len-1;
   }

   >DTYPE(INT16)PCORE(NUM_PCORE)[0:NUM_PCORE-1].THREAD[0:15].test::_A[0:7] <= DTYPE(INT16)MEM(req->in_p)[from:to];

   >EXE_LOCKSTEP(test::add,NUM_PCORE);

   dgridTaskYield();

   >DTYPE(INT16)MEM(req->out_p)[from:to] <= DTYPE(INT16)PCORE(NUM_PCORE)[0:NUM_PCORE-1].THREAD[0:15].test::_Z[0:7];
}

//
// This is a simple test
// It add 1 to every elements of a tensor
//

void kernel_test_exe() {
   uint32_t resp;
   REQUEST req; 
   int i;

   dgridInitPcore(dgrid_pcore_img);

   req.in_p=(uint32_t)&inbuf[0];
   req.out_p=(uint32_t)&outbuf[0];
   req.len = BUFSZ;
   
   for(i=0;i < req.len;i++)
      inbuf[i]=(i&0xFF);

   FLUSH_DATA_CACHE();

   dgridDualHartExecute(test,&req);

   dgridJobDone(0);

   // Wait for response....
   for(;;) {
      if(dgridReadResponse(&resp))
         break;
   }   

   for(i=0;i < req.len;i++) {
      if(outbuf[i] != ((i&0xFF)+1)) {
         for(;;) {
            APB[0]=0xffffffff;
         }
      }
   }
}
