//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <stdint.h>
#include <string.h>

typedef uintptr_t CLASSID;

extern void *cast_error(const char *errString);

#define CAST(myclass,myobj)   ((myobj)->isKindOf(myclass::getCLID())?(myclass *)myobj:(myclass *)cast_error("Invalid cast"))

#define DECLARE_ROOT_OBJECT(myclass)  \
                            private: static int M_clid; \
                            public: \
                            virtual bool isKindOf(CLASSID _clid) \
                            { \
                            if(myclass::getCLID()==_clid) \
                              return true; \
                            else \
                              return false; \
                            } \
                            static CLASSID getCLID() \
                            {  \
                              return (CLASSID)&M_clid; \
                            }

#define DECLARE_OBJECT(myclass,mybase)  \
                            private: static int M_clid; \
                            public: \
                            virtual bool isKindOf(CLASSID _clid) \
                            { \
                            if(myclass::getCLID()==_clid) \
                              return true; \
                            else \
                              return mybase::isKindOf(_clid); \
                            } \
                            static CLASSID getCLID() \
                            {  \
                              return (CLASSID)&M_clid; \
                            }

#define INSTANTIATE_OBJECT(myclass) int myclass::M_clid


#endif
