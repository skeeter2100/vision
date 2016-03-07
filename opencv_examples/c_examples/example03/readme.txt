
                 Read Me


*******************************************************
*  Description 
*******************************************************

Program opens an image file, displays information about 
the image, and displays the image.

example03 opens a color image, displays the source color
image, then splits the color image into its separate
blue, green, and red channels.

Each channel is displayed. Note that the channels are 
grayscale images. Try running this with the filled_geometric_shapes.jpg
image. 

You will note that the large red circle appears as
white in the red channel image, and black in the blue and
green channels. The red circle has a high red intensity which
corresponds to a high intensity in a gray scale image. 

Note how the green, yellow, and blue objects appear in the
various color channels.

Objective: Learn how to access individual color channels and
understand intensity values in color space and grayscale.

Functions not previously referenced in examples 1 and 2 are
well-commented to help you understand the functions and their parameters.


*******************************************************
*  Source files
*******************************************************

Name:	example03.c


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 1/8/2015
   The program compiles and runs library successfully.  
   
   The programs are developed and tested on Ubuntu 14.04,
   using gcc version 4.8.2, OpenCV 2.4.*
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Folder example03 contains the following files:

	example03.c
	filled_geometric_shapes.jpg
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % gcc -Wall -g example03.c -o example03 $(pkg-config --cflags --libs opencv)

    You must have the package pkg-config installed for this to work. 



3. Makefile Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % make

    If you experience problems with the Makefile, it is likely due to
    differences in paths where OpenCV was installed. To see the include
    path on your computer:
    % pkg-config --cflags opencv

    To see the library path on your computer:
    % pkg-config --libs opencv

    Change the -I and -L paths in the Makefile to match your installation.
    You may also need to change the #include directives for the OpenCV
    header files in the source code as well.


4. Run the executable program:
   % ./example03 filled_geometric_shapes.jpg

   Note: you may use any image file with the program. 







