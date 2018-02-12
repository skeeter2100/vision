import sys 
import cv2
import numpy as np
import copy


drawingBox = False 	# true when mouse pressed
x1, y1 = -1, -1     # initial x,y mouse coordinates
x2, y2 = -1, -1

boxList = list()

# mouse callback function
def on_mouse(event, x, y, flags, param):
	global x1, y1, x2, y2, drawingBox, boxList

	workImg = param;	# points to image registered with mouse call back

	if event == cv2.EVENT_LBUTTONDOWN:
		print("down")
		drawingBox = True 
		x1,y1 = x,y 

	elif event == cv2.EVENT_MOUSEMOVE:
		x2,y2 = x,y

	elif event == cv2.EVENT_LBUTTONUP:
		print("up")
		x2, y2 = x,y
		drawingBox = False 
		cv2.rectangle(workImg, (x1, y1), (x2,y2), (0,255,0,0), 5)
		boxList.append((x1,y1,x2,y2))
		


def display_image_info(img):
	print('\n(rows, cols, channels) ', img.shape)
	print('number of pixels:      ', img.size)
	print('pixel data type:       ', img.dtype)



if __name__ == '__main__':


	if len(sys.argv) < 2:
		print('Too few arguments, please specify an image filename')
		print ('example05.py  imageFileName')
		sys.exit(2)

	# argv[1] input image file name
	filename = sys.argv[1]
	
	img = cv2.imread(filename)
	workImg = copy.deepcopy(img)
	tempImg = copy.deepcopy(img)

	display_image_info(img)

	
	cv2.namedWindow('original', cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)
	cv2.namedWindow('mouse draw', cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)

	cv2.setMouseCallback('mouse draw', on_mouse, workImg)

	print("\npress escape key to quit")

	cv2.imshow('original', img)

	while(1):
		if drawingBox == True:
			tempImg = copy.deepcopy(workImg)
			cv2.rectangle(tempImg, (x1, y1), (x2,y2), (0,255,0,0), 5)

		cv2.imshow('mouse draw', tempImg)
		if cv2.waitKey(20) & 0xFF == 27:
			break 
	
	print('\nbox coordinates')
	for (ix, iy, fx, fy) in boxList:
		print("x1: " + str(ix) + ", y1: " + str(iy) + ", x2: " + str(fx) + ", y2: " + str(fy))


	# save a copy of the working image with the boxes
	cv2.imwrite("boxImage.jpg", tempImg)
	
	cv2.destroyAllWindows()
	