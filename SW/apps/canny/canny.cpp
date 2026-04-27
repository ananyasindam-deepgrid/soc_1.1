//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <math.h>
#include "../../base/types.h"
#include "../../base/tensor.h"
#include "../../base/graph.h"
#include "kernels/canny.h"
#include "canny.h"


// Graph node to do edge detection using Canny algorithm
// Refer to https://en.wikipedia.org/wiki/Canny_edge_detector

GraphNodeCanny::GraphNodeCanny() {
   m_magnitude=0;
   m_phase=0;
   m_maxima=0;
   m_thresholdLo=81;
   m_thresholdHi=163;
}

GraphNodeCanny::GraphNodeCanny(TENSOR *input,TENSOR *output) : GraphNodeCanny() {
   m_magnitude=0;
   m_phase=0;
   m_maxima=0;
   m_thresholdLo=81;
   m_thresholdHi=163;
   Create(input,output);
}

GraphNodeCanny::~GraphNodeCanny() {
   Cleanup();
}

DgridStatus GraphNodeCanny::Create(TENSOR *input,TENSOR *output) {
   Cleanup();
   m_input=input;
   m_output=output;
   return DgridStatusOk;
}


DgridStatus GraphNodeCanny::Verify() {
   int sz;
      
   if((*(m_input->GetDimension())).size() != 3)
      return DgridStatusFail;
   m_w=(*(m_input->GetDimension()))[2];
   m_h=(*(m_input->GetDimension()))[1];
   m_nChannel=(*(m_input->GetDimension()))[0];
   if(m_nChannel != 1)
      return DgridStatusFail;

   sz=(m_w+2*TILE_MAX_KZ)*(m_h+2*TILE_MAX_KZ);
   m_magnitude=dgridAllocSharedMem(sz*sizeof(int16_t));
   m_phase=dgridAllocSharedMem(sz*sizeof(uint8_t));
   m_maxima=dgridAllocSharedMem(sz*sizeof(int16_t));
//   m_thresholdHi=100;
   std::vector<int> dim={1,m_h,m_w};
   m_output->Create(TensorDataTypeUint8,TensorFormatSplit,TensorObjTypeMonochromeSingleChannel,dim);
   return DgridStatusOk;
}

DgridStatus GraphNodeCanny::Execute(int queue,int stepMode) {
   kernel_canny_exe(
      (unsigned int)GetJobId(queue),
      (unsigned int)m_input->GetBuf(),
      (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_magnitude),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_phase),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_maxima),
	  (unsigned int)m_output->GetBuf(),
      m_thresholdLo,
      m_thresholdHi,
      m_w,
      m_h,
      m_w,
      m_h,
      0,
      0,
      m_w,
      m_h);
   return DgridStatusOk;
}

void GraphNodeCanny::Cleanup() {
   if(m_magnitude) {
      dgridFreeSharedMem(m_magnitude);
      m_magnitude=0;
   }
   if(m_phase) {
      dgridFreeSharedMem(m_phase);
      m_phase=0;
   }
   if(m_maxima) {
      dgridFreeSharedMem(m_maxima);
      m_maxima=0;
   }
}

// Set contrast threshold

void GraphNodeCanny::SetThreshold(int _loThreshold,int _hiThreshold) {
   m_thresholdLo=_loThreshold;
   m_thresholdHi=_hiThreshold;
}

// Get current contrast threshold

void GraphNodeCanny::GetThreshold(int *_loThreshold,int *_hiThreshold) {
   *_loThreshold=m_thresholdLo;
   *_hiThreshold=m_thresholdHi;
}
 
