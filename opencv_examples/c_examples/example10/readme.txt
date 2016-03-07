
                 Read Me


*******************************************************
*  Description 
*******************************************************

    Program takes input from a usb camera and saves
    input as still images

    Objectives:
        1) Capture frames from a usb camera
        2) OpenCV function to save frames as still images.

    Usage: ./example10 imageSaveNamePrefix < number of images to save > < camera index >

    Command Line Arguments

        ./example10: name of executable
        imageSaveNamePrefix: string of characters to name still images
        number of images to save: number of still images to save, defaults to 3
 
        camera index:
                default value, -1. If you have more than 1 camera
                connected to your computer, pass an integer other
                than 0 to select another camera.

        Note: < > denotes optional arguments

    Example: Use the default camera. Save 10 images with the name prefix
             tuesday. Will save 10 images named tuesday1, tuesday2, ...,
             tuesday10.
 
    ./example10 tuesday 10
 
    Press the escape key to end the program.


*******************************************************
*  Source files
*******************************************************

Name:	example10.c


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

1. Folder example10 contains the following files:

	example10.c
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % g++ -Wall -g example10.c -o example10 $(pkg-config --cflags --libs opencv)

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
   % ./example10 imageSaveNamePrefix < number of images to save > < camera index >

   





