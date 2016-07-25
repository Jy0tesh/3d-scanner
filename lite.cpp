#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

float Brightness;
float Contrast ;
float Saturation;
int B;
int C;
int S;


VideoCapture cap(0); 
void cameratest(int,void*)            //function that when called resets camera parameters with given values
{
    Brightness =float(B)/100;
    Contrast   =float(C)/100;
    Saturation =float(S)/100;
    

    cap.set(CV_CAP_PROP_BRIGHTNESS,Brightness);
    cap.set(CV_CAP_PROP_CONTRAST, Contrast);
    cap.set(CV_CAP_PROP_SATURATION, Saturation);
    
}
int main(int argc,const char** argv)
{
  
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Brightness = cap.get(CV_CAP_PROP_BRIGHTNESS);        //recording camera parameters
    Contrast   = cap.get(CV_CAP_PROP_CONTRAST );
    Saturation = cap.get(CV_CAP_PROP_SATURATION);
    
    B=int(Brightness*100);
    C=int(Contrast*100);
    S=int(Saturation*100);
	namedWindow("thresh", CV_WINDOW_AUTOSIZE); 
	namedWindow("midline", CV_WINDOW_AUTOSIZE); 


    Mat back,back2;             
    cap >> back;                                        //getting background
    Mat backfinal1(back.rows,back.cols,CV_8UC1);
    cvtColor(back,back2,COLOR_BGR2GRAY);                
   	int count=0;
    while(count!=back.rows)                             //slicing background and applying otsu for both hue channel and gray image
    {
        Mat backslice=back2(Range(count,count+back.rows/4),Range(0,back.cols));
        GaussianBlur(backslice,backslice,Size(5,5),0,0);        
        threshold(backslice,backslice, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        backfinal1(Range(count,count+back.rows/4),Range(0,back.cols)) = backslice;
        count=count+back.rows/4;
    }
     
	createTrackbar( "Brightness","thresh", &B, 100, cameratest);                //setting trackbar to thresh window
    createTrackbar( "Contrast","thresh", &C, 100,cameratest);
    createTrackbar( "Saturation","thresh", &S, 100,cameratest);
    while(1)
    {
        
        Mat gray,frame,foreground1;
        cap >> frame;               // get a new frame from camera
        Mat img1(frame.rows,frame.cols,CV_8UC1);
        
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        img1=gray;

        int i=0,j;
        while(i!=frame.rows)
        {
            
            //afile<<"reached";
            Mat img3=gray(Range(i,i+frame.rows/4),Range(0,frame.cols));
            GaussianBlur(img3,img3,Size(5,5),0,0);            
            threshold(img3,img3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);            
            img1(Range(i,i+frame.rows/4),Range(0,frame.cols)) = img3; 
            i=i+frame.rows/4;
        }
        foreground1=img1-backfinal1;                                      //Background subtraction
        int a,b,c;
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
                        break;
                    }
                }
               
            }

        }
   		imshow("thresh", foreground1);
    	imshow("midline", mid1);
		if(waitKey(30) == 27) 
            break;
    }
}
