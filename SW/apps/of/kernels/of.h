//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _APPS_OF_KERNELS_OF_H_
#define _APPS_OF_KERNELS_OF_H_
#ifdef __cplusplus
extern "C" {
#endif

extern void kernel_of_exe(
   unsigned int req_id,
   unsigned int _input[2],
   unsigned int _x_gradient,
   unsigned int _y_gradient,
   unsigned int _t_gradient,
   unsigned int _x_vect,
   unsigned int _y_vect,
   unsigned int _display,
   unsigned int _spu,
   int _w,
   int _h,
   int _src_w,
   int _src_h,
   int _x_off,
   int _y_off,
   int _dst_w,
   int _dst_h);

// Parameter for doing convolution

#define TILE_DX_DIM        4  // Tile dimension 

#define TILE_DY_DIM        4  // Tile dimension 

#define TILE_MAX_PAD       1  // Overlap region between tiles

#define TILE_MAX_KZ        3  // (TILE_MAX_PAD*2+1)

#define OF_MAX_INBUF       36 // (TILE_DX_DIM+2*TILE_MAX_PAD)*(TILE_DY_DIM+2*TILE_MAX_PAD)

#define OF_MAX_OUTBUF      16  // TILE_DX_DIM*TILE_DY_DIM

// Parameter for doing Lucas-Kanade windowing

#define OF1_TILE_DX_DIM    4  // Tile dimension 

#define OF1_TILE_DY_DIM    4  // Tile dimension

#define OF1_TILE_DX_IN_DIM 8  // OF1_TILE_DX_DIM+2*OF1_TILE_MAX_PAD 

#define OF1_TILE_DY_IN_DIM 8  // OF1_TILE_DY_DIM+2*OF1_TILE_MAX_PAD 

#define OF1_TILE_MAX_PAD   2  // Overlap region between tiles

#define OF1_TILE_MAX_KZ    5  // (OF1_TILE_MAX_PAD*2+1)

#define OF1_MAX_INBUF     64 // (OF1_TILE_DX_DIM+2*OF1_TILE_MAX_PAD)*(OF1_TILE_DY_DIM+2*OF1_TILE_MAX_PAD)

#define OF1_MAX_OUTBUF    16  // OF1_TILE_DX_DIM*OF1_TILE_DY_DIM

#ifdef __cplusplus
}
#endif
#endif
