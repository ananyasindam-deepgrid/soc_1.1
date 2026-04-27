//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#ifndef __TARGET_BASE_GRAPH_H__
#define __TARGET_BASE_GRAPH_H__

#include "tensor.h"


#define GRAPH_MAX_INSTANCE 8

typedef bool (*GraphPollFunc)(void);

typedef enum {
   GraphNodeTypeProcessing, // Node that do the processing
   GraphNodeTypeSource // Node to represent input, must be first node of the graph
} GraphNodeType;

// A node in the execution graph
// This is a pure virtual class and to be implemented
// Graph node execution stages are as followed...
//    Create stage: Nodes are created with appropriate connections with other nodes.
//    Verify stage: Check input/output tensors. If tensors are undefined then initialize them
//    Prepare stage: Send commands to dgrid to perform the functions
//    Run stage: Run and wait for result to be completed.

class GraphNode {
public:
   GraphNode();
   virtual ~GraphNode(); 
   virtual DgridStatus Verify()=0; // Verify input/output/parameter
   virtual DgridStatus Execute(int queue,int stepMode)=0; // Schedule for execution.
   virtual GraphNodeType GetType();
public:
   static DgridStatus CheckResponse();
   bool AllRequestAreCompleted(int queue);
   uint32_t GetJobId(int queue);
};

// Graph is a container of GraphNode
// GraphNodes execution are coordinated by Graph

class Graph {
public:
   Graph();
   ~Graph();
   DgridStatus Clear();
   DgridStatus Add(GraphNode *node);
   DgridStatus Verify();
   DgridStatus Prepare();
   inline static bool Poll() {
      if(Graph::M_pollFunc)
         return (*M_pollFunc)();
      else
         return false;
   }
   inline DgridStatus Run(int timeout) {return run(timeout);}
   inline DgridStatus RunUntilCompletion() {
      DgridStatus rc;
	  for(;;) {
         rc=run(-1);
         if(rc==DgridStatusOk)
            return rc;
         if(rc != DgridStatusPending)
            return DgridStatusFail;
      }
   }
   static void SetPoll(GraphPollFunc pollFunc) {M_pollFunc=pollFunc;}
   bool IsRunning() {return m_nextNodeToSchedule >= 0;}
   void *GetOutputBuf(int _idx);
   int GetOutputBufLen(int _idx);
   int GetOutputDimension(int _idx,int _dimIdx);
   TENSOR *GetOutputTensor(int _idx);
   uint32_t GetElapsedTime() {return m_timeElapsed;}
private:
   DgridStatus run(int timeout);
private:
   std::vector<GraphNode *> m_nodes;
   int m_nextNodeToSchedule;
   uint32_t m_timeElapsed;
   int m_queue;
   static GraphPollFunc M_pollFunc;
public:
   uint32_t m_lastRequestId;
   uint32_t m_lastResponseId;
};

#endif
