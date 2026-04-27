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

// Each box will have a list of classes 
// Each box is assigned to a thread
// Number of classes that can be held by a thread at one time
#define CLASS_PER_THREAD 8

// Result is 2 words per box
// First word is the max score
// Second word is the class with highest score
#define RESULT_MAX_SCORE  0
#define RESULT_CLASS      1

extern void kernel_objdet_exe(
    unsigned int _req_id,
    unsigned int _score,
    unsigned int _score_result,
    unsigned int _class_result,
    int _numBoxes,
    int _numClasses
);

#ifdef __cplusplus
}
#endif
#endif
