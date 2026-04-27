//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <math.h>
#include "../../base/types.h"
#include "../../base/tensor.h"
#include "../../base/graph.h"
#include "kernels/harris.h"
#include "harris.h"

// Graph node to do harris-corder detection
// Refer to https://en.wikipedia.org/wiki/Harris_Corner_Detector

GraphNodeHarris::GraphNodeHarris() {
   m_x_gradient=0;
   m_y_gradient=0;
   m_score=0;
}

GraphNodeHarris::GraphNodeHarris(TENSOR *input,TENSOR *output) : GraphNodeHarris() {
   Create(input,output);
}

GraphNodeHarris::~GraphNodeHarris() {
   Cleanup();
}

DgridStatus GraphNodeHarris::Create(TENSOR *input,TENSOR *output) {
   Cleanup();
   m_input=input;
   m_output=output;
   return DgridStatusOk;
}


DgridStatus GraphNodeHarris::Verify() {
   if((*(m_input->GetDimension())).size() != 3)
      return DgridStatusFail;
   m_w=(*(m_input->GetDimension()))[2];
   m_h=(*(m_input->GetDimension()))[1];
   m_nChannel=(*(m_input->GetDimension()))[0];
   if(m_nChannel != 1)
      return DgridStatusFail;
   if(m_input->GetObjType() != TensorObjTypeMonochromeSingleChannel)
      return DgridStatusFail;
   
   m_x_gradient=dgridAllocSharedMem(m_w*m_h*sizeof(int16_t));
   m_y_gradient=dgridAllocSharedMem(m_w*m_h*sizeof(int16_t));
   m_score=dgridAllocSharedMem(m_w*m_h*sizeof(int16_t));
   std::vector<int> dim={m_h,m_w};
   m_output->Create(TensorDataTypeInt16,TensorFormatSplit,TensorObjTypeUnknown,dim);
   return DgridStatusOk;
}

DgridStatus GraphNodeHarris::Execute(int queue,int stepMode) {
   kernel_harris_exe(
      (unsigned int)GetJobId(queue),
	  (unsigned int)m_input->GetBuf(),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_x_gradient),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_y_gradient),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_score),
	  (unsigned int)m_output->GetBuf(),
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


void GraphNodeHarris::Cleanup() {
   if(m_x_gradient) {
      dgridFreeSharedMem(m_x_gradient);
      m_x_gradient=0;
   }
   if(m_y_gradient) {
      dgridFreeSharedMem(m_y_gradient);
      m_y_gradient=0;
   }
   if(m_score) {
      dgridFreeSharedMem(m_score);
      m_score=0;
   }
}
