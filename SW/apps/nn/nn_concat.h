//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_CONCAT_H_
#define _DGRID_NN_CONCAT_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

class NeuralNetLayerConcat : public NeuralNetLayer {
public:
   NeuralNetLayerConcat(NeuralNet *nn,NeuralNetOperatorDef *def);
   ~NeuralNetLayerConcat();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return LayerIoTypeInInterleaveOutInterleave;}
private:
   static int16_t SpuEval(int16_t _in,void *pparm,uint32_t index,uint32_t parm2);
public:
   uint32_t m_outerSize;
   std::vector<size_t> m_copySize;
   std::vector<DGRID_SHARED_MEM> m_shmSpu;
private:
   uint32_t m_func;
};

#endif
