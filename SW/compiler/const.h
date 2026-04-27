//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _CONST_H_
#define _CONST_H_
#include <vector>
#include "object.h"
#include "gen.h"
#include "ident.h"

class cConstant
{
public:
   static int Init();
   static RETCODE Allocate(cAstNode *_root);
   static int Find(float constant);
   static int Add(float constant);
   static int Size();
   static void Gen(FILE *fp,std::vector<uint16_t> &img);

   static std::vector<float> M_space;
};

#endif

