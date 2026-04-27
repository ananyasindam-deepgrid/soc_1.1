//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_ADD_H_
#define _DGRID_NN_ADD_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

class NeuralNetLayerAdd : public NeuralNetLayer {
public:
   NeuralNetLayerAdd(NeuralNet *nn,NeuralNetOperatorDef *def);
   ~NeuralNetLayerAdd();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return LayerIoTypeInFlatOutFlat;}
private:
   static int16_t SpuInputEval(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2);
   static int16_t SpuOutputEval(int16_t _in,void *pparm,uint32_t parm,uint32_t parm2);
public:
   DGRID_SHARED_MEM m_shmSpu;
private:
   uint32_t m_func;
};

#endif
