/**** example17.c
*
* Plays avi video
*
* Program plays entire video unless user presses the escape key
*
* Usage: ./erun -file video_file_name
*
*/

#include <stdio.h>
#include <highgui.h>

#define ESCAPE_KEY 27


int main(int argc, char** argv)
{

    CvCapture* capture = NULL;
    IplImage* img = NULL;
    unsigned int frameCount = 0;

    if(argc < 2)
    {
        fprintf(stderr, "\nUsage: ./erun <video_file_name>\n");
        return -1;
    }

    capture = cvCaptureFromAVI(argv[1]);

    if(capture == NULL)
    {
        fprintf(stderr, "capture failed\n");
        return -1;
    }
    else
    {
        fprintf(stderr, "\nFilename: %s, got capture\n", argv[1]);
    }


    /** IplImage* cvQueryFrame(CvCapture* capture)
    *
    *   Grabs, decodes and returns the next video frame.
    */
    img = cvQueryFrame(capture);

    /** Verify we can read the first frame */
    if(!img){
        printf("No img\n");
        cvReleaseCapture(&capture);
        return -1;
    }

    /** double cvGetCaptureProperty(CvCapture* capture, int property_id)*/
    double frames_per_sec = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int total_frames_in_video = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

    fprintf(stderr, "frames per sec: %f\n", frames_per_sec);
    fprintf(stderr, "frames in video: %d\n", total_frames_in_video);

    fprintf(stderr, "\nPress enter key to continue ...");
    getchar();

    /** Calculate approximate delay time for while loop cvWaitKey
    *   function to approximate play back at the same speed as the
    *   video was shot */
    //int wait_time_ms = (int)(frames_per_sec * 60.0 / 1000.0);


    /** create a window to display frames */
    cvNamedWindow("source");

    while(img && capture)
    {
        img = cvQueryFrame(capture);
        cvShowImage("flags", img);
        fprintf(stderr, "frameCount: %u\n", ++frameCount);
        //if ( (cvWaitKey(wait_time_ms) & 0xff) == ESCAPE_KEY ) break;
        if ( (cvWaitKey(30) & 0xff) == ESCAPE_KEY ) break;
    }

    /** free memory */
    cvReleaseImage(&img);
    cvReleaseCapture(&capture);
    cvDestroyAllWindows();

    return 0;
}
