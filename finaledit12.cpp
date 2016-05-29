

/*In this code intially background is taken in and made into 4 slices and 
on each slice otsu thresholding is applied for grayscale and 
hue channel and after that each frame of video is taken in and above method of slices is followed again.
Then we apply background subtraction and remove noise in image.
Finally we obtain only the mid points on laser line which are on foreground.Also here camera parameters are varied 
using trackbars creaeted to get optimum result*/ 


#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

float Brightness;
float Contrast ;
float Saturation;
float Gain;
int B;
int C;
int S;
int G;

VideoCapture cap(0); 
void cameratest(int,void*)            //function that when called resets camera parameters with given values
{
    Brightness =float(B)/100;
    Contrast   =float(C)/100;
    Saturation =float(S)/100;
    Gain       =float(G)/100;

    cap.set(CV_CAP_PROP_BRIGHTNESS,Brightness);
    cap.set(CV_CAP_PROP_CONTRAST, Contrast);
    cap.set(CV_CAP_PROP_SATURATION, Saturation);
    cap.set(CV_CAP_PROP_GAIN, Gain);
    

}

        
int main(int argc,const char** argv)
{
    int count=0;
    
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Brightness = cap.get(CV_CAP_PROP_BRIGHTNESS);        //recording camera parameters
    Contrast   = cap.get(CV_CAP_PROP_CONTRAST );
    Saturation = cap.get(CV_CAP_PROP_SATURATION);
    Gain       = cap.get(CV_CAP_PROP_GAIN);
    
    B=int(Brightness*100);
    C=int(Contrast*100);
    S=int(Saturation*100);
    G=int(Gain*100);
     

    Mat back,back2,backhsv;             
    cap >> back;                                        //getting background
    Mat backfinal1(back.rows,back.cols,CV_8UC1),backfinal2(back.rows,back.cols,CV_8UC1);
    cvtColor(back,back2,COLOR_BGR2GRAY);                
    cvtColor(back,backhsv,COLOR_BGR2HSV);          
    
    while(count!=back.rows)                             //slicing background and applying otsu for both hue channel and gray image
    {
        Mat backslice=back2(Range(count,count+back.rows/4),Range(0,back.cols));
        vector<Mat> channels(3);
        split(backhsv(Range(count,count+back.rows/4),Range(0,back.cols)),channels);
        Mat backhue=channels[0];
        GaussianBlur(backslice,backslice,Size(5,5),0,0);
        GaussianBlur(backhue,backhue,Size(5,5),0,0);
        threshold(backhue,backhue, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        threshold(backslice,backslice, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        backfinal1(Range(count,count+back.rows/4),Range(0,back.cols)) = backslice;
        backfinal2(Range(count,count+back.rows/4),Range(0,back.cols)) = backhue;

        count=count+back.rows/4;
    }

    namedWindow("foreground1", CV_WINDOW_AUTOSIZE); 
    namedWindow("midgray", CV_WINDOW_AUTOSIZE); 
    namedWindow("midhsv", CV_WINDOW_AUTOSIZE);
    namedWindow("foreground2", CV_WINDOW_AUTOSIZE);

    ofstream afile;
    string filename="pen1.txt";
    afile.open(filename, ios::app);
     
    while(1)
    {
        Mat hsv,gray,frame,foreground1,foreground2;
        Mat img1(frame.rows,frame.cols,CV_8UC1),img2(frame.rows,frame.cols,CV_8UC1);
        cap >> frame;               // get a new frame from camera
        
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        img1=gray;
        img2=gray;
        cvtColor(frame,hsv,COLOR_BGR2HSV);
        
        

        int i=0,j;
        while(i!=frame.rows)
        {
            
            //afile<<"reached";
            Mat img3=gray(Range(i,i+frame.rows/4),Range(0,frame.cols));
            Mat img4;
            vector<Mat> channels(3);
            split(hsv(Range(i,i+frame.rows/4),Range(0,frame.cols)),channels);
            img4=channels[0];
            GaussianBlur(img3,img3,Size(5,5),0,0);
            GaussianBlur(img4,img4,Size(5,5),0,0);
            threshold(img3,img3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            threshold(img4,img4, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            img1(Range(i,i+frame.rows/4),Range(0,frame.cols)) = img3; 
            img2(Range(i,i+frame.rows/4),Range(0,frame.cols))=img4; 
            i=i+frame.rows/4;
        }
        foreground1=img1-backfinal1;                                      //Background subtraction
        foreground2=img2-backfinal2;
        
        int c,a,b,f=0;
        if(waitKey(30) == 97)    
        {
            f++;
            
        } 
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
                        if(f==1)
                        {   


                            double l,d=50,x,y,z,angle=1.11;
                            l=(mid1.cols-(a+b))/2;
                            z=(20586*d-40*l)/(25*l+20586*(1.0/tan(angle)));   // angle not assigned
                            x=(l*(25*z-40))/(20586);
                            y=((mid1.rows/2-j)*(25*z-40)/20586);
                            afile << x << " " << y << " " << z << " " << "\n";
                        }
                        break;
                    }
                }
               
            }

        }


       

        Mat mid2(frame.rows,frame.cols,CV_8UC1,Scalar(0));                           // This function is to remove noice from the background subtracted image for gray image
        for(i=0;i<foreground2.rows;i++)
        {
            a=0;
            b=0;
            c=0;
            for(j=0;j<foreground2.cols;j++)
            {
                if(foreground2.at<uchar>(i,j)!=foreground2.at<uchar>(i,j+1))
                {
                    if(((foreground2.at<uchar>(i,j+1)+foreground2.at<uchar>(i,j+2)+foreground2.at<uchar>(i,j+3))<255*3) && ((foreground2.at<uchar>(i,j-1)+foreground2.at<uchar>(i,j-2)+foreground2.at<uchar>(i,j-3))<255*3))
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
                        mid2.at<uchar>(i,(a+b)/2)=255;
                        break;
                    }
                }
            }

        }
        
        imshow("foreground1", foreground1);
        imshow("foreground2", foreground2);
        imshow("midgray", mid1);
        imshow("midhsv", mid2);

        createTrackbar( "Brightness","foreground1", &B, 100, cameratest);                //setting trackbar to foreground1 window
        createTrackbar( "Contrast","foreground1", &C, 100,cameratest);
        createTrackbar( "Saturation","foreground1", &S, 100,cameratest);
        createTrackbar( "Gain","foreground1", &G, 100,cameratest);

        if(waitKey(30) == 27) 
            break;
    }
    afile.close();

    destroyWindow("foreground1");
    destroyWindow("foreground2");
    destroyWindow("midgray");
    destroyWindow("midhsv");
    return 0;
}
   