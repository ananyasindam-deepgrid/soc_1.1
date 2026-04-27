//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

//--------------------------------------------------------------
// This file specifies tunable parameters to match
// the FPGA configuration
//--------------------------------------------------------------

#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

//--------------------------------------------------------------
// Max number of pcores
// This must match pid_gen_max_c defined in config.vhd
//    4 for small version
//    8 for large version
//--------------------------------------------------------------

//#define NUM_PCORE 4  

#define NUM_PCORE 10  

//---------------------------------------------------------------
// Max tensor size in log2
// Max tensor size = 2**MAX_TENSOR_LOG2_SIZE
// This must match MAX_TENSOR_LOG2_SIZE defined in config.vhd
//---------------------------------------------------------------

#define MAX_TENSOR_LOG2_SIZE  24 

//#define MAX_TENSOR_LOG2_SIZE  28 

//---------------------------------------------------------------
// Enable/disable FPU. This option is required to run LLM models
// This must match fpu_enabled_c defined on config.vhd
//----------------------------------------------------------------
 
//#define FPU_ENABLED TRUE

#define FPU_ENABLED FALSE

#endif
