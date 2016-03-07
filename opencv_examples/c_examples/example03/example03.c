/** Filename: example03.c
*
*   Description: opens a color image file named in argv[1]
*   displays the image, displays each color channel.
*   Press the escape key to end the program.
*
*   This program uses the same code as example01.c and further
*   examines the IplImage struct. Several of the struct's data
*   members are accessed and displayed.
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
        printf("Usage: ./example03 imageName");
        return -1;
    }

    /** Purpose: read an input image file
    *
    *   opencv online documentation: http://docs.opencv.org/
    *
    *   IplImage* cvLoadImage(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR )
    *
    *   Parameters:
    *       filename – Name of file to be loaded.
    *       flags –
    *
    *               Flags specifying the color type of a loaded image:
    *
    *               CV_LOAD_IMAGE_ANYDEPTH - If set, return 16-bit/32-bit image when the input has
    *                                        the corresponding depth, otherwise convert it to 8-bit.
    *               CV_LOAD_IMAGE_COLOR - If set, always convert image to the color one
    *               CV_LOAD_IMAGE_GRAYSCALE - If set, always convert image to the grayscale one
    *
    *               >0 Return a 3-channel color image. Alpha channel is removed.
    *
    *               =0 Return a grayscale image.
    *               <0 Return the loaded image as is (with alpha channel).
    *
    *   Returns a pointer to the image file. The image is stored in an IplImage structure.
    *   If the image cannot be opened, NULL is returned.
    */

    /** Note: loading the image with this C function dynamically creates memory for the image data.
    *   Later we will need to release the image to free this memory.
    */
    IplImage* src = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);

    if(src == NULL){

        printf("File %s not opened, program ending\n", argv[1]);
        return -1;
    }

    /** Display the loaded image */
    cvNamedWindow("source", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    cvShowImage("source", src);

    /** Display each of the color channels separately. A color image will have 3 channels.
    *
    *   Create pointer variables for each of the color channels and an image header for
    *   each color channel.
    */

    /** CvSize cvSize(int width, int height)
    *
    *   constructs CvSize structure.
    *   Parameters:
    *       width – width of the rectangle.
    *       height – height of the rectangle.
    *
    */
    CvSize s = cvSize(src->width, src->height);
    int d = src->depth;


    /** IplImage* cvCreateImage(CvSize size, int depth, int channels)
    *
    *   Creates an image header but does not allocate the image data.
    *
    *   Parameters -
    *       size – Image width and height
    *       depth – Image depth
    *       channels – Number of channels
    *
    */

    IplImage* blueimg = cvCreateImage(s, d, 1);
    IplImage* greenimg = cvCreateImage(s, d, 1);
    IplImage* redimg = cvCreateImage(s, d, 1);


    /** if there are 3 channels, split the image into each of its channels */
    if(src->nChannels == 3){
        /** void cvSplit(const CvArr* src, CvArr* dst0, CvArr* dst1, CvArr* dst2, CvArr* dst3)
        *
        *   Description: The function splits a multi-channel array into separate single-channel arrays:
        *
        *   Parameters:	src – input multi-channel array.
        *               dst0, dst1, dst2, dst3 - destination arrays
        */

        /*** IMPORTANT: OpenCV stores data in the order blue, green, red (BGR) */
        cvSplit(src, blueimg, greenimg, redimg, NULL);

    }
    else{
        printf("\nImage does not have 3 channels\n");
        printf("Number of channels: %d\n", src->nChannels);
    }

    cvNamedWindow("blue channel", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    cvNamedWindow("green channel", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    cvNamedWindow("red channel", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);

    cvShowImage("blue channel", blueimg);
    cvShowImage("green channel", greenimg);
    cvShowImage("red channel", redimg);


    // Wait forever so that we can see displayed window. Pressing the escape key will end this program.
    cvWaitKey(0);


    /*** void cvReleaseImage(IplImage** image)
    *
    *    Deallocates the image header and image data.
    *
    *    Call when you are done with an image to free the dynamically allocated memory.
    */
    cvReleaseImage(&src);
    cvReleaseImage(&blueimg);
    cvReleaseImage(&greenimg);
    cvReleaseImage(&redimg);

    /** We are done with the image, return the memory allocated for the window to the operating system.
    *   void cvDestroyWindow(const char* name)
    */
    cvDestroyWindow("source");
    cvDestroyWindow("blue channel");
    cvDestroyWindow("green channel");
    cvDestroyWindow("red channel");
    return 0;
}
