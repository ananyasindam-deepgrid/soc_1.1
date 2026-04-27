#
# This example convert color input from webcam to monochrome for display
# Execution graph:
#   1. Copy webcam input to display. During copying, for each pixel, compute [R,G,B]=MonoValue(R,G,B)
# Program runs until push button is pressed
#
# Reference dgrid/micropython/MicropythonUserGuide.md documentation for more details.
#

import dgrid

tensorInput = dgrid.TensorCamera()
tensorOutput = dgrid.TensorDisplay()
n1=dgrid.GraphNodeCopyAndTransform(tensorInput,tensorOutput,dgrid.MONO3,dgrid.INTERLEAVED)
graph=dgrid.Graph(n1)
while (dgrid.ButtonState()==0):
   if(dgrid.CameraCapture()) :
       graph.Run()
       dgrid.CanvasDrawText('{} msec'.format(dgrid.GetElapsedTimeMsec()),0,0)
       dgrid.DisplayFlushCanvas()
dgrid.DeleteAll()

