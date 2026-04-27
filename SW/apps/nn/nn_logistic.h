//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_LOGISTIC_H_
#define _DGRID_NN_LOGISTIC_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

class NeuralNetLayerLogistic : public NeuralNetLayer {
public:
   NeuralNetLayerLogistic(NeuralNet *nn,NeuralNetOperatorDef *def);
   ~NeuralNetLayerLogistic();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return LayerIoTypeInOutSame;}
private:
   static int16_t SpuEval(int16_t _in,void *,uint32_t,uint32_t);
   static int16_t SpuEvalScale(int16_t _in,void *,uint32_t,uint32_t);
public:
   DGRID_SHARED_MEM m_shmSpu;
private:
   uint32_t m_func;
};

#endif
