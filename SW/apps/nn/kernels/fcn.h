//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _APPS_NN_KERNELS_FCN_H_
#define _APPS_NN_KERNELS_FCN_H_
#ifdef __cplusplus
extern "C" {
#endif

extern void kernel_innerProduct_exe(
   unsigned int _req_id,
   unsigned int _coef,
   unsigned int _biasHi,
   unsigned int _biasLo,
   unsigned int _bot,
   unsigned int _top,
   int _topcnt,
   int _botcnt,
   int _coeftopcnt,
   int _coefbotcnt,
   unsigned int _stream,
   int _top_scale,
   int _num_pcore,
   int _num_thread
);

extern void kernel_concatenate_exe(
   unsigned int _req_id,
   int _cnt,
   unsigned int *_src,
   int *_copySize,
   unsigned int *_spu,
   unsigned int *_dest
);

extern void kernel_logistic_exe(
   unsigned int _req_id,
   int _copySize,
   unsigned int _src,
   unsigned int _dest,
   unsigned int _spu
);

extern void kernel_Pooling_exe(
   unsigned int _req_id,
   unsigned int _bot,
   unsigned int _top,
   int _ksz,
   int _stride,
   int _topcnt,
   int _topdim,
   int _botcnt,
   int _botdim,
   unsigned int _stream,
   int _output_shift
);

#define IP_CHUNK_SIZE  8

#define POOL_BOT_SIZE  8

#define CONCATENATE_BUFSZ 1600

#ifdef __cplusplus
}
#endif
#endif
