/** Filename: example09.c
*
*   Description: Use mouse to draw blue rectangles on an image.
*
*   The program creates its own image filled with black pixels.
*
*   To draw a blue rectangle, position the mouse cursor in the image
*   window, left click to start drawing the rectangle, hold the left
*   button down and move the mouse. Release the left button to finish
*   drawing the rectangle.
*
*   Press the escape key to end the program.
*
*   Objective: Learn how to create a mouse event call back function and
*   use the rectangle drawing function cvRectangle.
*
*   Reference: Example taken from Learning OpenCV, 1st edition, pages 97-99
*/

#include <stdbool.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>

/** Global variables */

/** Declare a global variable of type CvRect
*   struct CvRect - Stores coordinates of a rectangle.
*/
CvRect box;

/** Need to track when we are in the process of drawing a box */
bool drawing_box = false;




/** Function to draw box onto image*/
void draw_box(IplImage* img, CvRect box){

    /** void cvRectangle(CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color,
    *                       int thickness=1, int line_type=8, int shift=0 )
    *
    *   Draws a simple, thick, or filled up-right rectangle.
    *
    *   Parameters:	img – Image.
    *       pt1 – Vertex of the rectangle.
    *       pt2 – Vertex of the rectangle opposite to pt1 .
    *       rec – Alternative specification of the drawn rectangle.
    *       color – Rectangle color or brightness (grayscale image).
    *       thickness – Thickness of lines that make up the rectangle.
    *           Negative values, like CV_FILLED , mean that the function has to draw a
    *           filled rectangle.
    *       lineType – Type of the line. See the line() description.
    *       shift – Number of fractional bits in the point coordinates.
    *
    *   cvScalar(255,0,0,0) sets color to bright blue
    */
    cvRectangle( img,
                cvPoint(box.x, box.y),
                cvPoint(box.x+box.width, box.y+box.height),
                cvScalar(255, 0, 0, 0), 3, 8, 0);
}


/** A mouse callback function must have the following prototype:
*
*   void CvMouseCallback{ int event, int x, int y, int flags, void* param)
*
*   The CvMouseCallback is replaced with the name of your function. Here
*   we are naming the function "on_mouse".
*
*   Whenever this function is called, OpenCV will fill in the arguments with
*   their appropriate values.
*
*   The event argument will have one of the following values.
*
*   EVENT                               Numerical Value
*
*   CV_EVENT_MOUSEMOVE                      0
*   CV_EVENT_LBUTTONDOWN                    1
*   CV_EVENT_RBUTTONDOWN                    2
*   CV_EVENT_MBUTTONDOWN                    3
*   CV_EVENT_LBUTTONUP                      4
*   CV_EVENT_RBUTTONUP                      5
*   CV_EVENT_MBUTTONUP                      6
*   CV_EVENT_LBUTTONDBLCLK                  7
*   CV_EVENT_RBUTTONDBLCLK                  8
*   CV_EVENT_MBUTTONDBLCLK                  9
*
*
*
*   The x and y parameters will be filled with the mouse's x and y coordinates.
*   These mouse coordinates correspond to the pixel in the image, not the pixel on
*   your video display.
*
*   The flag parameter is a bit field in which individual bits indicate special
*   conditions present at the time of the event.
*
*
*   FLAG                                Numerical Value
*
*   CV_EVENT_FLAG_LBUTTON                   1
*   CV_EVENT_FLAG_RBUTTON                   2
*   CV_EVENT_FLAG_MBUTTON                   4
*   CV_EVENT_FLAG_CTRLKEY                   8
*   CV_EVENT_FLAG_SHIFTKEY                  16
*   CV_EVENT_FLAG_ALTKEY                    32
*
*
*   Example: if you want to test if the shift key were held down, check the 5th bit.
*           AND the flag variable with (1<<4)
*
*
*   The void pointer parameter can be used to allow OpenCV to pass in any additional
*   information in the form of a pointer to whatever kind of stucture needed.
*
*
*/

void on_mouse(int event, int x, int y, int flags, void* param){

    IplImage* workimg = (IplImage*) param;

    switch(event){

        /** mouse movement determines the box's width and height after a left
        *  mouse click has been detected. We know a left mouse click was
        *  previously detected since drawing_box is set to true.
        */
        case CV_EVENT_MOUSEMOVE:
            if(drawing_box){
                box.width = x - box.x;
                box.height = y - box.y;
            }
            break;

        /* left mouse button down starts drawing a box */
        case CV_EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = cvRect(x, y, 0, 0);
            break;

        /* left mouse button up stops drawing a box */
        case CV_EVENT_LBUTTONUP:
            drawing_box = false;

            if(box.width < 0){
                box.x += box.width;
                box.width *= -1;
            }

            if(box.height < 0){
                box.y += box.height;
                box.height *= -1;
            }

            draw_box(workimg, box);
            break;
    } // end switch
}


/** Mouse events are handled by a callback function. OpenCV will call this
*   function whenever a mouse event occurs.
*
*/
int main(int argc, char** argv){

    /** struct CvRect - Stores coordinates of a rectangle.
    *
    *   Call function cvRect to construct a CvRect structure.
    *
    *   CvRect cvRect(int x, int y, int width, int height)
    *
    *   Parameters:
    *       x – x-coordinate of the top-left corner.
    *       y – y-coordinate of the top-left corner (sometimes bottom-left corner).
    *       width – width of the rectangle.
    *       height – height of the rectangle.
    */

    /** Initialize box to an invalid x, y pixel locations. Make the box area zero.*/
    box = cvRect(-1, -1, 0, 0);

    /** Create a 200 x 200 pixel image, pixel data is 8 bit unsigned, 3 channels
    *
    *   IplImage* cvCreateImage(CvSize size, int depth, int channels)
    */

    IplImage* img = cvCreateImage( cvSize(640, 480), IPL_DEPTH_8U, 3);

    /** Let's verify memory was allocated. We will terminate the program in this case.
    *   We need to release the memory allocated for the colorimg before terminating the program */
    if(img == NULL){
        printf("No memory allocated for img\n");
        return -1;
    }

    /** Initialize the image pixel data to all black pixels. Calling cvSetZero will
    *   clear the data array and set all values to 0.
    *
    *   void cvSetZero(CvArr* arr)
    */
    cvSetZero(img);


    /** Create a working copy of our image
    *
    *   IplImage* cvCloneImage(const IplImage* image)
    *
    *   Makes a full copy of an image, including the header, data, and ROI.
    */
    IplImage* tempimg = cvCloneImage(img);

    if(tempimg == NULL){
        printf("No memory allocated for tempimg\n");
        cvReleaseImage(&img);                          // avoid memory leak
        return -1;
    }

    /** Create a window to display our image */
    cvNamedWindow("Box Example", CV_WINDOW_AUTOSIZE);

    /** Register the mouse event callback function. Each time there is a mouse
    *   event, we want OpenCV to call our function that handles mouse events.
    *
    *   Above, we created a function called mouse_callback. Here we need to call
    *   OpenCV's function cvSetMouseCallback to register our function mouse_callback.
    *
    *   void cvSetMouseCallback(const char* window_name, CvMouseCallback on_mouse,
                                void* param=NULL )
    *
    *   Description: Sets mouse handler for the specified window
    *
    *   Parameters:
    *       winname – Window name
    *       onMouse – Mouse callback.
    *       userdata – The optional parameter passed to the callback.
    *
    *   We want to associate mouse events with our window named "Box Example"
    *   Our mouse callback function is named "on_mouse"
    *   We want OpenCV to pass our IplImage pointer img to the mouse callback function.
    */
    cvSetMouseCallback("Box Example", on_mouse, (void*) img);


    /** Now, we want the program to loop until the user presses the escape key */
    while(1){

        /** make a copy of the original image and store it in temporary working image
        *
        *   void cvCopy(const CvArr* src, CvArr* dst, const CvArr* mask=NULL)
        *
        *   Parameters:
        *       src – The source array
        *       dst – The destination array
        *       mask – Operation mask, 8-bit single channel array;
        *              specifies elements of the destination array to be changed
        *
        *   The function copies selected elements from an input array to an output array.
        */
        cvCopyImage(img, tempimg);

        if(drawing_box)
            draw_box(tempimg, box);

        cvShowImage("Box Example", tempimg);
        if((char)cvWaitKey(30) == 27)
            break;
    } // end while


    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseImage(&img);
    cvReleaseImage(&tempimg);
    cvDestroyWindow("Box Example");

    return 0;
}
