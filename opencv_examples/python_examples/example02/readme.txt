
                 Read Me


*******************************************************
*  Description 
*******************************************************

Program opens a color image file, splits the images into
its separate blue, green, and red channels.

Creates an output image composed of the original image,
red image, blue image, and green image.

The individual channels reflect the intensity of that color
in the image. Intensity values run from 0 to 255. 

Very intense red pixels will show as bright red in the red image. 

Similarly,the blue and green channels illustrate the intensity of those
colors in the original image. Black indicates an absence of
that color, a very low intensity value of 0.

Try running this with the filled_geometric_shapes.jpg
image. Any pixels that are white in the original image will
show as very bright red, green, and blue in each of those 
channels as white is the highest intensity value 255.



Objectives: Learn the OpenCV functions: split, merge, namedWindow, moveWindow


Learn how to access individual color channels and
understand intensity values in color space and grayscale.


*******************************************************
*  Source files
*******************************************************

Name:	example02.py


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 3/4/2017
   The program runs successfully.  
   
   The programs are developed and tested on Ubuntu 16.04,
   using python 2.7.12
   


*******************************************************
*  How to run the program
*******************************************************

1. Folder example02 contains the following files:

	example02.py
	readme.txt
	

2.  Dependencies:
        
	If you do not have imutils installed in your system,
        you will need to install it with the command

	pip install imutils


3.  Run script from command line

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Run the program:
    % python example02.py ../images/example_test_images/filled_geometric_shapes.jpg

    Note: you may use any image file with the program. 




