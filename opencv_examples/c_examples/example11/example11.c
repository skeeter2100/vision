/** Filename: example11.c
*
*  Description: Use a mouse to draw rectangles on an image.
*               Display color value statistics for each rectangular area.
*
*
*  DRAWING:
*  To draw a red rectangle on the image, hold the left mouse button down
*  and move the mouse to draw the rectangle. Release the left button to
*  complete the rectangle. The pixel location of the upper left corner of
*  the drawn rectange appears on the image.
*
**
*  ERASING:
*  To erase the last rectangle drawn, ctrl-click left button.
*
*
**
*   Press the escape key when you are done drawing on the images.
**
*   Standard Output:
*
*   For each rectangle drawn on the image, the upper left and lower right
*   coordinates are displayed. This rectangular area is sampled. The average,
*   standard deviation and min/max pixel values for the sampled area are displayed.
*
**************************************************************************************
*   Functionality to add to the program:
*
*   Draw circles on the image with the right mouse button.
*   Display the coordinates of the centers of circles. Sample the circular color space
*   and compute average, std. dev, min/max values
*
*   Save a copy of the image with the drawn shapes and coordinates displayed on that image.
*
*   Write the output to a file that can be parsed to extract the color space statistics
*   as input for color space filtering.
*
*
*   Experiment with setting the rectangular region of interest and then calling cvAvg and
*   standard deviation functions. Compare those results to calculated stats.
*
*
*
*/


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <string>


/** Create a struct data type to store information about a circle */

typedef struct CircleType
{
    int centerx;
    int centery;
    int radius;
} myCircleType;



/** Global Variables */

/* Declaring the pointer to the original image as global, so it can be accessed in
*  the redraw_image function. The redraw_image function is called from the mouse
*  callback function when a box or circle is erased. Can only pass 1 image to
*  the mouse callback function. The image passed is the work image. Erasing
*  needs to copy the original image so the box or circle pixels erased are
*  restored back to the original image pixel values.
*/
IplImage *pOriginalImage;

/** Drawing box variables */
CvRect box;                     /// CvRect(int x, int y, int width, int height)
bool drawing_box;               /// bool is a c++ true/false data type
bool erasing_box;


/** Drawing circle variables */
myCircleType aCircle;
bool drawing_circle;
bool erasing_circle;


/** c++ class vector
*
*   template < class T, class Alloc = allocator<T> > class vector; // generic template
*   Vectors are sequence containers representing arrays that can change in size.
*
*   Just like arrays, vectors use contiguous storage locations for their elements,
*   which means that their elements can also be accessed using offsets on regular
*   pointers to its elements, and just as efficiently as in arrays. But unlike arrays,
*   their size can change dynamically, with their storage being handled automatically
*   by the container.
*
*   Using this container so we do not have to worry about dynamic memory allocation
*   or a maximum number of boxes/circles we can draw on the image if we had statically
*   allocated the array size.
*/
std::vector<CvRect> boxes_drawn;
std::vector<myCircleType> circles_drawn;


/* Function Prototypes */
void draw_box( IplImage *img, CvRect rect);
void draw_circle(IplImage *img, myCircleType circ);
void redraw_image(IplImage *img, std::vector<CvRect> boxes_drawn);
void my_mouse_callback(int event, int x, int y, int flags, void* param);
bool parse_command_line(int argc, char** argv, std::string &imageName);
void pause_program();
void extract_BGR_colors_from_rectangularROI(IplImage* bgrImagePtr, cv::Point upperLeft, cv::Point lowerRight);
void extract_HSV_colors_from_rectangularROI(IplImage* hsvImagePtr, cv::Point upperLeft, cv::Point lowerRight);




int main(int argc, char** argv)
{
    int i, stop;

    /** string is a c++ class. It is a character array, but has several library
    *   functions that are more convenient to use when dealing with character
    *   arrays than the c string.h library
    */
    std::string inputImageName;

    /** CvRect(int x, int y, int width, int height) */
    box = cvRect(-1, -1, 0, 0);                     /// initialize box vales

    /** OpenCV has several drawing functions. One lets you put text on an image.
    *
    *  Must initialize the font structure for the text rendering functions
    *
    *  hscale - scales the character width. 1 maintains orginal
    *  vscale - scales the character height
    */
    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    int    lineWidth=1;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);


    /** Create pointers for the working images */
    IplImage *ptempImage, *pworkImage, *phsvImage;

    /// Initialize all pointers to NULL
    ptempImage = pworkImage = pOriginalImage = phsvImage = NULL;

    if (!parse_command_line(argc, argv, inputImageName))
        return -1;

    /** IplImage* cvLoadImage( const char* filename, int flags=CV_LOAD_IMAGE_COLOR );

        filename -  Name of file to be loaded.
        flags -     Specifies colorness and depth of the loaded image:

    *  cvLoadImage determines the file format to be loaded based on the filename
       and automatically allocates memory needed for that data structure
    */

    /** In the call to cvLoadImage, the first argument must be a const char*.
    *   Because the string data type is not a const char*, we call the string
    *   class's member funtion .c_str() to return the string as a const char * type.
    */
    pOriginalImage = cvLoadImage(inputImageName.c_str(),CV_LOAD_IMAGE_COLOR);

    if (pOriginalImage == NULL){
        fprintf(stderr, "cvLoadImage returned a null pointer\n");
        return -1;
    }

    // Display Image information
    fprintf (stderr, "\nOriginal Image: width = %d\nheight = %d\nwidthStep = %d\nnumber of channels = %d\n", pOriginalImage->width,
                pOriginalImage->height, pOriginalImage->widthStep, pOriginalImage->nChannels);
    fprintf(stderr, "pixel depth in bits = %d\ncolor model = %s\nchannel sequence = %s\nimage data size in bytes = %d\n\n",
                pOriginalImage->depth, pOriginalImage->colorModel, pOriginalImage->channelSeq,
                pOriginalImage->imageSize);

    /** Display the original image */
    cvNamedWindow(inputImageName.c_str(), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    cvShowImage(inputImageName.c_str(), pOriginalImage);

    pause_program();

    /** Make 2 copies of original image */
    ptempImage = cvCloneImage(pOriginalImage);
    if(!ptempImage)
    {
        fprintf(stderr, "ptempImage = NULL, program terminating\n");
        cvReleaseImage(&pOriginalImage);
        return -1;
    }

    pworkImage = cvCloneImage(ptempImage);
    if(!pworkImage)
    {
        fprintf(stderr, "pworkImage = NULL, program terminating\n");
        cvReleaseImage(&pOriginalImage);
        cvReleaseImage(&ptempImage);
        return -1;
    }

    phsvImage = cvCloneImage(pOriginalImage);
    if(!phsvImage)
    {
        fprintf(stderr, "phsvImage = NULL, program terminating\n");
        cvReleaseImage(&pOriginalImage);
        cvReleaseImage(&ptempImage);
        cvReleaseImage(&pworkImage);
        return -1;
    }

    /** Create a window to interact with the mouse */
    cvNamedWindow("Mouse Draw", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);


    /** Register our mouse callback function
    *
    *   cvSetMouseCallback(const char* windoow_name, CvMouseCallback on_mouse, void* param CV_DEFAULT(NULL))
    *
    *   window_name -   name of window to which callback will be attached,
    *                   only events in this window will trigger the callback function
    *
    *   The image that will be passed to the mouse callback function will be our pworkImage.
    */
    cvSetMouseCallback("Mouse Draw", my_mouse_callback, (void*) pworkImage);

    /** initialize drawing states */
    erasing_box = erasing_circle = drawing_box = drawing_circle = false;


    fprintf(stdout, "\nUse left mouse button to draw on Mouse Draw window\n"
                    "Press escape key when done drawing\n\n");

    /** Loop will continue execution until the user presses the escape key */
    while(1){

        if(!erasing_box)
        {
            /// copy the work image to the temp image
            cvCopyImage(pworkImage, ptempImage);

            /** perform all drawing on the temp image */
            if(drawing_box)
            {
                draw_box(ptempImage, box);

                /** Display rectangle coordinates, upper left corner while drawing box
                *
                *   void cvPutText( CvArr* img, const char* text, CvPoint org, CvFont* font, int color );
                *
                *   org - Coordinates of the bottom-left corner of the first letter.
                */
                char textBuffer[16];

                /** int snprintf ( char * s, size_t n, const char * format, ... );
                *
                *   Write formatted output to sized buffer
                *   Composes a string with the same text that would be printed if format
                *   was used on printf, but instead of being printed, the content is stored
                *   as a C string in the buffer pointed by s (taking n as the maximum buffer
                                                              capacity to fill).
                *
                *   If the resulting string would be longer than n-1 characters, the
                *   remaining characters are discarded and not stored, but counted for
                *   the value returned by the function.
                *
                *   A terminating null character is automatically appended after the content
                *   written.
                *
                *   Parameters
                *       s   Pointer to a buffer where the resulting C-string is stored.
                *           The buffer should have a size of at least n characters.
                *
                *       n   Maximum number of bytes to be used in the buffer.
                *           The generated string has a length of at most n-1, leaving space
                *           for the additional terminating null character
                *
                *       format C string that contains a format string that follows the same
                *           specifications as format in printf.
                */
                snprintf(textBuffer, 15, "(%3d, %3d)", box.x, box.y);

                /** void cvPutText(CvArr* img, const char* text, CvPoint org, const CvFont* font, CvScalar color)
                *
                *   img – Image.
                *   text – Text string to be drawn.
                *   org – Bottom-left corner of the text string in the image.
                *   font – CvFont structure initialized using InitFont().
                *   color - CvScalar structure, color of text
                */
                cvPutText (ptempImage,textBuffer,cvPoint(50,50), &font, cvScalar(0,0,0));
            }

        }

        cvShowImage("Mouse Draw", ptempImage);

        /** Will wait 100 ms for a user key press.
        *   while(1) loop stops when user presses the esc key, ascii 27
        */
        if( (char)cvWaitKey(100) == 27 )   break;

    } /// end while(1)

    /** After user presses escape key, retrieve the color information from the boxe areas */

    /** vector class has a member function size that returns the number of items stored in
    *   the vector container */
    stop = boxes_drawn.size();

    for(i = 0; i < stop; ++i)
    {
        cv::Point upperLeft(boxes_drawn[i].x, boxes_drawn[i].y);
        cv::Point lowerRight((boxes_drawn[i].x+boxes_drawn[i].width),
                                            (boxes_drawn[i].y+boxes_drawn[i].height));
        fprintf(stderr, "\n****** Rectangle Statistics ******\n\n");
        fprintf(stderr, "Box number %d, UpperLeft (%d, %d), LowerRight (%d, %d)\n", i, upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);

        extract_BGR_colors_from_rectangularROI(pworkImage, upperLeft, lowerRight);
        cvCvtColor(pworkImage, phsvImage, CV_BGR2HSV);
        extract_HSV_colors_from_rectangularROI(pworkImage, upperLeft, lowerRight);

    }

    /** Save a copy of the working image with the boxes */
    cvSaveImage("workImage.jpg", pworkImage);

    /** Stop program to allow user to see color information */
    pause_program();

    cvReleaseImage(&pOriginalImage);
    cvReleaseImage(&ptempImage);
    cvReleaseImage(&pworkImage);
    cvDestroyAllWindows();

    return 0;
}


/** Define a callback function for mouse events, must match the prototype
*
*  void CvMouseCallback( int event, int x, int y, int flags, void* param);
*
*       events are such things as mouse movement and button clicks
*       x and y represent the pixel in an emage, independent of window size
*       flags - individual bits indicate shift, ctrl, and alt keys
*
*                   Flag                        Numerical Value
*               CV_EVENT_FLAG_LBUTTON                   1
*               CV_EVENT_FLAG_RBUTTON                   2
*               CV_EVENT_FLAG_MBUTTON                   4
*               CV_EVENT_FLAG_CTRLKEY                   8
*               CV_EVENT_FLAG_SHIFTKEY                  16
*               CV_EVENT_FLAG_ATLKEY                    32
*
*
*
*   Description:
*       Pressing the left mouse button starts a box
*       Releasing the left button adds the box to the current image.
*       Dragging the mouse with left button down resizes the box.
*
*/

void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
    IplImage* image = (IplImage*) param;

    // all possible mouse events are listed below, but many are ignored
    switch(event)
    {
        case CV_EVENT_MOUSEMOVE:
            if(drawing_box)
            {
                box.width =  x - box.x;
                box.height = y - box.y;
            }
            break;


        case CV_EVENT_LBUTTONDOWN:

            /** ctrl + left mouse down initiates erasing the last box drawn */
            if( (flags & 0x8) == CV_EVENT_FLAG_CTRLKEY )
            {
                if(!boxes_drawn.empty())
                    erasing_box = true;
            }
            else        // left mouse button down starts drawing a new box
            {
                drawing_box = true;
                box = cvRect(x, y, 0, 0);
            }
            break;

        case CV_EVENT_RBUTTONDOWN:
            break;
        case CV_EVENT_MBUTTONDOWN:
            break;

        case CV_EVENT_LBUTTONUP:
            // ctrl key plus left button up erases the last drawn box
            if( erasing_box )
            {
                erasing_box = false;
                if(!boxes_drawn.empty())
                {
                    /** remove the last box object stored in the vector container */
                    boxes_drawn.pop_back();
                    redraw_image(image, boxes_drawn);
                }
            }
            else  // box is complete, draw it
            {
                drawing_box = false;
                if(box.width < 0)
                {
                    box.x += box.width;
                    box.width *= -1;
                }
                if(box.height < 0)
                {
                    box.y += box.height;
                    box.height *= -1;
                }
                draw_box(image, box);
                boxes_drawn.push_back(box);
            }
            break;

        case CV_EVENT_RBUTTONUP:
            break;
        case CV_EVENT_MBUTTONUP:
            break;
        case CV_EVENT_LBUTTONDBLCLK:
            break;
        case CV_EVENT_RBUTTONDBLCLK:
            break;
        case CV_EVENT_MBUTTONDBLCLK:
            break;
    }
}


void draw_box( IplImage *img, CvRect rect)
{
    /** void cvRectangle(CvArr* img, CvPoint pt1, CvPoint pt2,
                         CvScalar color, int thickness=1, int line_type=8,
                         int shift=0 )
    *   Parameters:
    *   cvScalar is color of rectangle (b, g, r) bright red
    *   thickness is line thickness, negative value fills rectangle */
    cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height),
                cvScalar(0x00, 0x00, 0xff), 8 );

}

/// We have to redraw all boxes except the one erased.
void redraw_image(IplImage *img, std::vector<CvRect> boxes_drawn)
{
    IplImage *pEraseImage = cvCloneImage(pOriginalImage);
    if(!pEraseImage)
        return;

    int i = 0;

    while(i < (int)boxes_drawn.size())
    {
        draw_box(pEraseImage, boxes_drawn[i]);
        ++i;
    }

    cvCopyImage(pEraseImage, img);
    cvReleaseImage(&pEraseImage);
}



/** Input: bgrImagePtr is a pointer to an image in the bgr color space
*       upperLeft are the x,y coordinates of upper left corner of a rectangle
*       lowerRight are the x,y coordinates of lower right corner of a rectangle
*
*  Output: writes the following information to the stderr stream
*
*  Average values and standard deviation of red, green, and blue
*  Min and max values of red, green, and blue
*
*  Note: all the pixels in the rectangular area are sampled
*/
void extract_BGR_colors_from_rectangularROI(IplImage* bgrImagePtr, cv::Point upperLeft, cv::Point lowerRight)
{
    int row_size = bgrImagePtr->widthStep;
    char *pixel_data = bgrImagePtr->imageData;     // pointer to start of image data

    /** Initialize min, max values. Assumes we are working with images that
    *   use 8 bits for each color channel. The color channel data type is an
    *   unsigned char.
    */
    unsigned char maxBlue, maxGreen, maxRed;
    unsigned char minBlue, minGreen, minRed;

    unsigned long sumBlue = 0;
    unsigned long sumGreen = 0;
    unsigned long sumRed = 0;

    unsigned int pixelCount = 0;

    double avgBlue, avgGreen, avgRed;

    int startrow = upperLeft.y;
    int stoprow = lowerRight.y;
    int startcol = upperLeft.x;
    int stopcol = lowerRight.x;

    // Get data from rectangle
    for(int row = startrow; row <= stoprow; ++row)
    {
	    for(int col = startcol; col <= stopcol; ++col)
	    {
            /** Get the BGR pixel color values. NOTE that OpenCV stores RGB pixels in B,G,R order.
            *   Multiplying col*3 assumes there are only 3 color channels, no fourth alpha channel
            */
            unsigned char *pBGR= (unsigned char*)(pixel_data + row*row_size + col*3);
            unsigned char blue = *(unsigned char*)(pBGR+0);
            unsigned char green = *(unsigned char*)(pBGR+1);
            unsigned char red = *(unsigned char*)(pBGR+2);

            fprintf(stderr, "row: %d, col: %d, blue: %u, green: %u, red: %u, \n",
                        row, col, blue, green, red);
            pause_program();

            sumBlue += blue;
            sumGreen += green;
            sumRed += red;
            ++pixelCount;

            // Find min, max

            /** Initialize min, max values to the first pixel in the region.
            *   The first pixel is found when row == startrow AND col == startcol
            *
            *   Using DeMorgan's law to find the complement of this condition,
            *   NOT (first pixel) which is NOT( row == startrow AND col == startcol )
            *
            *   becomes row NOT equal startrow OR col NOT equal startcol
            */
            if(row != startrow || col != startcol)
            {
                // not first time through loops
                if(blue < minBlue)      minBlue = blue;
                if(blue > maxBlue)      maxBlue = blue;
                if(green < minGreen)    minGreen = green;
                if(green > maxGreen)    maxGreen = green;
                if(red < minRed)        minRed = red;
                if(red > maxRed )       maxRed  = red;

            }
            else /** Initialize min and max to first values found */
            {
                minBlue = blue;
                maxBlue = blue;
                minGreen = green;
                maxGreen = green;
                minRed = red;
                maxRed = red;
                // To verify this action only happens once
                /* fprintf(stderr, "\n\n%s, I love DeMorgan's Law, this only executes once per function call\n\n",
                        __PRETTY_FUNCTION__);
                */
            }
	    }  // end for col
    }   // end for row

    // Calculate average values
    avgBlue = (double)sumBlue / (double)pixelCount;
    avgGreen = (double)sumGreen / (double)pixelCount;
    avgRed = (double)sumRed / (double)pixelCount;

    // calculate variance
    double varBlue, varGreen, varRed;
    varBlue = varGreen = varRed = 0;

    /** Variance is sum of squared differences divided by the pixel count
    *
    *   Find the difference of each pixel from the mean (average) value.
    *   Square the difference
    *   Add the squared difference to the sum.
    */
    for(int row = startrow; row <= stoprow; ++row)
    {
        for(int col = startcol; col <= stopcol; ++col)
        {
            unsigned char *pBGR = (unsigned char*)(pixel_data + row*row_size + col*3);
            varBlue += pow( (double) ((*(unsigned char*)(pBGR+0))-avgBlue),2);
            varGreen += pow((double) ((*(unsigned char*)(pBGR+1))-avgGreen),2);
            varRed += pow( (double) ((*(unsigned char*)(pBGR+2))-avgRed),2);
        }
    }

    // Divide sum of squared differences by pixelCount to find variance
    varBlue /= pixelCount;
    varGreen /= pixelCount;
    varRed /= pixelCount;

    /** standard deviation is the square root of the variance **/
    fprintf(stderr, "\nBGR Color Statistics\n\n");
    fprintf(stderr, "Pixel Count = %d\n", pixelCount);

    fprintf(stderr, "avgBlue = %d, avgGreen = %d, avgRed = %d\n",
            (int)avgBlue, (int)avgGreen, (int)avgRed);
    fprintf(stderr, "stdDevBlue = %5.2f, stdDevGreen = %5.2f, stdDevRed = %5.2f\n",
            sqrt(varBlue), sqrt(varGreen), sqrt(varRed));

    fprintf(stderr, "minBlue = %d, maxBlue = %d\n", minBlue, maxBlue);
    fprintf(stderr, "minGreen = %d, maxGreen = %d\n", minGreen, maxGreen);
    fprintf(stderr, "minRed = %d, maxRed = %d\n", minRed, maxRed);

}



/** Input: bgrImagePtr is a pointer to an image in the bgr color space
*       upperLeft are the x,y coordinates of upper left corner of a rectangle
*       lowerRight are the x,y coordinates of lower right corner of a rectangle
*
*  Output: writes the following information to the stderr stream
*
*  Average values and standard deviation of red, green, and blue
*  Min and max values of red, green, and blue
*
*  Note: all the pixels in the rectangular area are sampled
*/
void extract_HSV_colors_from_rectangularROI(IplImage* hsvImagePtr, cv::Point upperLeft, cv::Point lowerRight)
{
    int row_size = hsvImagePtr->widthStep;
    char *pixel_data = hsvImagePtr->imageData;     // pointer to start of image data

    /** Initialize min, max values. Assumes we are working with images that
    *   use 8 bits for each color channel. The color channel data type is an
    *   unsigned char.
    */
    unsigned char maxhue, maxval, maxsat;
    unsigned char minhue, minval, minsat;

    unsigned long sumhue = 0;
    unsigned long sumsat = 0;
    unsigned long sumval = 0;

    unsigned int pixelCount = 0;

    double avghue, avgsat, avgval;

    int startrow = upperLeft.y;
    int stoprow = lowerRight.y;
    int startcol = upperLeft.x;
    int stopcol = lowerRight.x;

    // Get data from rectangle
    for(int row = startrow; row <= stoprow; ++row)
    {
	    for(int col = startcol; col <= stopcol; ++col)
	    {
            /** Get the BGR pixel color values. NOTE that OpenCV stores RGB pixels in B,G,R order.
            *   Multiplying col*3 assumes there are only 3 color channels, no fourth alpha channel
            */
            unsigned char *pHSV= (unsigned char*)(pixel_data + row*row_size + col*3);
            unsigned char hue = *(unsigned char*)(pHSV+0);
            unsigned char sat = *(unsigned char*)(pHSV+1);
            unsigned char val = *(unsigned char*)(pHSV+2);

            fprintf(stderr, "row: %d, col: %d, hue: %u, sat: %u, val: %u, \n",
                        row, col, hue, sat, val);
            pause_program();

            sumhue += hue;
            sumsat += sat;
            sumval += val;
            ++pixelCount;

            // Find min, max

            /** Initialize min, max values to the first pixel in the region.
            *   The first pixel is found when row == startrow AND col == startcol
            *
            *   Using DeMorgan's law to find the complement of this condition,
            *   NOT (first pixel) which is NOT( row == startrow AND col == startcol )
            *
            *   becomes row NOT equal startrow OR col NOT equal startcol
            */
            if(row != startrow || col != startcol)
            {
                // not first time through loops
                if(hue < minhue)      minhue = hue;
                if(hue > maxhue)      maxhue = hue;
                if(sat < minsat)      minsat = sat;
                if(sat > maxsat)      maxsat = sat;
                if(val < minval)        minval = val;
                if(val > maxval )       maxval  = val;

            }
            else /** Initialize min and max to first values found */
            {
                minhue = hue;
                maxhue = hue;
                minsat = sat;
                maxsat = sat;
                minval = val;
                maxval = val;
                /// To verify this action only happens once
                fprintf(stderr, "\n\n%s, I love DeMorgan's Law, this only executes once per function call\n\n",
                        __PRETTY_FUNCTION__);

            }
	    }  /// end for col
    }   /// end for row

    /// Calculate average values
    avghue = (double)sumhue / (double)pixelCount;
    avgsat = (double)sumsat / (double)pixelCount;
    avgval = (double)sumval / (double)pixelCount;

    // calculate variance
    double varhue, varsat, varval;
    varhue = varsat = varval = 0;

    /** Variance is sum of squaval differences divided by the pixel count
    *
    *   Find the difference of each pixel from the mean (average) value.
    *   Square the difference
    *   Add the squaval difference to the sum.
    */
    for(int row = startrow; row <= stoprow; ++row)
    {
        for(int col = startcol; col <= stopcol; ++col)
        {
            unsigned char *pBGR = (unsigned char*)(pixel_data + row*row_size + col*3);
            varhue += pow( (double) ((*(unsigned char*)(pBGR+0))-avghue),2);
            varsat += pow((double) ((*(unsigned char*)(pBGR+1))-avgsat),2);
            varval += pow( (double) ((*(unsigned char*)(pBGR+2))-avgval),2);

        }
    }

    // Divide sum of squared differences by pixelCount to find variance
    varhue /= pixelCount;
    varsat /= pixelCount;
    varval /= pixelCount;

    /** standard deviation is the square root of the variance **/
    fprintf(stderr, "\nHSV Color Statistics\n\n");
    fprintf(stderr, "Pixel Count = %d\n", pixelCount);

    fprintf(stderr, "avghue = %d, avgSat = %d, avgVal= %d\n",
            (int)avghue, (int)avgsat, (int)avgval);
    fprintf(stderr, "stdDevhue = %5.2f, stdDevSat = %5.2f, stdDevVal = %5.2f\n",
            sqrt(varhue), sqrt(varsat), sqrt(varval));

    fprintf(stderr, "minhue = %d, maxhue = %d\n", minhue, maxhue);
    fprintf(stderr, "minsat = %d, maxsat = %d\n", minsat, maxsat);
    fprintf(stderr, "minval = %d, maxval = %d\n", minval, maxval);

}

/**\fn bool parse_command_line(int argc, char** argv, std::string &imageName)
*
*\param[in] argc    number of command line arguments
*\param[in] argv    array of command line strings
*\param[out] imageName  argv[1] is the image name
*
*\return Success::
*       Returns true when there are 2 command line arguments.
*       Stores argv[1] in imageName.
*
*   Failure: returns false when there is only 1 command line argument.
*
*   For the c++ string object imageName, the & denotes it is passed as a reference
*   Passing it by reference means any changes we make to the objects memory in
*   this function will not be lost when the function returns. It is similar to
*   the C idea of passing an argument as a pointer.
*/
bool parse_command_line(int argc, char** argv, std::string &imageName)
{
    if (argc < 2){
        fprintf(stderr, "mouse example program Usage: Enter 2 arguments on the command line,\n");
        fprintf(stderr, "./example11 --file filename\n");
        return false;
    }

    imageName = argv[1];
    return true;
}

void pause_program()
{
	printf("Press enter key to continue...");
	getchar();
}
