/** Filename: example12.c
*
*   Description: Postion of trackbar sliders perform in range filtering
*   of a color image.
*
*       a) Opens a color image file named in argv[1].
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
const int HUE_MAX = 180;         // max value of slider position
const int SAT_MAX = 255;
const int VAL_MAX = 255;
const int HUE_MIN = 0;
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
    IplImage *hsvimg, *filterimg;
    int key;

    /** Two arguments must be passed to the program.
    *   argv[0] is the name of the executable
    *   argv[1] is the name of the image file to open
    */
    if(argc < 2){
        printf("Usage: ./example12 imageName");
        return -1;
    }

    /** Load the input image.
    *   Note: loading the image with this C function dynamically creates memory for the image data.
    *   Later we will need to release the image to free this memory.
    */
    colorimg = cvLoadImage(argv[1],1);

    /** The pointer will be NULL if the image was not correctly opened and loaded into memory */
    if(colorimg == NULL){

        printf("File %s not opened, program ending\n", argv[1]);
        return -1;
    }

    /** Create a window for the image */
    cvNamedWindow("source", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
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
    cvCreateTrackbar("h max", "source", &hue_max, HUE_MAX, onHueMax);
    cvCreateTrackbar("h min", "source", &hue_min, HUE_MAX, onHueMin);
    cvCreateTrackbar("s max", "source", &sat_max, SAT_MAX, onSatMax);
    cvCreateTrackbar("s min", "source", &sat_min, SAT_MAX, onSatMin);
    cvCreateTrackbar("v max", "source", &val_max, VAL_MAX, onValMax);
    cvCreateTrackbar("v min", "source", &val_min, VAL_MAX, onValMin);

    /** Creates an image header and allocates the image data. */
    hsvimg = cvCreateImage(cvGetSize(colorimg), colorimg->depth, 3);
    if(hsvimg == NULL)
    {
        cvReleaseImage(&colorimg);
        fprintf(stderr, "Not able to create hsvimg\n");
        return -1;
    }

    /// Copy the source color image data into the hsv image
    hsvimg = cvCloneImage(colorimg);

    /// convert from bgr to hsv
    cvCvtColor(colorimg, hsvimg, CV_BGR2HSV);

    /// Create a bgr image to display filtered results
    filterimg = cvCreateImage(cvGetSize(hsvimg), hsvimg->depth, 1);
    if(filterimg == NULL)
    {
        cvReleaseImage(&colorimg);
        fprintf(stderr, "Not able to create filterimg\n");
        return -1;
    }

    /// Display image information
    printf("\nNumber of channels,\tsource: %d, hsv: %d, filter: %d\n",
           colorimg->nChannels, hsvimg->nChannels, filterimg->nChannels);
    printf("Channel depth in bits,\tsource: %d, hsv: %d, filter: %d\n\n",
           colorimg->depth, hsvimg->depth, filterimg->depth);

    printf("pixel width, \tsource: %d, hsv: %d, filter: %d\n",
           colorimg->width, hsvimg->width, filterimg->width);
    printf("pixel height, \tsource: %d, hsv: %d, filter: %d\n",
           colorimg->height, hsvimg->height, filterimg->height);

    printf("\nimage size in bytes, source: %d, hsv: %d, filter: %d\n\n",
           colorimg->imageSize, hsvimg->imageSize, filterimg->imageSize);


    /** Press escape key to end program */
   while( 1 )
   {

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

        cvShowImage("source", colorimg);
        cvShowImage("inRangeS", filterimg);

        key = cvWaitKey(30);        // wait 30 ms
        if((char)key == 27)
            break;
    }

    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseImage(&colorimg);
    cvReleaseImage(&hsvimg);
    cvReleaseImage(&filterimg);
    cvDestroyAllWindows();

    return 0;
}
