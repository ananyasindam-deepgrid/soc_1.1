//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_CONV2D_H_
#define _DGRID_NN_CONV2D_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

enum ConvolutionType {
   ConvolutionType2D,
   ConvolutionTypeDepthWise
};

class NeuralNetLayerConv2D : public NeuralNetLayer {
public:
   NeuralNetLayerConv2D(NeuralNet *nn,NeuralNetOperatorDef *def,ConvolutionType _type);
   ~NeuralNetLayerConv2D();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return (m_type==ConvolutionTypeDepthWise)?LayerIoTypeInInterleaveOutInterleaveAndOrFlat:LayerIoTypeInFlatOutInterleaveAndOrFlat;}
private:
   static int16_t SpuEvalActivation(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2);
   static int16_t SpuEvalInput(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2);
   static int16_t SpuEvalFilter(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2);
   DgridStatus ConvolutionStrategy(int topcnt, int topdim, int botcnt, int botdim,int ksz,int stride,int num_pcore,
                                 int *conv_dx,int *conv_dycnt,int *conv_groupsz,
                                 int max_conv_dx,int max_conv_dy,int max_dycnt);
   DGRID_SHARED_MEM GenConvolutionWeight(uint8_t *_coef,std::vector<int> &shape,int topcnt,int botcnt,int kz);
   DGRID_SHARED_MEM GenFcWeight(uint8_t *_coef,int _topcnt,int _botcnt,int *coef_dim,int *_nthread,int *_npcore);
   void GenBias(int32_t *bias,int biasLen,int32_t activationBias,DGRID_SHARED_MEM *shmHi,DGRID_SHARED_MEM *shmLo);
public:
   ConvolutionType m_type;
   DGRID_SHARED_MEM m_shmFilter;
   DGRID_SHARED_MEM m_shmBiasHi;
   DGRID_SHARED_MEM m_shmBiasLo;
   DGRID_SHARED_MEM m_shmSpu;
   union {
      struct {
         int dx;
         int dycnt;
         int groupsz;
      } conv;
      struct {
         int nthread;
         int npcore;
         int coef_dim[3];
      } fcn;
   } m_strategy;
private:
   uint32_t m_fcnFunc;
   uint32_t m_convFunc;
   uint32_t m_convDepthwiseFunc;
};

#endif
