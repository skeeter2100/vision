
                 Read Me


*******************************************************
*  Description 
*******************************************************

Program opens an image file and displays the image.

Objective: Learn the OpenCV functions
		cvLoadImage, cvNamedWindow, cvShowWindow, 
		cvWaitKey, cvReleaseImage, cvDestroyWindow

The program is well commented to help you understand the 
functions and their parameters.


*******************************************************
*  Source files
*******************************************************

Name:	example01.c


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 1/6/2015
   The program compiles and runs library successfully.  
   
   The programs are developed and tested on Ubuntu 14.04,
   using gcc version 4.8
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Folder example01 contains the following files:

	example01.c
	filled_geometric_shapes.jpg
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % gcc -Wall -g example01.c -o example01 $(pkg-config --cflags --libs opencv)

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
   % ./example01 filled_geometric_shapes.jpg

   Note: you may use any image file with the program. 




