/** Filename: example01.c
*
*   Description: opens a color image file named in argv[1]
*   and displays the image. Press the escape key to end the program.
*/

#include <stdio.h>
#include <opencv/highgui.h>

int main(int argc, char** argv){

    /** Two arguments must be passed to the program.
    *   argv[0] is the name of the executable
    *   argv[1] is the name of the image file to open
    */
    if(argc < 2){
        printf("Usage: ./example01 imageName");
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
    IplImage* img = cvLoadImage(argv[1],1);

    if(img == NULL){

        printf("File %s not opened, program ending\n", argv[1]);
        return -1;
    }

    /** int cvNamedWindow(const char* name, int flags=CV_WINDOW_AUTOSIZE )
    *
    *   Creates a window that can be used as a placeholder for images and trackbars.
    *   Created windows are referred to by their names.
    *
    *   Parameters:
    *       name – Name of the window in the window caption that may be used as a window identifier.
    *       flags – Flags of the window. The supported flags are:
    *           WINDOW_NORMAL If this is set, the user can resize the window (no constraint).
    *           WINDOW_AUTOSIZE If this is set, the window size is automatically adjusted to fit
    *           the displayed image and you cannot change the window size manually.
    *
    *       See opencv documentation for additional flags.
    *
    *   When done with the window, call destroyWindow() or destroyAllWindows() to close the window
    *   and de-allocate any associated memory usage.
    *
    */
    cvNamedWindow("Image Window", CV_WINDOW_AUTOSIZE);

    /** Display an image in the specified window
    *
    *   void cvShowImage(const char* name, const CvArr* image)
    *
    *   The function imshow displays an image in the specified window. If the window was created with
    *   the CV_WINDOW_AUTOSIZE flag, the image is shown with its original size. Otherwise, the image
    *   is scaled to fit the window.
    *
    *
    *   Parameters:
    *       name – Name of the window.
    *       image – Image to be shown.
    *
    *   Notice, we are passing an IplImage* to the function argument that requires a const CvArr*
    *       struct CvArr - This is the “metatype” used only as a function parameter.
    *       It denotes that the function accepts arrays of multiple types, such as IplImage*, CvMat*
    *       or even CvSeq* sometimes. The particular array type is determined at runtime by analyzing
    *       the first 4 bytes of the header.
    */

    cvShowImage("Image Window", img);

    /** int cvWaitKey(int delay=0 )
    *
    *       Parameters:	delay – Delay in milliseconds. 0 is the special value that means “forever”.
    *
    *       The function waitKey waits for a key event infinitely (when delay <= 0 ) or for
    *       delay milliseconds, when it is positive. Since the OS has a minimum time between switching
    *       threads, the function will not wait exactly delay ms, it will wait at least delay ms,
    *       depending on what else is running on your computer at that time. It returns the code of the
    *       pressed key or -1 if no key was pressed before the specified time had elapsed.
    *
    *       Note: The function only works if there is at least one HighGUI window created and the
    *       window is active. If there are several HighGUI windows, any of them can be active.
    */

    // Wait forever so that we can see displayed window. Pressing the escape key will end this program.
    cvWaitKey(0);


    /*** void cvReleaseImage(IplImage** image)
    *
    *    Deallocates the image header and image data.
    *
    *    Call when you are done with an image to free the dynamically allocated memory.
    */
    cvReleaseImage(&img);

    /** We are done with the image, return the memory allocated for the window to the operating system.
    *   void cvDestroyWindow(const char* name)
    */
    cvDestroyWindow("example 1");
    return 0;
}
