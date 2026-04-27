#
# This example performs Harris-Corner detection algorithm to find point of interests.
# Execution graph:
#   1. Copy webcam input to t1. During copying, reformat to output format MMM... where M=MonoValue(R,G,B) 
#   2. Perform Harris-Corner detection on t1. This algo expects monochrome input
#   3. Copy webcam input to display
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#


import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
t1 = dgrid.Tensor()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,t1,dgrid.MONO1,dgrid.PLANAR)
n2=dgrid.GraphNodeHarris(t1)
n3=dgrid.GraphNodeCopyAndTransform(tensorInput,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       points=n2.GetPOI()
       for point in points:
          dgrid.CanvasDrawPoint(point[1],point[0])
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

