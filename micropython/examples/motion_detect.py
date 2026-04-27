#
# This example performs optical-flow algorithm to detect motion
# Execution graph:
#   1. Copy webcam input to t1. During copying, reformat to output format MMM... where M=MonoValue(R,G,B) 
#   2. Perform OpticalFlow algo on t1 and result to t2. Motion information is encoded as colored pixel on t2.
#   3. Copy t2 to display. Display has format RGBRGB...
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#


import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
t1 = dgrid.Tensor()
t2= dgrid.Tensor()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,t1,dgrid.MONO1,dgrid.PLANAR)
n2=dgrid.GraphNodeOpticalFlow(t1,t2)
n3=dgrid.GraphNodeCopyAndTransform(t2,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

