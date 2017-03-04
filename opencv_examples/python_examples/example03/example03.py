#capture video from camera

import cv2
import numpy as np

''' create a video capture object

    argument passed to VideoCapture function is the device index or
    name of a video file. Normally, at least one camera is connected.
    Pass 0 or -1 as argument. If a second camera is connected, its 
    index is 1.
    '''

cap = cv2.VideoCapture(0)

while(1):
    # capture the video frame by frame
    ret,frame=cap.read()

    # convert frame to grayscale
    grayimg = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # display both the original and grayscale images
    cv2.imshow('grayscale', grayimg)
    cv2.imshow('original', frame)
    
    # wait 5 milliseconds for escape key press
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break


cv2.destroyAllWindows()