


/*In this code intially background is taken in and made into 4 slices and 
on each slice otsu thresholding is applied for grayscale and 
hue channel and after that each frame of video is taken in and above method of slices is followed again.
Then we apply background subtraction and remove noise in image.
Finally we obtain only the mid points on laser line which are on foreground*/ 



#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc,const char** argv)
{
	VideoCapture cap(1); 
	if(!cap.isOpened())  // check if we succeeded
        return -1;
    int z;
    Mat back,backgray,backhsv;				
    cap >> back;			//getting background
    Mat otsuback1(back.rows,back.cols,CV_8UC1),otsuback2(back.rows,back.cols,CV_8UC1);
    cvtColor(back,backgray,COLOR_BGR2GRAY);
    cvtColor(back,backhsv,COLOR_BGR2HSV);          
    while(z!=back.rows)
    {
        Mat back1=backgray(Range(z,z+back.rows/4),Range(0,back.cols));
        vector<Mat> channels(3);
        split(backhsv(Range(z,z+back.rows/4),Range(0,back.cols)),channels);
        Mat back2=channels[0];
        GaussianBlur(back1,back1,Size(5,5),0,0);
        GaussianBlur(back2,back2,Size(5,5),0,0);
        threshold(back1,back1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        threshold(back2,back2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        otsuback1(Range(z,z+back.rows/4),Range(0,back.cols)) = back1;       // storing the slices in otsuback1,otsuback2
        otsuback2(Range(z,z+back.rows/4),Range(0,back.cols))=back2; 
        z=z+back.rows/4;
    }  // applying otsu and binary thresholding
    waitKey(10000);
    
    while(1)
    {
    	Mat hsv,gray,frame;
    	Mat img1(frame.rows,frame.cols,CV_8UC1),img2(frame.rows,frame.cols,CV_8UC1);  // creating two images of single channel
    	img1=gray;
        img2=gray;
        cap >> frame; 				// get a new frame from camera
        cvtColor(frame,gray,COLOR_BGR2GRAY);
    	cvtColor(frame,hsv,COLOR_BGR2HSV);
    	int i=0,j;
    	while(i!=frame.rows)           // diving image into four slices horizontally and applting otsu thresholding
    	{
    		
    		Mat img3=gray(Range(i,i+frame.rows/4),Range(0,frame.cols));
    		vector<Mat> channels(3);
    		split(hsv(Range(i,i+frame.rows/4),Range(0,frame.cols)),channels);
    		Mat img4=channels[0];           // taking in only hue channel
    		GaussianBlur(img3,img3,Size(5,5),0,0);
    		GaussianBlur(img4,img4,Size(5,5),0,0);
    		threshold(img3,img3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    		threshold(img4,img4, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    		img1(Range(i,i+frame.rows/4),Range(0,frame.cols)) = img3;       // storing the slices in img1,img2
    		img2(Range(i,i+frame.rows/4),Range(0,frame.cols))=img4; 
    		i=i+frame.rows/4;
    	}
    	Mat foreground1=img1-otsuback1;
        Mat foreground2=img2-otsuback2;                // background subtraction

    	int c,a,b;
        Mat mid1(frame.rows,frame.cols,CV_8UC1,Scalar(0)),mid2(frame.rows,frame.cols,CV_8UC1,Scalar(0));
        for(i=0;i<foreground1.rows;i++)              // this loop is to remove noise in the image(otsugray) and pass only forground part into txt files
        {
            a=0;
            b=0;
            c=0;
            for(j=0;j<foreground1.cols;j++)
            {
                if(foreground1.at<uchar>(i,j)!=foreground1.at<uchar>(i,j+1))
                {
                	if(((foreground1.at<uchar>(i,j+1)+foreground1.at<uchar>(i,j+2)+foreground1.at<uchar>(i,j+3))<255*3) && ((foreground1.at<uchar>(i,j-1)+foreground1.at<uchar>(i,j-2)+foreground1.at<uchar>(i,j-3))<255*3))
                		continue;                          // it will check it's previous three pixels and next three pixels to find whether it is noise or forground
                    
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
        for(i=0;i<foreground2.rows;i++)              // this loop is to remove noise in the image(otsuhsv) and pass only forground part into txt files
        {
            a=0;
            b=0;
            c=0;
            for(j=0;j<foreground2.cols;j++)
            {
                if(foreground2.at<uchar>(i,j)!=foreground2.at<uchar>(i,j+1))
                {
                    if(((foreground2.at<uchar>(i,j+1)+foreground2.at<uchar>(i,j+2)+foreground2.at<uchar>(i,j+3))<255*3) && ((foreground2.at<uchar>(i,j-1)+foreground2.at<uchar>(i,j-2)+foreground2.at<uchar>(i,j-3))<255*3))
                        continue;                          // it will check it's previous three pixels and next three pixels to find whether it is noise or forground
                    
                    if(c==0)
                    {   
                        a=j+1;
                        c=c+1;
                        continue;
                        
                    }
                    if(c==1)
                    {   
                        b=j;
                        mid2.at<uchar>(i,(a+b)/2)=255;
                        break;
                    }
                }
            }
        }
        namedWindow("midgray", CV_WINDOW_AUTOSIZE); 
        namedWindow("midhsv", CV_WINDOW_AUTOSIZE); 
        namedWindow("slicegray", CV_WINDOW_AUTOSIZE); 
    	namedWindow("slicehsv", CV_WINDOW_AUTOSIZE); 
    	imshow("slicegray", img1);
    	imshow("midgray", mid1);
        imshow("midhsv", mid2);
    	imshow("slicehsv", img2);                      // for this image otsu applied on hue channel
        if(waitKey(30) >= 0) 
    		break;
	}
    destroyWindow("midhsv");
    destroyWindow("midgray");
    destroyWindow("slicegray");
    destroyWindow("slicehsv");
    return 0;
}
   