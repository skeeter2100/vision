/** Filename: example04.c
*
*   Description: opens a color image file named in argv[1],
*   converts the color image to grayscale. Displays both the
*   color and grayscale images. Press the escape key to end the program.
*
*/

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


int main(int argc, char** argv){

    /** Two arguments must be passed to the program.
    *   argv[0] is the name of the executable
    *   argv[1] is the name of the image file to open
    */
    if(argc < 2){
        printf("Usage: ./example04 imageName");
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

    /** We want to convert our color image to a grayscale image. First, let's declare an
    *   IplImage pointer for our grayscale image. */
    IplImage* grayimg;

    /** Next, we need to allocate memory to store the grayscale image. Use the function
    *
    *   IplImage* cvCreateImage(CvSize size, int depth, int channels)
    *
    *   Parameters:
    *       size – Image width and height
    *       depth – Bit depth of image elements. See IplImage for valid depths.
    *       channels – Number of channels per pixel. See IplImage for details.
    *
    *   This function creates memory to store all the data members of the IplImage struct
    *   type, including memory for the pixel data.
    *
    *   What is the size of the grayscale image? It will be the same pixel width and height
    *   as the color image. The data type CvSize is a struct that stores the size of an
    *   image or rectangle. It has two data members, width and height. To create a variable of
    *   type CvSize, we can call the function CvSize cvSize(int width, int height)
    *
    */

    /** Remember, colorimg is a pointer to an IplImage struct. To access the struct's data
    *   members, use the notation -> */
    CvSize s = cvSize(colorimg->width, colorimg->height);

    /** The grayscale image depth will be the same as the color image's depth. Depth refers to
    *   the number of bits of the image pixels. Typically, we will work with 8 bit unsigned
    *   integers. OpenCV's enumeration for this depth type is IPL_DEPTH_8U.
    */
    int d = colorimg->depth;

    /** Now, let's call the cvCreateImage function, passing our size and depth parameters.
    *   The channel parameter will be 1. Grayscale only has 1 intensity channel for pixel values.
    */
    grayimg = cvCreateImage(s, d, 1);

    /** Alternatively, we could have called the function like this
    * grayimg = cvCreateImage( cvSize(colorimg->width, colorimg->height), colorimg->depth, 1);
    */

    /** Let's verify memory was allocated. We will terminate the program in this case.
    *   We need to release the memory allocated for the colorimg before terminating the program */
    if(grayimg == NULL){
        printf("No memory allocated for grayimg\n");
        cvReleaseImage(&colorimg);                      /// Prevent memory leak
        return -1;
    }

    /** The next step is calling the cvtColor function to access the color image pixel data,
    *   convert these pixels to grayscale and store those grayscale pixels in grayimg data.
    *
    *   We will use OpenCV's convert color function
    *
    *   void cvCvtColor(const CvArr* src, CvArr* dst, int code)
    *
    *   Parameters:
    *   src – input image: 8-bit unsigned, 16-bit unsigned ( CV_16UC... ), or single-precision floating-point.
    *   dst – output image of the same size and depth as src.
    *   code – color space conversion code
    *
    *   We will use the code CV_BGR2GRAY whichs converts an BGR image 2 GRAY
    *   Note that the default color format in OpenCV is often referred to as RGB but it is actually BGR
    *   (the bytes are reversed). So the first byte in a standard (24-bit) color image will be an 8-bit
    *   Blue component, the second byte will be Green, and the third byte will be Red. The fourth, fifth,
    *   and sixth bytes would then be the second pixel (Blue, then Green, then Red), and so on.
    *
    *   How does OpenCV convert color to gray?   Y = 0.299 * R + 0.587 * G + 0.114 B
    *
    */

    cvCvtColor(colorimg, grayimg, CV_BGR2GRAY);

    /** Display the images */
    cvNamedWindow("color", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    cvNamedWindow("gray", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

    /** Let's tell OpenCV where to display the window on our screen with the function
    *
    *   void cvMoveWindow(const char* name, int x, int y)¶
    *
    *   Parameters:
    *   winname – Window name
    *   x – The new x-coordinate of the window
    *   y – The new y-coordinate of the window
    */
    cvMoveWindow("color", 100, 100);
    cvMoveWindow("gray", 500, 100);

    cvShowImage("color", colorimg);
    cvShowImage("gray", grayimg);

    cvWaitKey(0);         /// Display images until user presses a key

    /** Free memory, or as the OpenCV book says: Don't be a piggy, clean up */
    cvReleaseImage(&colorimg);
    cvReleaseImage(&grayimg);
    cvDestroyAllWindows();


    return 0;
}
