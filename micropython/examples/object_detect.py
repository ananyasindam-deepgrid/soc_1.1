#
# This example performs TensorFlowLite's SSD-Mobinet AI algorithm for object detection
# dgrid uses the same tflite model from TensorFlowLite without any retraining or adaptation.
# Execution graph:
#   1. Copy webcam input to t1. Reformat t1 to format RR..GG..BB 
#   2. Resize t1 to dimension 300x300. SSD-Mobinet expects this input size. 
#   3. Perform SSD-Mobinet object detection algo.
#   4. Copy webcam input to display. Display has format RGBRGB...
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#

import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
t1 = dgrid.Tensor()
t2 = dgrid.Tensor()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,t1,dgrid.COLOR,dgrid.PLANAR)
n2=dgrid.GraphNodeResize(t1,t2,300,300)
n3=dgrid.GraphNodeObjectDetection(t2)
n4=dgrid.GraphNodeCopyAndTransform(tensorInput,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3,n4)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       objects=n3.GetObjects()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       for object in objects:
          dgrid.CanvasDrawText(object[5],object[1],object[0])
          dgrid.CanvasDrawRectangle((object[1],object[0]),(object[3],object[2]))
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()



