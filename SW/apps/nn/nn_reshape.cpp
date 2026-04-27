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
#include "nn_reshape.h"

// Reshape layer. Nothing to do
NeuralNetLayerReshape::NeuralNetLayerReshape(NeuralNet *nn,NeuralNetOperatorDef* def) : NeuralNetLayer(nn,def) {
}

NeuralNetLayerReshape::~NeuralNetLayerReshape() {
}

DgridStatus NeuralNetLayerReshape::Prepare() {
   return DgridStatusOk;
}

DgridStatus NeuralNetLayerReshape::Evaluate(int queue) {
   return DgridStatusOk;
}

