//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _TARGET_APPS_OF_OF_H_
#define _TARGET_APPS_OF_OF_H_

#include "../../base/tensor.h"
#include "../../base/graph.h"

// Graph node to do optical flow using Lucas-Kanade algorithm
// https://en.wikipedia.org/wiki/Lucas%E2%80%93Kanade_method

class GraphNodeOpticalFlow : public GraphNode {
public:
   GraphNodeOpticalFlow();
   GraphNodeOpticalFlow(TENSOR *input1,
                        TENSOR *x_gradient,
                        TENSOR *y_gradient,
                        TENSOR *t_gradient,
                        TENSOR *x_vect,
                        TENSOR *y_vect,
                        TENSOR *display);
   virtual ~GraphNodeOpticalFlow();
   DgridStatus Create(TENSOR *input1,
                    TENSOR *x_gradient,
                    TENSOR *y_gradient,
                    TENSOR *t_gradient,
                    TENSOR *x_vect,
                    TENSOR *y_vect,
                    TENSOR *display);
   virtual DgridStatus Verify();
   virtual DgridStatus Execute(int queue,int stepMode);
private:
   void Cleanup();
   static int16_t SpuCallback(int16_t _in,void *pparm,uint32_t parm);
   static int16_t SpuDisplayLeftHorizontalCallback(int16_t _in,void *pparm,uint32_t parm);
   static int16_t SpuDisplayRightHorizontalCallback(int16_t _in,void *pparm,uint32_t parm);
   static int16_t SpuDisplayVerticalCallback(int16_t _in,void *pparm,uint32_t parm);
private:
   TENSOR *m_input1;
   TENSOR *m_x_gradient;
   TENSOR *m_y_gradient;
   TENSOR *m_t_gradient;
   TENSOR *m_x_vect;
   TENSOR *m_y_vect;
   TENSOR *m_display;
   int m_w;
   int m_h;
   int m_nChannel;
   uint32_t m_func;
   DGRID_SHARED_MEM m_spu;
   TENSOR m_buffer[2];
   int m_bufferHead;
};

#endif
