All:example10

example10: example10.c 
	g++ -Wall -g example10.c  -o example10 \
	-I /usr/local/include/opencv2 -I /usr/local/include  -L /usr/local/lib \
	-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video \
	-lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib \
	-lopencv_legacy -lopencv_flann  

clean: 
	rm example10


