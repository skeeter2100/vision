import numpy as np 
import cv2
import sys

# ensure there are at least two command line arguments
# program assumes second argument is image file name

if len(sys.argv) < 2:
	print ('error: missing command line argument for image name')
	print ('example01.py  imageFileName')
	sys.exit(2)
	
imageName = sys.argv[1]

''' cv2.imread( image name, flag )

	image name: name of image file to be read. Loads from the working directory
				unless the path is specified
	
	flags:
		cv2.IMREAD_COLOR       loads a color image, transparency is neglected
		cv2.IMREAD_GRAYSCALE   load image in grayscale mode
		cv2.IMREAD_UNCHANGED   loads image as such including alpha channel

		Numeric values of flags:
		cv2.IMREAD_COLOR       1
		cv2.IMREAD_GRAYSCALE   0
		cv2.IMREAD_UNCHANGED   -1

		If the image path is incorrect, no error is thrown

'''
colorimg = cv2.imread(imageName, cv2.IMREAD_COLOR)

grayimg = cv2.imread(imageName, cv2.IMREAD_GRAYSCALE)

''' The function namedWindow creates a window that can be used as a placeholder 
    for images and trackbars. Created windows are referred to by their names.

    namedWindow( window Name, flags)

    WINDOW_NORMAL flag allows user to resize the window

'''
cv2.namedWindow('color', cv2.WINDOW_NORMAL)
cv2.namedWindow('gray', cv2.WINDOW_NORMAL)


''' To specifically position window on screen
    moveWindow(window name, x, y)
'''
cv2.moveWindow("color", 50, 50)
cv2.moveWindow("gray", 50, 400)



''' Display an image

	cv2.imshow(window name, image to display)

	window name is a string argument

	window automatically fits to image size
	'''

cv2.imshow('color', colorimg)
cv2.imshow('gray', grayimg)

''' waitkey( time in milliseconds)

	function waits for specified milliseconds for a keyboard event. If you
	press any key in that time, the program continues.

	If 0 is passed, it waits indefinitely for a keystroke

'''
print ('press any key to end program')
cv2.waitKey(0)

# free memory
cv2.destroyAllWindows()
