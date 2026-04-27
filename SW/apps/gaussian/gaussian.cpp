//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <math.h>
#include "../../base/types.h"
#include "../../base/tensor.h"
#include "../../base/graph.h"
#include "kernels/gaussian.h"
#include "gaussian.h"

#define SIGMA 0.84089642
#define PI 3.14159265


// Graph node to do gaussian convolution on an image
// The effect is image blurring

GraphNodeGaussian::GraphNodeGaussian() {
   m_kernel=0;
}

GraphNodeGaussian::GraphNodeGaussian(TENSOR *input,TENSOR *output) : GraphNodeGaussian() {
   Create(input,output);
}

GraphNodeGaussian::~GraphNodeGaussian() {
   Cleanup();
}

DgridStatus GraphNodeGaussian::Create(TENSOR *input,TENSOR *output) {
   Cleanup();
   m_input=input;
   m_output=output;
   m_sigma=SIGMA;
   return DgridStatusOk;
}


DgridStatus GraphNodeGaussian::Verify() {
   if((*(m_input->GetDimension())).size() != 3)
      return DgridStatusFail;
   m_w=(*(m_input->GetDimension()))[2];
   m_h=(*(m_input->GetDimension()))[1];
   m_nChannel=(*(m_input->GetDimension()))[0];
   if(m_input->GetFormat() != TensorFormatSplit && m_nChannel > 1)
      return DgridStatusFail;
   m_ksz=7;
   m_kernel=dgridAllocSharedMem(m_ksz*m_ksz*sizeof(int16_t));
   BuildKernel();
   std::vector<int> dim={m_nChannel,m_h,m_w};
   m_output->Create(TensorDataTypeUint8,TensorFormatSplit,m_input->GetObjType(),dim);
   return DgridStatusOk;
}

DgridStatus GraphNodeGaussian::Execute(int queue,int stepMode) {
   kernel_gaussian_exe(
      (unsigned int)GetJobId(queue),
	  (unsigned int)m_input->GetBuf(),
	  (unsigned int)m_output->GetBuf(),
	  (unsigned int)DGRID_SHARED_MEM_VIRTUAL(m_kernel),
      m_nChannel,
      m_ksz,
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

float GraphNodeGaussian::gaussian(int x,int y,float sigma) {
   return (exp(-((x*x)+(y*y))/(2*sigma*sigma)))/(2*PI*sigma*sigma);
}

void GraphNodeGaussian::BuildKernel() {
   int16_t *kernel;
   kernel=(int16_t *)DGRID_SHARED_MEM_VIRTUAL(m_kernel);
   for(int y=0;y < m_ksz;y++) {
      for(int x=0;x < m_ksz;x++) {
         kernel[x+y*m_ksz]=(int)(gaussian(x-m_ksz/2,y-m_ksz/2,m_sigma)*1024);
      }
   }
}

void GraphNodeGaussian::SetSigma(float _sigma) {
   m_sigma = _sigma;
}

float GraphNodeGaussian::GetSigma() {
   return m_sigma;
}

void GraphNodeGaussian::Cleanup() {
   if(m_kernel) {
      dgridFreeSharedMem(m_kernel);
      m_kernel=0;
   }
}
