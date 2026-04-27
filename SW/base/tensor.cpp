//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <stdarg.h>
#include <malloc.h>
#include "types.h"
#include "dgridlib.h"
#include "util.h"
#include "tensor.h"

// Default constructor.
// Tensor definition/allocation to be provided later

TENSOR::TENSOR() {
   m_shm=0;
   m_isAlias=false;
   m_dataType=TensorDataTypeUint8;
   m_dataElementLen=1;
   m_fmt=TensorFormatSplit;
   m_objType=TensorObjTypeUnknown;
   m_buf=0;
   m_size=0;
}

// Create a fully defined tensor.
// Also allocate memory for it

TENSOR::TENSOR(TensorDataType _dataType,TensorFormat _fmt,TensorObjType _objType,int numDim,...):TENSOR() {
   va_list args;
   int v;

   setDataType(_dataType);
   setFormat(_fmt);
   setObjType(_objType);
   va_start(args,numDim);
   for(int i=0;i < numDim;i++) {
      v=va_arg(args,int);
      m_dim.push_back(v);
   }
   m_size=TENSOR::GetTensorSize(m_dim)*m_dataElementLen;
   allocate();
   va_end(args);
}

// Create tensor. Free previous allocation
// If _shm specified, tensor buffer has been created already so just reference it directly
// Otherwise allocate new buffer

DgridStatus TENSOR::Create(TensorDataType _dataType,TensorFormat _fmt,TensorObjType _objType,std::vector<int> &dim,
                        DGRID_SHARED_MEM _shm) {
   if(m_shm && !m_isAlias) {
      dgridFreeSharedMem(m_shm);
   }
   m_shm=0;
   m_isAlias=false;
   setDataType(_dataType);
   setFormat(_fmt);
   setObjType(_objType);
   setDimension(dim);
   if(_shm)
      allocate(_shm);
   else
      allocate();
   return DgridStatusOk;
}

DgridStatus TENSOR::Clone(TENSOR *other) {
   return Create(other->GetDataType(),other->GetFormat(),other->GetObjType(),other->m_dim);
}

DgridStatus TENSOR::CreateWithBitmap(const char *bmpFile,TensorFormat fmt)
{
   uint8_t *pict;
   int bmp_w,bmp_h;
   int bmpActualWidth;
   int r,c;
   int w,h;
   int dx,dy;
   uint8_t *output;

   pict = bmpRead(bmpFile,&bmp_h,&bmp_w);
   if(!pict) {
      return DgridStatusFail;
   }
   if(fmt==TensorFormatSplit) {
      std::vector<int> dim={3,bmp_h,bmp_w};
      Create(TensorDataTypeUint8,TensorFormatSplit,TensorObjTypeRGB,dim);
   } else {
      std::vector<int> dim={bmp_h,3*bmp_w};
      Create(TensorDataTypeUint8,TensorFormatSplit,TensorObjTypeRGB,dim);
   }
   output=(uint8_t *)GetBuf();

   w=bmp_w;
   h=bmp_h;
   dx=0;
   dy=0;
   bmpActualWidth=((bmp_w*3+3)/4)*4;
   uint8_t red, blue, green;
   for (r = 0; r < h; r++) {
      for (c = 0; c < w; c++) {
         blue = (pict[((bmp_h-1)-(r+dy))*bmpActualWidth+3*(c+dx)+0]);
         green = (pict[((bmp_h-1)-(r+dy))*bmpActualWidth+3*(c+dx)+1]);
         red = (pict[((bmp_h-1)-(r+dy))*bmpActualWidth+3*(c+dx)+2]);
         if(fmt==TensorFormatSplit) {
            output[0*w*h+r*w+c] = red;
            output[1*w*h+r*w+c] = green;
            output[2*w*h+r*w+c] = blue;
         } else {
             output[3*(r*w+c)] = red;
             output[3*(r*w+c)+1] = green;
             output[3*(r*w+c)+2] = blue;
         }
      }
   }
   free(pict);
   return DgridStatusOk;
}

TENSOR::~TENSOR() {
   if(m_shm && !m_isAlias)
      dgridFreeSharedMem(m_shm);
}

DgridStatus TENSOR::setDataType(TensorDataType _dataType) {
   m_dataType=_dataType;
   switch(m_dataType) {
      case TensorDataTypeInt8:
      case TensorDataTypeUint8:
         m_dataElementLen=1;
         break;
      case TensorDataTypeInt16:
      case TensorDataTypeUint16:
         m_dataElementLen=2;
         break;
      case TensorDataTypeFloat32:
         m_dataElementLen=4;
         break;
      default:
         assert(0);
   }
   return DgridStatusOk;
}

DgridStatus TENSOR::setObjType(TensorObjType _objType) {
   m_objType=_objType;
   return DgridStatusOk;
}

DgridStatus TENSOR::setFormat(TensorFormat fmt) {
   m_fmt=fmt;
   return DgridStatusOk;
}

DgridStatus TENSOR::setDimension(std::vector<int> &_dim) {
   m_dim.clear();
   m_dim=_dim;
   m_size = TENSOR::GetTensorSize(m_dim)*m_dataElementLen;
   return DgridStatusOk;
}

DgridStatus TENSOR::allocate(DGRID_SHARED_MEM shm) {
   assert(m_size < (1<<(DP_ADDR_WIDTH-1))); // Tensor must be less than half tensor address dynamic range
   if(m_shm && !m_isAlias)
      dgridFreeSharedMem(m_shm);
   m_isAlias=false;
   if(shm) {
      m_shm=shm;
   } else {
      m_shm=dgridAllocSharedMem(m_size);
   }
   m_buf=DGRID_SHARED_MEM_VIRTUAL(m_shm);
   return DgridStatusOk;
}

// Set this tensor as an alias for another buffer

DgridStatus TENSOR::Alias(TENSOR *other) {
   if(m_shm && !m_isAlias)
      dgridFreeSharedMem(m_shm);
   m_shm=other->GetShm();
   m_isAlias=true;
   m_buf=DGRID_SHARED_MEM_VIRTUAL(m_shm);
   assert(other->m_size==m_size);
   return DgridStatusOk;
}

DgridStatus TENSOR::Alias(DGRID_SHARED_MEM _shm) {
   if(m_shm && !m_isAlias)
      dgridFreeSharedMem(m_shm);
   m_shm=_shm;
   m_isAlias=true;
   m_buf=DGRID_SHARED_MEM_VIRTUAL(m_shm);
   return DgridStatusOk;
}

// Return total tensor array size

size_t TENSOR::GetTensorSize(std::vector<int>& shape) {
   size_t sz=1;
   for(int i=0;i < (int)shape.size();i++) {
      sz*=shape[i];
   }
   return sz;
}

