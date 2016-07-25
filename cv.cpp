#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;
VideoCapture cap(1); 
int main(int argc,const char** argv)
{	int a,b,c,e;
	e=atoi(argv[1]);
    int count=0;
    if(!cap.isOpened())  // check if we succeeded
        return -1;
 
    namedWindow("foreground1", CV_WINDOW_AUTOSIZE); 
    namedWindow("midline", CV_WINDOW_AUTOSIZE); 
    namedWindow("background", CV_WINDOW_AUTOSIZE);
    Mat backfinal1;
    backfinal1=imread("back.JPEG",0);
    
		ofstream afile; 
        Mat hsv,gray,frame,foreground1;
        cap >> frame;               // get a new frame from camera
        
        Mat img1(frame.rows,frame.cols,CV_8UC1);
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        img1=gray;

        int i=0,j;
        while(i!=frame.rows)
        {	 //afile<<"reached";
            Mat img3=gray(Range(i,i+frame.rows/4),Range(0,frame.cols));
            GaussianBlur(img3,img3,Size(5,5),0,0);            
            threshold(img3,img3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);            
            img1(Range(i,i+frame.rows/4),Range(0,frame.cols)) = img3; 
            i=i+frame.rows/4;
        }
        foreground1=img1-backfinal1;          
        string filename="pointset="+to_string(e)+".txt";
        afile.open(filename, ios::in);   

        Mat mid1(frame.rows,frame.cols,CV_8UC1,Scalar(0));
        for(i=0;i<foreground1.rows;i++)                   // This function is to remove noice from the background subtracted image for gray image
        {
            
            a=0;
            b=0;
            c=0;
            for(j=0;j<foreground1.cols;j++)
            {
                
                if(foreground1.at<uchar>(i,j)!=foreground1.at<uchar>(i,j+1))
                {
                    if(((foreground1.at<uchar>(i,j+1)+foreground1.at<uchar>(i,j+2)+foreground1.at<uchar>(i,j+3))<255*3) && ((foreground1.at<uchar>(i,j-1)+foreground1.at<uchar>(i,j-2)+foreground1.at<uchar>(i,j-3))<255*3))
                        continue;
                    
                    if(c==0)
                    {   
                        a=j+1;
                        c=c+1;
                        continue;
                        
                    }
                    if(c==1)
                    {   
                        b=j;
                        mid1.at<uchar>(i,(a+b)/2)=255;
                        double l,d=50,x,y,z,angle=1.11;
                        l=(mid1.cols-(a+b))/2;
                        z=(20586*d-40*l)/(25*l+20586*(1.0/tan(angle)));   // angle not assigned
                        x=(l*(25*z-40))/(20586);
                        y=((mid1.rows/2-j)*(25*z-40)/20586);
                        afile << x << " " << y << " " << 50-z << " " << "\n";

                        break;
                    }
                }
               
            }
        }
}
