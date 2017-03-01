/********************************************************************************
*
*
*  This program is demonstration for ellipse fitting. Program finds
*  contours and approximate it by ellipses.
*
*  Trackbar specify threshold parametr.
*
*  White lines is contours. Red lines is fitting ellipses.
*
*
*  Autor:  Denis Burenkov.
*
*
*
********************************************************************************/
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout <<
        "\nThis program is demonstration for ellipse fitting. The program finds\n"
        "contours and approximate it by ellipses.\n"
        "Call:\n"
        "./fitellipse [image_name -- Default ../data/stuff.jpg]\n" << endl;
}

int sliderPos = 70;


void processImage(Mat & image);

int main( int argc, char** argv )
{
    Mat image;
    cv::VideoCapture cap("ringmov.mp4");
    namedWindow("regular", 1);
    namedWindow("bimage", 1); 
    namedWindow("edgeMap", 1); 
    while(true)
    {
        cap.set(CV_CAP_PROP_POS_FRAMES, 0);
        while(cap.read(image))
        {
             GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT );       
             processImage(image);
             imshow("regular", image);
             waitKey(1);
        }
    }
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
    Canny(edgeMap, bimage, 200, 230);
    imshow("bimage", bimage);
    return;
    findContours(bimage, contours, RETR_LIST, CHAIN_APPROX_NONE);
    /*colorImage = Mat::zeros(tmpY.size(), CV_32FC1);
    MatConstIterator_<float> itlX = tmpX.begin<float>();
    MatConstIterator_<float> itlEnd = tmpX.end<float>();
    MatConstIterator_<float> itlY = tmpY.begin<float>();
    MatIterator_<float> dst =  colorImage.begin<float>();
        for (; itlX != itlEnd; ++itlX, ++itlY, ++dst) {
            *dst = atan2(*itlY, *itlX)/CV_2PI;
        }
    imshow("colormap", colorImage);  */
    
    Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
        if( count < 6 )
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

        ellipse(cimage, box, Scalar(0,0,255), 1, LINE_AA);
        circle(cimage, box.center, 1, Scalar(255, 255, 255));
    }
}
