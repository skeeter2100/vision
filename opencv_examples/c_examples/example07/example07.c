/** Filename: example07.c
*
*   Description: Add trackbar (sliders) to an image window.
*   Opens a color image file named in argv[1].
*
*   A single trackbar slider is added to the color image window.
*   Moving the slider causes the slider value to be displayed in the
*   terminal window. Press the escape key to end the program.
*
*   Objective: Learn how to create a trackbar and attach that trackbar
*   to a particular window. Learn how to code the trackbar callback function.
*
*/

#include <stdio.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

/** Global variables */
const int slider_max = 100;         // max value of slider position
int slider_value = 0;               // start value of slider position

IplImage* colorimg = NULL;


/** Create a callback function for the trackbar slider
*
*   A trackbar slider will be created in the main function with a call
*   to cvCreateTrackbar. This function will be registered as the
*   callback function. Whenever the slider is moved, this function
*   will automatically be called. The parameter position will hold
*   the value of the position of the slider bar.
*/
void onTrackbarSlide(int position)
{
    printf("slider position: %d\n", position);
    cvShowImage("color", colorimg);
    cvWaitKey(30);      // give highgui time to draw image
}


int main(int argc, char** argv){

    /** Two arguments must be passed to the program.
    *   argv[0] is the name of the executable
    *   argv[1] is the name of the image file to open
    */
    if(argc < 2){
        printf("Usage: ./example07 imageName");
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
    cvNamedWindow("color", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);


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

    cvCreateTrackbar("Slider 1", "color", &slider_value, slider_max, onTrackbarSlide);

    cvShowImage("color", colorimg);

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
    cvDestroyAllWindows();


    return 0;
}
