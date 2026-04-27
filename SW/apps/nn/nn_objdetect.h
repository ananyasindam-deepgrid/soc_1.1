//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _DGRID_NN_OBJDETECT_H_
#define _DGRID_NN_OBJDETECT_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "../../base/types.h"
#include "../../base/util.h"
#include "../../base/dgridlib.h"
#include "nn.h"

#define dNnObjDetectThreshold ((float)0.5)

#define dNnObjDetectIouThreshold ((float)0.6)

#define dNnObjDetectMaxNumDetects 10

struct ObjDetectionResult {
   float xmin;
   float ymin;
   float xmax;
   float ymax;
   int detectClass;
   int score;
};

class NeuralNetLayerObjDetect : public NeuralNetLayer {
public:
   NeuralNetLayerObjDetect(NeuralNet *nn,NeuralNetOperatorDef *def);
   ~NeuralNetLayerObjDetect();
   DgridStatus Prepare();
   DgridStatus Evaluate(int queue);
   LayerIoType GetIoType() {return LayerIoTypeInInterleaveOutInterleave;}
   virtual bool RunAtHost() {return true;}
public:
   int m_maxNumDetects;
private:
   float m_lookup_score[256];
   float m_lookup_box_x[256];
   float m_lookup_box_y[256];
   float m_lookup_box_h[256];
   float m_lookup_box_w[256];
   size_t m_boxesSize;
   size_t m_classesSize;
   float *m_anchors;
   DGRID_SHARED_MEM m_scoreResult;
   DGRID_SHARED_MEM m_classResult;
   int m_score_threshold;
};

#endif
