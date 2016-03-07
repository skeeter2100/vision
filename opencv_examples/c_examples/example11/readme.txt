
                 Read Me


*******************************************************
*  Description 
*******************************************************

    Program takes input from a still image

    Objectives:
        1) Left mouse button down starts drawing a red rectangle on the image
	2) Hold left mouse buttonn down and move mouse to draw rectangles
	3) Releasing the left mouse button finishes drawing the rectangle
	4) Ctrl + left mouse button click erases last drawn rectangle
        5) Press escape key to end drawing mode
	6) Color pixel statistics are displayed for each rectangular region
	   when drawing mode ends.
	7) A copy of the work image, including any rectangles drawn is saved.

    Usage: ./example11 imageName 

    Command Line Arguments

        ./example11: name of executable
        imageName: name of image to open
        
 
    Press the escape key to end drawing mode. Follow terminal prompts for
    pressing the enter key when the program pauses.


*******************************************************
*  Source files
*******************************************************

Name:	example11.c


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 1/12/15
   The program compiles and runs library successfully.  
   
   The programs are developed and tested on Ubuntu 14.04,
   using gcc version 4.8.2, OpenCV 2.4.8
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Folder example11 contains the following files:

	example11.c
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % g++ -Wall -g example11.c -o example11 $(pkg-config --cflags --libs opencv)

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
   % ./example11 imageName

   





