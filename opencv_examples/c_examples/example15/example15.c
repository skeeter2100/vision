/** Filename: example15.c
*
*   Save video from webcam.
*
*       a) Reads frames from a camera.
*       b) Saves frames to a video named movie.avi
*
*       c) Press the escape key to end the program.
*
*   Objective: Learn how to use the cvCreateCameraCapture,
*   cvGetCaptureProperty and cvCreateVideoWriter.
*
*   Exercise for the user is changing the code to read the
*   video save file name from the command line or user input.
*
*/

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>


int main(int argc, char** argv)
{
    IplImage* frame = NULL;                 /// camera image frame

    int key;                                /// key press value
    unsigned int framecount = 0;

    int cameraIndex = 0;

    /** Second camera argument is assumed to be the camera index
    *   argv[0] is the name of the executable
    *   argv[1] is the camera index
    */
    if(argc > 1)
    {
        cameraIndex= atoi(argv[1]);
    }

    printf("cameraIndex: %d\n", cameraIndex);

    /** CvCapture* cvCaptureFromCAM(int device)¶
    *
    *   device - id of the opened video capturing device (i.e. a camera index).
    *            If there is a single camera connected, just pass 0.
    *
    *   Note: Linux Ubuntu 12.04 - camera capture device 0 is /dev/video0
    *                                             device 1 is /dev/video1
    *
    *   To see the list of all connected video devices, type
    *   ls -al /dev/video*
    **/

    CvCapture* capture = NULL;
    capture = cvCaptureFromCAM( cameraIndex );
    if( !capture )
    {
            fprintf( stderr, "ERROR: capture is NULL \n" );
            return -1;
    }

    /** Obtain camera properties
    *
    * double cvGetCaptureProperty(CvCapture* capture, int property_id)
    *
    *   Parameters:
    *       property_id - Property identifier. There are many, we will use
    *                     those shown below.
    *   CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
    *   CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
    *   CV_CAP_PROP_FPS Frame rate.

    */

    double cam_width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    double cam_height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    double cam_fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

    /** This property is not always available */
    if((int)cam_fps == -1)
    {
        fprintf(stderr, "Unable to read camera fps, setting to 30 fps\n");
        cam_fps = 30;
    }

    printf("\nCamera information, width: %d, height: %d, fps: %f\n", (int)cam_width,
            (int)cam_height, cam_fps);
    printf("\npress enter key to start recording video\n");
    getchar();


    /** CvVideoWriter* cvCreateVideoWriter(const char* filename, int fourcc, double fps,
    *                                           CvSize frame_size, int is_color=1 )
    *
    *
    *   Parameters:
    *       filename    Name of the output video file.
    *
    *       fourcc      4-character code of codec used to compress the frames. For example,
    *                   CV_FOURCC('P','I','M,'1') is a MPEG-1 codec, CV_FOURCC('M','J','P','G')
    *                   is a motion-jpeg codec etc. List of codes can be obtained at Video Codecs
    *                   by FOURCC page: http://www.fourcc.org/codecs.php
    *
    *       fps         Frame rate of the created video stream.
    *       frameSize   Size of the video frames.
    *       isColor     If it is not zero, the encoder will expect and encode color frames, otherwise
    *                   it will work with grayscale frames (the flag is currently supported on Windows only).
    *
    *
    *       On Linux FFMPEG is used to write videos; on Windows FFMPEG or VFW is used; on MacOSX QTKit is used.
    */

    CvVideoWriter* vwriter = NULL;
    vwriter = cvCreateVideoWriter("movie.avi", CV_FOURCC('M','J','P','G'), cam_fps,
                                              cvSize((int)cam_width, (int)cam_height), 1);

    if(!vwriter)
    {
        fprintf(stderr, "Error: vwriter is null\n");
        cvReleaseCapture(&capture);
        return -1;
    }


    /** Create windows for the images */
    cvNamedWindow( "camera", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO );

    fprintf(stderr, "recording video to file named movie.avi\npress esc to quit\n");


    /** Press escape key to end program */
    while( 1 )
    {
        /// read image frame, update frame count
        ++framecount;

        /** Grab a frame to retrieve image properties neededto create video writer
        *
        *  IplImage* cvQueryFrame( CvCapture* capture)
        *
        *  Grabs a frame from camera, decompresses and returns it
        *
        *  Uses memory already allocated in the cvCapture structure. Do not call
        *  cvReleaseImage for the frame pointer. Wait until you want to stop the
        *  video input stream and then call cvReleaseCapture to free the memory
        */

        frame = cvQueryFrame( capture );

        if( !frame )
        {
                fprintf( stderr, "ERROR: framecount: %d, frame is null...\n", framecount);
                break;
        }


        /// save frame to video
        cvShowImage("camera", frame);

        /**int cvWriteFrame(CvVideoWriter* writer, const IplImage* image) */
        cvWriteFrame(vwriter, frame);

        key = cvWaitKey(10);        /// wait 10 ms
        if((char)key == 27)
            break;
    }

    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseVideoWriter(&vwriter);
    cvReleaseCapture( &capture );

    cvDestroyAllWindows();

    return 0;
}
