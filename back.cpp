#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;
VideoCapture cap(1); 
int main(int argc,const char** argv)

{
    int count=0;
    
    if(!cap.isOpened())  // check if we succeeded
        return -1;
 	Mat back,back2;             
    cap >> back;                                        //getting background
    Mat backfinal1(back.rows,back.cols,CV_8UC1);
    cvtColor(back,back2,COLOR_BGR2GRAY);                
   
    while(count!=back.rows)                             //slicing background and applying otsu for both hue channel and gray image
    {
        Mat backslice=back2(Range(count,count+back.rows/4),Range(0,back.cols));
        GaussianBlur(backslice,backslice,Size(5,5),0,0);        
        threshold(backslice,backslice, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        backfinal1(Range(count,count+back.rows/4),Range(0,back.cols)) = backslice;
        count=count+back.rows/4;
    }
     imwrite("back.JPEG", backfinal1);
}
