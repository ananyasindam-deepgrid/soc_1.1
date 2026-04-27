//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "kernels/fcn.h"
#include "nn_poolavg.h"

// Do pool average layer

NeuralNetLayerPoolAvg::NeuralNetLayerPoolAvg(NeuralNet *nn,NeuralNetOperatorDef* def) : NeuralNetLayer(nn,def) {
}

NeuralNetLayerPoolAvg::~NeuralNetLayerPoolAvg() {
}

DgridStatus NeuralNetLayerPoolAvg::Prepare() {
   m_shmSpu=dgridBuildSpuBundle(1,SpuAvgPool,this,0,0);
   m_nn->BufferAllocate(m_shmSpu);
   return DgridStatusOk;
}

DgridStatus NeuralNetLayerPoolAvg::Evaluate(int queue) {
   NeuralNetOperatorDef *op=&m_def;
   bool interleave=(m_nn->BufferGetInterleave(op->output[0])!=0);
   kernel_Pooling_exe(
      (unsigned int)GetJobId(queue),
      (unsigned int)(interleave?m_nn->BufferGetInterleave(op->input[0]):m_nn->BufferGetFlat(op->input[0])),
	  (unsigned int)(interleave?m_nn->BufferGetInterleave(op->output[0]):m_nn->BufferGetFlat(op->output[0])),
      op->u.pool_avg.filter_w,
      op->u.pool_avg.stride_w,
      (*op->output_shape[0])[3],
      (*op->output_shape[0])[2],
      (*op->input_shape[0])[3],
      (*op->input_shape[0])[2],
	  (unsigned int)m_shmSpu,
      m_outputShift);
   return DgridStatusOk;
}

int16_t NeuralNetLayerPoolAvg::SpuAvgPool(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2)
{
   NeuralNetLayer *layer=static_cast<NeuralNetLayer *>(pparm);
   NeuralNetOperatorDef *op=layer?&((NeuralNetLayerPoolAvg *)layer)->m_def:0;
   static float D=0.0;
   static float N=0.0;
   static int activation_max=0;
   static int activation_min=0;
   if(op) {
      int cnt,bit=0;
      cnt=op->u.pool_avg.filter_w*op->u.pool_avg.filter_h;
      while(cnt > 0) {
         cnt=cnt>>1;
         bit++;
      }
      if(bit > 2) {
         bit -= 2;
      } else if(bit > 1) {
         bit -= 1;
      }
      D=static_cast<float>(op->u.pool_avg.filter_w*op->u.pool_avg.filter_h);
      N=static_cast<float>((1<<bit));
      activation_max=op->u.pool_avg.activation_max;
      activation_min=op->u.pool_avg.activation_min;
      ((NeuralNetLayerPoolAvg *)layer)->m_outputShift=bit;
   }
   float _in2;
   _in2=static_cast<float>((((float)_in*(float)N)/(float)D)+0.5);
   if(_in2 > activation_max) {
      return static_cast<int16_t>(activation_max);
   } else if(_in2 < activation_min) {
      return static_cast<int16_t>(activation_min);
   } else {
      return FLOAT2INT(_in2);
   }
}  

