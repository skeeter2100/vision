/** Filename: example08.c
*
*   Description: Add trackbar (sliders) to an image window.
*   Opens a color image file named in argv[1], converts the color image
*   to grayscale. A single trackbar slider is added to the grayscale
*   image window.
*
*   The slider position value is used as the threshold to the cvThreshold
*   function, using the CV_THRESHOLD_TOZERO filter we learned in
*   example 6.
*
*   Press the escape key to end the program.
*
*   Objective: Learn how to create a trackbar and attach that trackbar
*   to a particular window. Learn how to code the trackbar callback function.
*
*/

#include <stdio.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

/** Global variables */
const int slider_max = 255;         // max value of slider position
int slider_value = 50;               // start value of slider position

/** Both functions main and onTrackbarSlide need access to the
*   grayscale image and the threshold image. Make these global
*   variables.
*/
IplImage* grayimg = NULL;
IplImage* thresholdimg = NULL;


/** Create a callback function for the trackbar slider
*
*   A trackbar slider will be created in the main function with a call
*   to cvCreateTrackbar. This function will be registered as the
*   callback function. Whenever the slider is moved, this function
*   will automatically be called. The parameter position will hold
*   the value of the position of the slider bar.
*
*   Moving the slider changes the threshold filter value.
*/
void onTrackbarSlide(int position)
{
    // filter based on new slider position
    cvThreshold(grayimg, thresholdimg, slider_value, slider_max, CV_THRESH_TOZERO);
    cvShowImage("threshold filter", thresholdimg);
    //cvWaitKey(30);      // give highgui time to draw image
}


int main(int argc, char** argv){

    /** Two arguments must be passed to the program.
    *   argv[0] is the name of the executable
    *   argv[1] is the name of the image file to open
    */
    if(argc < 2){
        printf("Usage: ./example08 imageName");
        return -1;
    }

    /** Load the input image.
    *   Note: loading the image with this C function dynamically creates memory for the image data.
    *   Later we will need to release the image to free this memory.
    */
    IplImage* colorimg = cvLoadImage(argv[1],1);

    /** The pointer will be NULL if the image was not correctly opened and loaded into memory */
    if(colorimg == NULL){
        printf("File %s not opened, program ending\n", argv[1]);
        return -1;
    }

    /** Allocate memory to store the grayscale image. Use the function
    *
    *   IplImage* cvCreateImage(CvSize size, int depth, int channels)
    */

    grayimg = cvCreateImage( cvSize(colorimg->width, colorimg->height), colorimg->depth, 1);

    /** Let's verify memory was allocated. We will terminate the program in this case.
    *   We need to release the memory allocated for the colorimg before terminating the program */
    if(grayimg == NULL){
        printf("No memory allocated for grayimg\n");
        cvReleaseImage(&colorimg);                      /// Prevent memory leak
        return -1;
    }

    /** Allocate memory to store the thresholded image */
    thresholdimg = cvCreateImage( cvSize(colorimg->width, colorimg->height), colorimg->depth, 1);

    /** Let's verify memory was allocated. We will terminate the program in this case.
    *   We need to release the memory allocated for the colorimg before terminating the program */
    if(thresholdimg == NULL){
        printf("No memory allocated for thresholdimg\n");
        cvReleaseImage(&colorimg);                      /// Prevent memory leak
        cvReleaseImage(&grayimg);
        return -1;
    }

    /** Convert color image to grayscale
    *   void cvCvtColor(const CvArr* src, CvArr* dst, int code)
    */
    cvCvtColor(colorimg, grayimg, CV_BGR2GRAY);

    /** Apply the threshold filter.
    *
    *   double cvThreshold(const CvArr* src, CvArr* dst, double threshold, double max_value,
    *                       int threshold_type)
    *
    *       CV_THRESH_TOZERO   dst(x,y) = src(x,y) if src(x,y) > thresh, 0 otherwise
    */
    cvThreshold(grayimg, thresholdimg, slider_value, slider_max, CV_THRESH_TOZERO);



    /** Display the images */
    cvNamedWindow("color", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    cvNamedWindow("gray", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    cvNamedWindow("threshold filter", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

    cvMoveWindow("color", 100, 100);
    cvMoveWindow("gray", 900, 100);
    cvMoveWindow("threshold filter", 500, 100);

    cvShowImage("color", colorimg);
    cvShowImage("gray", grayimg);
    cvShowImage("threshold filter", thresholdimg);


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

    cvCreateTrackbar("Threshold", "threshold filter", &slider_value, slider_max, onTrackbarSlide);


    int key;

    /** Press escape key to end program */
    while( 1 )
    {
        key = cvWaitKey(30);        // wait 30 ms
        if((char)key == 27)
            break;
    }

    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseImage(&colorimg);
    cvReleaseImage(&grayimg);
    cvReleaseImage(&thresholdimg);
    cvDestroyAllWindows();


    return 0;
}
