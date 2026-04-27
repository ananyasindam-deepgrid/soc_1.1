//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_RESHAPE_H_
#define _DGRID_NN_RESHAPE_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

class NeuralNetLayerReshape : public NeuralNetLayer {
public:
   NeuralNetLayerReshape(NeuralNet *nn,NeuralNetOperatorDef *def);
   ~NeuralNetLayerReshape();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return LayerIoPassthrough;}
};

#endif
