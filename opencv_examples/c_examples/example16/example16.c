/** Filename: example16.c
*
*   Description: Postion of trackbar sliders perform in range filtering
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
*   Objective: Learn how to use the inRangeS filter, create multiple trackbars,
*   and work in the hsv color space.
*
*/

#include <stdio.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

/** Global variables */

/// max and min slider value range constants
const int HUE_MAX = 79;         // max value of slider position
const int SAT_MAX = 255;
const int VAL_MAX = 255;
const int HUE_MIN = 36;
const int SAT_MIN = 0;
const int VAL_MIN = 0;

/// declare and initialize slider value position variables
int hue_max = HUE_MAX;
int hue_min = HUE_MIN;
int sat_max = SAT_MAX;
int sat_min = SAT_MIN;
int val_max = VAL_MAX;
int val_min = VAL_MIN;

/// Global image variable
IplImage* colorimg = NULL;


/** Create a callback function for the trackbar slider
*
*   A trackbar slider will be created in the main function with a call
*   to cvCreateTrackbar. This function will be registered as the
*   callback function. Whenever the slider is moved, this function
*   will automatically be called. The parameter position will hold
*   the value of the position of the slider bar.
*/
void onHueMax(int position)
{
    hue_max = position;
}

void onHueMin(int position)
{
    hue_min = position;
}

void onSatMax(int position)
{
    sat_max = position;
}

void onSatMin(int position)
{
    sat_min = position;
}

void onValMax(int position)
{
    val_max = position;
}

void onValMin(int position)
{
    val_min = position;
}

int main(int argc, char** argv)
{
    IplImage* frame;                    /// camera image frame
    IplImage *hsvimg, *filterimg, *dilateimg;
    int key;
    unsigned int framecount = 0;

    int cameraIndex = -1;

	 //cvMemStorage *hlines;

    /** Second camera argument is assumed to be the camera index
    *   argv[0] is the name of the executable
    *   argv[1] is the camera index
    */
    if(argc > 1)
    {
        cameraIndex= atoi(argv[1]);
		fprintf(stderr, "cameraIndex: %d\n",cameraIndex);
    }

    /** CvCapture* cvCreateCameraCapture( int index)
    *
    *  index - Index of the camera to be used. If there is only one camera or it does not matter what
    *          camera is used, -1 may be passed
    *
    *   Note: Linux 12.04 - Open capture device 0 is /dev/video0
    *                                    device 1 is /dev/video1
    *
    *   To see the list of all connected video devices, type
    *   ls -al /dev/video*
    **/

    CvCapture* capture = cvCaptureFromCAM( cameraIndex );
    if( !capture )
    {
            fprintf( stderr, "ERROR: capture is NULL \n" );
            return -1;
    }

    /** Capture one image frame from the camera to allow us to create an hsv image
    *   and a filtered image before entering the while loop. This allows us to use
    *   the same areas of memory for these images each time through the while loop
    *   without creating and destroying them with each execution of the loop.
    **/

    /** IplImage* cvQueryFrame( CvCapture* capture)
    *
    *  Grabs a frame from camera, decompresses and returns it
    *
    *  Uses memory already allocated in the cvCapture structure. Do not call
    *  cvReleaseImage for the frame pointer. Wait until you want to stop the
    *  video stream and then call cvReleaseCapture to free the memory
    */
    frame = cvQueryFrame( capture );
    if( !frame )
    {
            fprintf( stderr, "ERROR: frame is null...\n" );
            return -1;
    }

    /** Create a 3 channel image to store hsv image
    *   Creates an image header and allocates memory for the image data. */
    hsvimg = cvCreateImage(cvGetSize(frame), frame->depth, 3);
    if(hsvimg == NULL)
    {
        // Release the capture device housekeeping
        cvReleaseCapture( &capture );
        fprintf(stderr, "Not able to create hsvimg\n");
        return -1;
    }

    /// Create a single channel image to display filtered results
    filterimg = cvCreateImage(cvGetSize(frame), frame->depth, 1);
    if(filterimg == NULL)
    {
        // Release the capture device housekeeping
        cvReleaseCapture( &capture );
        cvReleaseImage(&hsvimg);
        fprintf(stderr, "Not able to create filterimg\n");
        return -1;
    }

	/// Create a single channel image to display filtered results
    dilateimg = cvCreateImage(cvGetSize(frame), frame->depth, 1);
    if(dilateimg == NULL)
    {
        // Release the capture device housekeeping
        cvReleaseCapture( &capture );
        cvReleaseImage(&hsvimg);
		  cvReleaseImage(&filterimg);
        fprintf(stderr, "Not able to create filterimg\n");
        return -1;
    }


    /// Create a window in which the captured images will be presented
    cvNamedWindow( "camera", CV_WINDOW_AUTOSIZE );

    /// Create a window for the filtered image
    cvNamedWindow("inRangeS", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);


    /** int cvCreateTrackbar(const char* trackbar_name, const char* window_name,
    *                           int* value, int count, CvTrackbarCallback on_change=NULL )
    *
    *   Description: Creates a trackbar and attaches it to the specified window.
    *
    *   Parameters:
    *       trackbarname – Name of the created trackbar.
    *       winname – Name of the window that will be used as a parent of the created trackbar.
    *       value – Optional pointer to an integer variable whose value reflects the position of the slider. Upon creation, the slider position is defined by this variable.
    *       count – Maximal position of the slider. The minimal position is always 0.
    *       onChange – Pointer to the function to be called every time the slider changes position. This function should be prototyped as void Foo(int,void*); , where the first parameter is the trackbar position and the second parameter is the user data (see the next parameter). If the callback is the NULL pointer, no callbacks are called, but only value is updated.
    *       userdata – User data that is passed as is to the callback. It can be used to handle trackbar events without using global variables.
    *
    *
    *   The first two arguments are the name for the trackbar itself and the name of the parent
    *   window to which the trackbar will be attached. When the trackbar is created it is added
    *   to either the top or the bottom of the parent window; it will not occlude any image that
    *   is already in the window.
    *
    *   The next two arguments are value, a pointer to an integer that will be set automatically
    *   to the value to which the slider has been moved, and count, a numerical value for the
    *   maximum value of the slider.
    *
    *   The last argument is a pointer to a callback function that will be automatically called
    *   whenever the slider is moved. If used, the callback function must have the form
    *   CvTrackbarCallback, which is defined as: void (*callback)( int position )
    *
    *   This callback is not actually required, so if you don’t want a callback then you can
    *   simply set this value to NULL. Without a callback, the only effect of the user moving the
    *   slider will be the value of *value being changed.
    *
    */
    cvCreateTrackbar("h max", "camera", &hue_max, HUE_MAX, onHueMax);
    cvCreateTrackbar("h min", "camera", &hue_min, HUE_MAX, onHueMin);
    cvCreateTrackbar("s max", "camera", &sat_max, SAT_MAX, onSatMax);
    cvCreateTrackbar("s min", "camera", &sat_min, SAT_MAX, onSatMin);
    cvCreateTrackbar("v max", "camera", &val_max, VAL_MAX, onValMax);
    cvCreateTrackbar("v min", "camera", &val_min, VAL_MAX, onValMin);

	 //MemStorageAlloc(hlines, sizeof(double)*20);


    /** Press escape key to end program */
    while( 1 )
    {
        /// Get one frame
        ++framecount;
        frame = cvQueryFrame( capture );

        if( !frame )
        {
                fprintf( stderr, "ERROR: framecount: %d, frame is null...\n", framecount);
                break;
        }


        /// convert from bgr to hsv
        cvCvtColor(frame, hsvimg, CV_BGR2HSV);

        /** Filter the color image
        *
        *   void cvInRangeS(const CvArr* src, CvScalar lower, CvScalar upper, CvArr* dst)¶
        *
        *   Checks if array elements lie between the elements of two other arrays.
        *
        *   Parameters:
                src – first input array.
                lowerb – inclusive lower boundary array or a scalar.
                upperb – inclusive upper boundary array or a scalar.
                dst – output array of the same size as src and CV_8U type.

        */
        cvInRangeS(hsvimg, cvScalar(hue_min, sat_min, val_min, 0), cvScalar(hue_max, sat_max, val_max, 0),filterimg);

		  cvErode(filterimg, dilateimg, NULL, 3);
		  cvDilate(dilateimg, dilateimg, NULL, 3);

		  


        cvShowImage("camera", frame);		
        cvShowImage("inRangeS", filterimg);
		  cvShowImage("dilate", dilateimg);

        key = cvWaitKey(30);        // wait 30 ms
        if((char)key == 27)
            break;
    }

    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseCapture( &capture );       // Release the capture device housekeeping
    cvReleaseImage(&hsvimg);
    cvReleaseImage(&filterimg);
	 cvReleaseImage(&dilateimg);
    cvDestroyAllWindows();

    return 0;
}
