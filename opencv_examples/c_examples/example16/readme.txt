
                 Read Me


*******************************************************
*  Description 
*******************************************************
*   Postion of trackbar sliders perform in range filtering
*   of a color image.
*
*       a) Works with a usb video camera.
*       b) Creates an image in the hsv color space. The color image
*          is in the rgb color space. Color filtering often works
*          better in the hsv space. Program makes a copy of the
*          color image and then converts it to hsv.
*
*       c) Trackbar sliders are used to set a range of hsv values.
*       d) cvInRangeS filter uses the range of min, max hsv values
*          to keep all values in the range. Values outside the range
*          are converted to 0 valued pixels. This produces the single
*          channel image shown in the window named inRangeS.
*
*       e) Press the escape key to end the program.
*
*   Objective: Learn how to use the inRangeS filter, 
*   create multiple trackbars, and work in the hsv color space.


    Usage: ./example16 <camera index>

    Command Line Arguments

        ./example16: name of executable
        camera index: optional argument, defaults to camera index 0.
			If you have more than one camera connected, you can see
                       	a list of video devices: ls -al /dev/video*
        
 
    Press the escape key to end drawing mode.


*******************************************************
*  Source files
*******************************************************

Name:	example16.c


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 1/12/2015
   The program compiles and runs library successfully.  
   
   The programs are developed and tested on Ubuntu 14.04,
   using gcc version 4.8.2, OpenCV 2.4.*
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Folder example15 contains the following files:

	example15.c
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % g++ -Wall -g example15.c -o example15 $(pkg-config --cflags --libs opencv)

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
   % ./example16 <camera index>

   





