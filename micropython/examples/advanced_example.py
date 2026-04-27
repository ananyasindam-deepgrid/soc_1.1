#
# This is an advanced example, it is recommended to you familiarize with other examples first
# before learning this example.
# This example performs multiple vision tasks and display the results on different
# region of the display. Display is partitioned into 4 tiles to display the results of...
#   - Object detection with SSD-Mobinet (top-left display tile)
#   - Edge detection (top-right display tile)
#   - Harris-Corner for point of interests (bottom-left display tile)
#   - OpticalFlow (bottom-right display tile)
# Reference on other examples on how the graph is setup for each of the tasks above.
# This example also demonstrates the use of multi graph processing. There are 2 graphs...
# Graph1:
#    graph1 is performing faster,non-AI tasks such as edge-detetion,harris-corner and optical-flow 
#    and this graph is allowed to run till completion
# Graph2:
#    graph2 is performing slower AI task SSD-Mobinet, it is allowed to run only upto 40msec before it is paused 
#    and to be resumed later. This allows for graph1 to run on new image without having to wait for graph2 
#    to complete first.
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#

import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
t1=dgrid.Tensor()
t2=dgrid.Tensor()
t3=dgrid.Tensor()
t4=dgrid.Tensor()
t5=dgrid.Tensor()
t6=dgrid.Tensor()
t7=dgrid.Tensor()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,t1,dgrid.COLOR,dgrid.PLANAR);
n2=dgrid.GraphNodeResize(t1,t2,320,240);
n3=dgrid.GraphNodeCopyAndTransform(t2,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED,0,0) # Display @ r=0,c=0
n4=dgrid.GraphNodeCopyAndTransform(t2,t3,dgrid.MONO1,dgrid.PLANAR);
n5=dgrid.GraphNodeCanny(t3,t4);
n6=dgrid.GraphNodeCopyAndTransform(t4,tensorOutput,dgrid.MONO3,dgrid.INTERLEAVED,0,320) # Display @ r=0,c=320
n7=dgrid.GraphNodeHarris(t3)
n8=dgrid.GraphNodeCopyAndTransform(t2,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED,240,0) # Display @ r=240,c=0
n9=dgrid.GraphNodeOpticalFlow(t3,t5)
n10=dgrid.GraphNodeCopyAndTransform(t5,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED,240,320) # Display @ r=240,c=320
n11=dgrid.GraphNodeCopyAndTransform(tensorInput,t6,dgrid.COLOR,dgrid.PLANAR);
n12=dgrid.GraphNodeResize(t6,t7,300,300)
n13=dgrid.GraphNodeObjectDetection(t7)
graph=dgrid.Graph(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10)
graphNN=dgrid.Graph(n11,n12,n13)
objectsValid=False
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       points=n7.GetPOI()
       for point in points:
          dgrid.CanvasDrawPoint(point[1]+240,point[0])
       if(objectsValid) :
          for object in objects:
             dgrid.CanvasDrawText(object[5],object[1]>>1,object[0]>>1)
             dgrid.CanvasDrawRectangle((object[1]>>1,object[0]>>1),(object[3]>>1,object[2]>>1))
       dgrid.DisplayFlushCanvas()
   graphNN.RunWithTimeout(40)
   if(graphNN.IsBusy()==False):
       objects=n13.GetObjects()
       objectsValid=True
dgrid.DeleteAll()

