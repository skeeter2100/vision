# Trackbar sliders control range of color filter

import cv2
import numpy as np

#optional argument to cv2.createTrackbar
def nothing(x):
    pass


# open camera video stream 
cap = cv2.VideoCapture(0)

# create window name before using it in track bar functions
cv2.namedWindow('filter')   


# track bar names
bh='Blue High'
bl='Blue Low'
gh='Green High'
gl='Green Low'
rh='Red High'
rl='Red Low'


''' create trackbars to control color filter ranges

    createTrackbar( trackbar name, window name, default value,
                    maximum value, callback function name)

    callback function is executed each time trackbar changes
        our callback function does nothing


'''

cv2.createTrackbar(bl, 'filter',0,255,nothing)
cv2.createTrackbar(bh, 'filter',0,255,nothing)
cv2.createTrackbar(gl, 'filter',0,255,nothing)
cv2.createTrackbar(gh, 'filter',0,255,nothing)
cv2.createTrackbar(rl, 'filter',0,255,nothing)
cv2.createTrackbar(rh, 'filter',0,255,nothing)

# set trackbar position
cv2.setTrackbarPos(bh, 'filter', 255)
cv2.setTrackbarPos(gh, 'filter', 255)
cv2.setTrackbarPos(rh, 'filter', 255)

while(1):
    
    ret,frame=cap.read()
    
    #create a working copy
    bgr=frame.copy()


    #read trackbar positions for all
    blow  =cv2.getTrackbarPos(bl, 'filter')
    bhigh =cv2.getTrackbarPos(bh, 'filter')
    glow  =cv2.getTrackbarPos(gl, 'filter')
    ghigh =cv2.getTrackbarPos(gh, 'filter')
    rlow  =cv2.getTrackbarPos(rl, 'filter')
    rhigh =cv2.getTrackbarPos(rh, 'filter')
    
    #make array for final values
    BGRLOW  = np.array([blow,glow,rlow])
    BGRHIGH = np.array([bhigh,ghigh,rhigh])

    #apply the range on a mask
    mask = cv2.inRange(bgr,BGRLOW, BGRHIGH)
    res = cv2.bitwise_and(frame,frame, mask =mask)

    cv2.imshow('filter', res)
    cv2.imshow('original', frame)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break


cv2.destroyAllWindows()