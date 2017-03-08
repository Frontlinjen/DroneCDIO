#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int sliderPos = 70;
#define RECORD_STREAM 1

void processImage(Mat & image);

int main( int argc, char** argv )
{
    CommandLineParser args(argc, argv, "{help h | | }{@stream |   |   .avi file to load}");
    String str = args.get<String>(0);
    cv::VideoCapture cap;
    if(str != "")
    {
        cap.open(str);
        if(!cap.isOpened())
        {
            printf("%s\n", "Failed to open file");
            return -1;
        }
    }
    else
    {
       cap.open(0);
    }
    
    #if RECORD_STREAM
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(CAP_PROP_FPS);
    int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G'); //cap.get(CAP_PROP_FOURCC);
    cv::VideoWriter writer("sample.avi", fourcc, fps, Size(width, height)); 
    #endif
    Mat image;
   // cv::VideoCapture cap("ringmov.mp4");
    
    #if RECORD_STREAM
    cap.read(image);
    
    #endif
    namedWindow("regular", 1);
    namedWindow("bimage", 1); 
    namedWindow("edgeMap", 1); 
    while(true)
    {
        cap.set(CV_CAP_PROP_POS_FRAMES, 0);
        while(cap.read(image))
        {
            #if RECORD_STREAM
                writer.write(image);
            #endif
             GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT );       
             processImage(image);
             imshow("regular", image);
             waitKey(1);
        }
    }
    #if RECORD_STREAM
    writer.release();
    #endif
    printf("jobs done!");
    // Wait for a key stroke; the same function arranges events processing
    waitKey();
    return 0;
}

// Define trackbar callback functon. This function find contours,
// draw it and approximate it by ellipses.
void processImage(Mat & image)
{
    vector<vector<Point> > contours;
    Mat edgeMap; //= image >= sliderPos;
    Mat tmpX, tmpY, bimage, colorImage;
#ifndef USE_SCHARR
    Sobel(image, tmpX, CV_32FC1, 1, 0, 3);
    Sobel(image, tmpY, CV_32FC1, 0, 1, 3);
#else
    Scharr(image, tmpY, CV_32FC1, 1, 0);
    Scharr(image, tmpY, CV_32FC1, 0, 1);
#endif
    convertScaleAbs(tmpX, edgeMap);
    convertScaleAbs(tmpY, edgeMap);
    imshow("edgeMap", edgeMap);
    GaussianBlur(edgeMap, edgeMap, Size(9, 9), 1, 1, BORDER_DEFAULT );
    //Canny(edgeMap, bimage, 200, 230);
    //imshow("bimage", bimage);

    std::vector<Vec3f> circles;
    Mat cimage;// = Mat::zeros(edgeMap.size(), CV_8UC3);
    cvtColor(edgeMap, cimage, CV_BGR2GRAY);
    HoughCircles(cimage, circles, CV_HOUGH_GRADIENT, 1.0, 20.0, 100.0, 100.0, 0, 5000);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( cimage, center, 3, Scalar(255,255,255), -1, 8, 0 );
         // draw the circle outline
         circle( cimage, center, radius, Scalar(255,0,255), 3, 8, 0 );
    }
    imshow("cimage", cimage);
    return;
}
