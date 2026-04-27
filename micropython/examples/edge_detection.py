#
# This example performs Canny edge detection on webcam input and then display
# the result
# Execution graph:
#   1. Copy webcam input to t1. Reformat t1 to have format RR..GG...BB..
#   2. Perform canny edge detection on t1 and result to t2, t2 has a single color plane format
#   3. Copy t2 to display. During copying, reformat to format RGBRGB... with R=G=B
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
n2=dgrid.GraphNodeCanny(t1,t2)
n2.SetThreshold(50,100)
n3=dgrid.GraphNodeCopyAndTransform(t2,tensorOutput,dgrid.MONO3,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1,n2,n3)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

