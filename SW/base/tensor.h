//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef __TARGET_BASE_TENSOR_H__
#define __TARGET_BASE_TENSOR_H__

#include <stdint.h>
#include <vector>
#include "types.h"
#include "dgridlib.h"

// Tensor data types

typedef enum {
   TensorDataTypeInt8,
   TensorDataTypeUint8,
   TensorDataTypeInt16,
   TensorDataTypeUint16,
   TensorDataTypeFloat32,
   TensorDataTypeFloat16
} TensorDataType;

// Tensor format.
// Tensor outermost dimension may be stored in an interleave format
// Tensor outermost dimension may be stored in a regular array format (split)

typedef enum {
   TensorFormatInterleaved,
   TensorFormatSplit
} TensorFormat;

// Tensor data meaning...

typedef enum {
   TensorObjTypeRGB, // Image with pixel color in RGB order
   TensorObjTypeBGR, // Image with pixel color in BGR order
   TensorObjTypeYUYV, // Image with YUYV pixel (2bytes per pixel)
   TensorObjTypeMonochrome, // Image monochrome but with 3 color plane (R=G=B)
   TensorObjTypeMonochromeSingleChannel, // Image monochrome but 1 color plane
   TensorObjTypeUnknown // Unknown meaning
} TensorObjType;

class TENSOR {
public:
   TENSOR();
   TENSOR(TensorDataType _dataType,TensorFormat _fmt,TensorObjType _objType,int numDim,...);
   DgridStatus Create(TensorDataType _dataType,TensorFormat _fmt,TensorObjType _objType,std::vector<int> &dim,DGRID_SHARED_MEM _shm=0);
   DgridStatus Clone(TENSOR *other);
   DgridStatus Alias(TENSOR *other);
   DgridStatus Alias(DGRID_SHARED_MEM _shm);
   DgridStatus CreateWithBitmap(const char *bmpFile,TensorFormat fmt=TensorFormatSplit);
   ~TENSOR();
   TensorDataType GetDataType() {return m_dataType;}
   TensorFormat GetFormat() {return m_fmt;}
   TensorObjType GetObjType() {return m_objType;}
   std::vector<int> *GetDimension() {return &m_dim;}
   int GetDimension(int _idx) {return m_dim[_idx];}
   DGRID_SHARED_MEM GetBufShm() {return m_shm;}
   void *GetBuf() {return m_buf;}
   DGRID_SHARED_MEM GetShm() {return m_shm;}
   int GetBufLen() {return m_size;}
   static size_t GetTensorSize(std::vector<int>& shape);
public:
   std::vector<int> m_dim;
private:
   DgridStatus setDataType(TensorDataType _dataType);
   DgridStatus setFormat(TensorFormat fmt);
   DgridStatus setObjType(TensorObjType _objType);
   DgridStatus setDimension(std::vector<int> &dim);
   DgridStatus allocate(DGRID_SHARED_MEM shm=0);
private:
   DGRID_SHARED_MEM m_shm;
   bool m_isAlias;
   TensorDataType m_dataType;
   int m_dataElementLen;
   TensorFormat m_fmt;
   TensorObjType m_objType;
   void *m_buf;
   int m_size;
};

#endif
