#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <cstdio>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat img, gray;
    if( argc != 2 || (img=imread(argv[1], 1)).data==NULL)
        return -1;
    
    Ptr<FeatureDetector> blobs = ORB::create();
    vector<KeyPoint> keypoints;
    blobs->detect(img, keypoints);
    Mat visualizer = img.clone();
    printf("Found %d points\n", keypoints.size());
    for (size_t i = 0; i < keypoints.size(); ++i)
        circle(visualizer, keypoints[i].pt, 4, Scalar(255, 0, 255), -1);
 /*
    cvtColor(img, gray, CV_BGR2GRAY);
    // smooth it, otherwise a lot of false circles may be detected
    GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
                 1.1, 12, 50, 100, 4, 0);

    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
   */
    namedWindow( "circles", WINDOW_NORMAL);
    imshow( "circles", visualizer );
   // imshow("circles", gray);
    
    while( waitKey(30)!=27){
       
    }
    return 0;
}
