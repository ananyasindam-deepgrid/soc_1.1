//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include "dgridlib.h"
#include "../src/soc.h"
#include "tensor.h"
#include "graph.h"

// Execution graph object

static Graph *M_graphLst[GRAPH_MAX_INSTANCE]={0,0};

GraphPollFunc Graph::M_pollFunc=0;

Graph::Graph() {
   int queue;
   for(queue=0;queue < GRAPH_MAX_INSTANCE;queue++) {
      if(!M_graphLst[queue])
         break;
   }
   assert(queue < GRAPH_MAX_INSTANCE);
   assert(!M_graphLst[queue]);
   M_graphLst[queue]=this;
   m_nextNodeToSchedule=-1;
   m_timeElapsed=0;
   m_queue=queue;
   m_lastRequestId=0;
   m_lastResponseId=0xffffffff;
}

Graph::~Graph() {
   M_graphLst[m_queue]=0;
}

// Clear all the nodes from graph

DgridStatus Graph::Clear() {
   m_nextNodeToSchedule=-1;
   m_nodes.clear();
   return DgridStatusOk;
}

// Add a GraphNode to the Graph
// Execution order of GraphNodes are same order as they are being added
// to Graph...

DgridStatus Graph::Add(GraphNode *node) {
   m_nodes.push_back(node);
   return DgridStatusOk;
}

// Verify the graph
// Verify each nodes in the graph

DgridStatus Graph::Verify() {
   for(int i=0;i < (int)m_nodes.size();i++) {
      if(m_nodes[i]->Verify() != DgridStatusOk)
         return DgridStatusFail;
   }
   return DgridStatusOk;
}

// Prepare the graph nodes for execution...
// GraphNodes are scheduled in the order they are pushed to the
// Graph

DgridStatus Graph::Prepare() {
   if(m_nextNodeToSchedule < 0) {
      // Begin of a scheduling...
      m_nextNodeToSchedule=0;
      return DgridStatusOk;
   } else {
      return DgridStatusFail;
   }
}

// Execute graph

DgridStatus Graph::run(int timeout) {
   DgridStatus rc;
   if(m_nextNodeToSchedule < 0)
      return DgridStatusOk;
   while(m_nextNodeToSchedule < (int)m_nodes.size()) {
      rc=m_nodes[m_nextNodeToSchedule]->Execute(m_queue,timeout);
      if(rc==DgridStatusPending)
         break;
      if(rc!=DgridStatusOk)
         return rc;
      m_nextNodeToSchedule++;
      if(timeout>=0)
         break;
   }
   GraphNode::CheckResponse();
   if((m_lastResponseId==m_lastRequestId) &&
      (m_nextNodeToSchedule >= (int)m_nodes.size())) {
      // Done...
      m_nextNodeToSchedule=-1;
      m_timeElapsed = 0;
      return DgridStatusOk;
   } else
      return DgridStatusPending;
}

GraphNode::GraphNode() {
}

GraphNode::~GraphNode() {
}

GraphNodeType GraphNode::GetType() {
   return GraphNodeTypeProcessing;
}

// Running the graph.
// Check response queue for any available responses.


DgridStatus GraphNode::CheckResponse() {
   int queue;
   uint32_t resp;
   // Wait for response....
   while(dgridReadResponse(&resp)) {
	   queue=(resp>>24);
	   resp=(resp&0xFFFFFF);
	   assert(queue < GRAPH_MAX_INSTANCE);
	   M_graphLst[queue]->m_lastResponseId=resp;
   }
   return DgridStatusOk;
}

// Allocate and return the next request id

uint32_t GraphNode::GetJobId(int queue) {
   Graph *g=M_graphLst[queue];
   g->m_lastRequestId++;
   if((g->m_lastRequestId & 0xFF000000) != 0)
      g->m_lastRequestId=0;
   return g->m_lastRequestId+(queue<<24);
}

bool GraphNode::AllRequestAreCompleted(int queue) {
   Graph *g=M_graphLst[queue];
   return g->m_lastRequestId==g->m_lastResponseId;
}

