
                 Read Me


*******************************************************
*  Description 
*******************************************************

    Plays frames of a video file.

       a) Works with both avi and mov files.
       b) MOV file appears to play too fast, but avi speed looks reasonable. 
       
       e) Press the escape key to end the program if you do not want to play
          the entire video.


    Objective: Learn how to use the cvCapture functions to play a video file.


    Usage: ./erun <video_file_name>

    Command Line Arguments

        ./erun: name of executable
        video_file_name: path to your file and name of file
        
 
    Press the escape key to end abruptly. Will cause a segmentation fault error.


*******************************************************
*  Source files
*******************************************************

Name:	example17.c


*******************************************************
*  Circumstances of programs
*******************************************************
Date: 3/19/2014
   The program compiles and runs library successfully.  
   
   The programs are developed and tested on Ubuntu 12.04,
   using gcc version 4.6.3, OpenCV 2.4.*
   


*******************************************************
*  How to build and run the program
*******************************************************

1. Folder example17 contains the following files:

	example17.c
	Makefile
	readme.txt
	


Follow step 2 to build the program from the command line or step 3
to build the program with the Makefile.

2.  Command Line Build

    Change to the directory that contains the file by:
    % cd [directory_name] 

    Compile the program and build the executable file:
    % gcc -Wall -g example17.c -o erun $(pkg-config --cflags --libs opencv)

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
   % ./erun <video_file_name>

   





