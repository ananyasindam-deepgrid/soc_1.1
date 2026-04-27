//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef APPS_EQUALIZE_KERNELS_EQUALIZE_H_
#define APPS_EQUALIZE_KERNELS_EQUALIZE_H_
#ifdef __cplusplus
extern "C" {
#endif

extern void kernel_equalize_exe(
   unsigned int _req_id,
   unsigned int _input,
   unsigned int _output,
   unsigned int _output2,
   int _nchannels,
   unsigned int _equalize,
   int _w,
   int _h
   );

#define kHistogramInSize   8  // Number of pixels to be processed per thread
#define kHistogramBinSize  4  // Size of histogram bin in vector unit
#define kHistogramBinBit   3  // Number of bit to shift data to get index into histogram bin 

#define HISTOGRAM_HI_FACTOR 1000 // Multiplication factor for histogram hi value 

#ifdef __cplusplus
}
#endif
#endif
