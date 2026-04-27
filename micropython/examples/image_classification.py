
#
# This example performs TensorFlowLite's Mobinet AI algorithm for image classification
# dgrid uses the same tflite model from TensorFlowLite without any retraining or adaptation.
# Execution graph:
#   1. Copy webcam input to t1. Reformat t1 to format RR..GG..BB 
#   2. Resize t1 to dimension 224x224. Mobinet expects this input size. 
#   3. Perform Mobinet Image Classification algo.
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
n2=dgrid.GraphNodeResize(t1,t2,224,224)
n3=dgrid.GraphNodeImageClassifier(t2)
n4=dgrid.GraphNodeCopyAndTransform(tensorInput,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3,n4)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       top5=n3.GetTop5()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       r=16
       for top5Item in top5:
          dgrid.CanvasDrawText('{} 0.{:02d} '.format(top5Item[1],(top5Item[0]*100)>>8),r,0)
          r+=16
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

