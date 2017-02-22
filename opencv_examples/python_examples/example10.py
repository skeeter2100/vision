import numpy as np
import cv2
import os

# directory path where images will be written
path = 'images/a7'

# create directory if does not exist
if not os.path.exists(path):
	os.makedirs(path)

# base name for image files
basename = 'a7_'

# track number of frames
frameCount = 0

# connect with video camera stream
cap = cv2.VideoCapture(1)

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    # append frame count to image name 
    # zfill will append leading zeros to integer
    filename = basename + str(frameCount).zfill(5) + '.jpeg'

    # save frame as an image
    cv2.imwrite(os.path.join(path, filename), frame)

    frameCount += 1
    
    # Display the resulting frame
    cv2.imshow('frame',frame)

    # stop running when user presses q
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()