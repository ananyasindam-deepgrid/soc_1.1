//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _KERNEL_LLM_P_H_
#define _KERNEL_LLM_P_H_
#ifdef __cplusplus
extern "C" { 
#endif

// Maximum quantization group supported

#define GS_DEFAULT 32 // Default quantization group

#define LLM_GS 8 // LLM_GS==VECTOR_WIDTH

#define LLM_GS_FACTOR 4 // GS_DEFAULT/LLM_GS


#ifdef __cplusplus
}
#endif
#endif
