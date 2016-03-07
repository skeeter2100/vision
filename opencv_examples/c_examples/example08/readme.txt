
                 Read Me


*******************************************************
*  Description 
*******************************************************

Program opens an color image file, converts the color image
to grayscale and then thresholds the grayscale image using
the value of the slider position as the threshold value.

Use a mouse to move the slider.

Objective: 
	Learn how to add a trackbar slider to a window
        and read the slider value.

	Learn how to end your program by pressing the
	escape key.

The program is well-commented to help you understand the 
functions and their parameters.


*******************************************************
*  Source files
*******************************************************

Name:	example08.c


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

1. Folder example08 contains the following files:

	example08.c
	cody.jpg
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % gcc -Wall -g example08.c -o example08 $(pkg-config --cflags --libs opencv)

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
   % ./example08 cody.jpg

   Note: you may use any image file with the program. 





