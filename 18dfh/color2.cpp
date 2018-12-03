#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "uart.hpp"
#include <iostream>
#include <math.h>	
#include "color.hpp"												
using namespace cv;
using namespace std;
extern char send_Buf;
extern  int  fd;
int color2(void)
{
int hmin =10;
int hmin_Max = 360;
int hmax = 77;
int hmax_Max = 360;

int smin = 23;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 245;
int vmin_Max = 255;
int vmax = 255;
int vmax_Max = 255;
int thresh = 125;   //二值化阈值分割、
Mat scrImage;
  Mat HSVImage;
  Mat hsv_1Image;
  VideoCapture capture;
    capture.open(1);    
    if (!capture.isOpened())
      {
         cout << "No capture" << endl;
            return -1; 
      }   
     else
	cout<<"Original size:"<<capture.get(CV_CAP_PROP_FRAME_WIDTH)<<" "<<capture.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	capture.set(CV_CAP_PROP_FPS, 15);
	cout<<"Modified size:"<<capture.get(CV_CAP_PROP_FRAME_WIDTH)<<" "<<capture.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    {
  
       for(;;)
     { 
       capture >> scrImage;
       // resize(scrImage,scrImage,Size(scrImage.cols/2,scrImage.rows/2),0,0,INTER_AREA);
       GaussianBlur( scrImage,scrImage,Size(3,3), 0.1, 0, BORDER_DEFAULT );      
     /*******************************
     * 为了图像范围，可剪切图形。
     ******************************/
         // Mat cutimage;
        //  cutimage=scrImage(Range(107,445),Range(145,475));
          cvtColor(scrImage, HSVImage, COLOR_BGR2HSV);    //将图像转换为HSV图、
 
   
 // moveWindow("image_2",200,20);                      //显示hsv图像
/*************************************/
 //创建滑动条，调整H，S，V 三个参数，分离出最佳阈值
/*********************************/
/*
    namedWindow("dst", WINDOW_GUI_EXPANDED);
	//调节色相 H
	createTrackbar("hmin", "dst", &hmin, hmin_Max);
	createTrackbar("hmax", "dst", &hmax, hmax_Max);
	//调节饱和度 S
	createTrackbar("smin", "dst", &smin, smin_Max);
	createTrackbar("smax", "dst", &smax, smax_Max);
	//调节亮度 
	createTrackbar("vmin", "dst", &vmin, vmin_Max);
	createTrackbar("vmax", "dst", &vmax, vmax_Max);

	*/
    inRange(HSVImage,Scalar(hmin,smin,vmin),Scalar(hmax,smax,vmax),hsv_1Image);//hsv颜色空间转化，通道分离。
     namedWindow( "image_2",CV_WINDOW_NORMAL);       
    imshow( "image_2", hsv_1Image);      
    //namedWindow( "image_3", CV_WINDOW_NORMAL);       
    //imshow( "image_3",  hsv_1Image); 
   // Mat out_image;
   // blur(hsv_1Image,out_image,Size(3,3));
    /*
        Mat  src_2;    
        bitwise_not(HSVImage,src_2);   //图像去反，为了正确 矫正对象
        Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));  //自定义内核扫描 
        Mat dilate_image; 
        erode(src_2,dilate_image,element);//进行膨胀操作  
         Mat out; 
       bitwise_not(dilate_image,out);   //图像还原);
    */    
   
    vector<vector<Point> > contours;      

    vector<Vec4i> hierarchy;    
 
    findContours( hsv_1Image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) ); 
   
   for( int i = 0; i < contours.size(); i++ )     

    {   
        double ConArea = fabs(contourArea(contours[i], true));
      // cout << "面" << i << "轮廓的面积为：" << ConArea << endl;   ///计算面积
    } 

   vector <vector<Point> >::iterator iter = contours.begin();
     for (; iter != contours.end();)
     {
         double g_dConArea = fabs(contourArea(*iter));
     if (1000<g_dConArea||g_dConArea< 450)
     {
         iter = contours.erase(iter);
         
     }
     else
     {
         ++iter;}}	 
     //cout << "【筛选后总共轮廓个数为：" << (int)contours.size() << endl;
	 
	 

        
	/********************************************************
	 * 判断是否满足需要的的圆
	 * *****************************************************/
   
    Mat drawing = Mat::zeros( hsv_1Image.size(), CV_8UC3 );  
   for( int i = 0; i< contours.size(); i++ )      

    {    
      Scalar color = Scalar( 255, 0,0);
      if (contours[i].size()>5)
	  {
    RotatedRect rectElli = fitEllipse(contours[i]);
    float fR = MIN(rectElli.size.width ,rectElli.size.height); // 是否为圆，可以比较这两个值，若十分接近或相等，就是一个正圆
    cout << "fitEllipse 中心: " <<  rectElli.center.x << ", " <<rectElli.center.y << "  半径:"<<fR/2<< endl;  
    circle(drawing, Point(rectElli.center), fR/2, Scalar(0,0,255), 2);//圆周
   // circle(drawing, Point(rectElli.center), 5, Scalar(0,0,255), 3);//圆心
    if(rectElli.center.x>150)
         Uart_SendFrame_L(fd);
         else
         Uart_SendFrame_R(fd);
	  }
     } 
      
       
          namedWindow( "Contours",CV_WINDOW_NORMAL);      
          imshow( "Contours", drawing);       
        //moveWindow("image_3",400,20);     //调试用途
       if (waitKey(1)=='q')
            break;
}
}
 return 0;
}
