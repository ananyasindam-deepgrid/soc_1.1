#
# This example performs gaussian blurring algorithm
# Execution graph:
#   1. Copy webcam input to t1. During copying, reformat to output format RR..GG..BB.. 
#   2. Perform blurring on t1 and output result to t2, t2 has format RR..GG..BB..
#   3. Copy t2 to display, reformat to output format RGBRGB..
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#

import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
t1 = dgrid.Tensor()
t2= dgrid.Tensor()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,t1,dgrid.COLOR,dgrid.PLANAR)
n2=dgrid.GraphNodeGaussian(t1,t2)
n2.SetSigma(3.0)
n3=dgrid.GraphNodeCopyAndTransform(t2,tensorOutput,dgrid.COLOR,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

