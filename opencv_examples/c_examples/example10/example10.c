/** Filename: example10.c
*
*   Description: Program takes input from a usb camera and saves
*                input as still images
*
*   Objectives:
*       1) Capture frames from a usb camera
*       2) OpenCV function to save frames as still images.
*
*   Usage: ./example10 imageSaveNamePrefix < number of images to save > < camera index >
*
*   Command Line Arguments
*
*       ./example10: name of executable
*       imageSaveNamePrefix: string of characters to name still images
*       number of images to save: number of still images to save, defaults to 3
*
*       camera index:
*               default value, -1. If you have more than 1 camera
*               connected to your computer, pass an integer other
*               than 0 to select another camera.
*
*       Note: < > denotes optional arguments
*
*   Example: Use the default camera. Save 10 images with the name prefix
*            tuesday. Will save 10 images named tuesday1, tuesday2, ...,
*            tuesday10.
*
*   ./example10 tuesday 10
*
*   Press the escape key to end the program.
*
*/


#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FRAME_SPACING 5

#define NAME_LENGTH  32


// Function Prototypes
int ParseCommandLine(int argc, char** argv, char *imageSaveName, int *numberImagesToSave, int *cameraIndex);


int main(int argc, char* argv[])
{
    int imageCount = 0;
    int frameCount = 0;
    int numberOfImagesToSave = 3;
    int cameraIndex = -1;

    char imageSaveName[NAME_LENGTH];

    if(!ParseCommandLine(argc, argv, imageSaveName, &numberOfImagesToSave, &cameraIndex))
        return -1;

    /** CvCapture* cvCreateCameraCapture( int index)
    *
    *  index - Index of the camera to be used. If there is only one camera or it does not matter what
    *          camera is used, -1 may be passed
    *
    *   Note: Linux 12.04 - Open capture device 0 is /dev/video0
    *                                    device 1 is /dev/video1
    *
    *   To see the list of usb devices, type lsusb on the command line
    */

    CvCapture* capture = cvCaptureFromCAM( cameraIndex );
    if( !capture )
    {
            fprintf( stderr, "ERROR: capture is NULL \n" );
            return -1;
    }

    /// Create a window in which the captured images will be presented
    cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );

    while(1)
    {
        /// Get one frame

        /** IplImage* cvQueryFrame( CvCapture* capture)
        *
        *  Grabs a frame from camera, decompresses and returns it
        *
        *  Uses memory already allocated in the cvCapture structure. Do not call
        *  cvReleaseImage for the frame pointer. Wait until you want to stop the
        *  video stream and then call cvReleaseCapture to free the memory
        */
        IplImage* frame = cvQueryFrame( capture );
        if( !frame )
        {
                fprintf( stderr, "ERROR: frame is null...\n" );
                break;
        }

        while(++frameCount % FRAME_SPACING == 0 && imageCount < numberOfImagesToSave)
        {
            ++imageCount;
            /// Create image save name string
            char buffer[NAME_LENGTH];
            char number_string[5];
            memcpy(buffer, imageSaveName, NAME_LENGTH);
            sprintf(number_string, "%05d",imageCount);
            strcat(buffer, number_string);
            strcat(buffer, ".jpg");

            fprintf(stderr, "buffer: %s\n", buffer);

            /** int cvSaveImage( const char* filename, const CvArr* image);
            *
            *  Image format is chosen depending on the filename extension
            */
            cvSaveImage(buffer, frame);
        }

        cvShowImage( "Camera", frame );

        // Do not release the frame!

        //If ESC key pressed, Key=0x10001B under OpenCV,
        //remove higher bits using AND operator
        if( (cvWaitKey(10) & 255) == 27 ) break;

    }

    // Release the capture device housekeeping
     cvReleaseCapture( &capture );
     cvDestroyWindow( "Camera" );
    return 0;
} // end of main


int ParseCommandLine(int argc, char** argv, char *imageSaveName, int *numberImagesToSave, int *cameraIndex)
{
    /** Note: this if statement sequence can be written with much less code.
    *   It was written in this manner to make the logic easier to understand
    */
    if (argc >= 4)
    {
        memset(imageSaveName, '\0', NAME_LENGTH );
        memcpy(imageSaveName, argv[1], NAME_LENGTH);
        *numberImagesToSave = atoi(argv[2]);
        *cameraIndex= atoi(argv[3]);
        return 1;
    }
    else if(argc == 3)
    {
        memset(imageSaveName, '\0', NAME_LENGTH );
        memcpy(imageSaveName, argv[1], NAME_LENGTH);
        *numberImagesToSave = atoi(argv[2]);
        return 1;
    }
    else if(argc < 2)
    {
        printf("Usage: ./example10 imageSaveNamePrefix < number of images to save > < camera index >\n");
        return 0;
    }
    else
    {
        memset(imageSaveName, '\0', NAME_LENGTH );
        memcpy(imageSaveName, argv[1], NAME_LENGTH);
        printf("ParseCommandLine, using default parameter values, images to save = %d, camera index = %d\n",
                *numberImagesToSave, *cameraIndex);
        return 1;
    }
}
