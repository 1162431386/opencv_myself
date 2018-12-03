#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "color_local.hpp"
#include <iostream>																	
using namespace cv;
using namespace std;
int local;
int color_local(void)
{
int hmin = 0;
int hmin_Max = 360;
int hmax = 360;
int hmax_Max = 360;
//饱和度
int smin = 0;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 106;
int vmin_Max = 255;
int vmax = 250;
int vmax_Max = 255;
int thresh = 125;   //二值化阈值分割、
  Mat scrImage;
  Mat HSVImage;
  Mat hsv_1Image;
  VideoCapture capture;
    capture.open(0);    
    if (!capture.isOpened())
      {
         cout << "No capture" << endl;
            return -1; 
      }   
     else
    {
  
       for(;;)
     { 
       capture >> scrImage;
       GaussianBlur( scrImage, scrImage, Size(3,3), 0.1, 0, BORDER_DEFAULT );      
     /*******************************
     * 为了图像范围，可剪切图形。
     ******************************/
          Mat cutimage;
          cutimage=scrImage(Range(107,445),Range(145,475));
  cvtColor(cutimage, HSVImage, COLOR_BGR2HSV);    //将图像转换为HSV图、
  namedWindow( "image_1", CV_WINDOW_NORMAL );       
  imshow( "image_1", cutimage );      
  //moveWindow("image_1",20,20);
  
  namedWindow( "image_2",CV_WINDOW_NORMAL);       
  imshow( "image_2", HSVImage);      
 // moveWindow("image_2",200,20);                      //显示hsv图像
/*************************************/
 //创建滑动条，调整H，S，V 三个参数，分离出最佳阈值
/*********************************/
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
    inRange(HSVImage,Scalar(hmin,smin,vmin),Scalar(hmax,smax,vmax),hsv_1Image);//hsv颜色空间转化，通道分离。
    namedWindow( "image_3", CV_WINDOW_NORMAL);       
    imshow( "image_3",  hsv_1Image); 
    Mat out_image;
    blur(hsv_1Image,out_image,Size(3,3));
    /*
        Mat  src_2;    
        bitwise_not(HSVImage,src_2);   //图像去反，为了正确 矫正对象
        Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));  //自定义内核扫描 
        Mat dilate_image; 
        erode(src_2,dilate_image,element);//进行膨胀操作  
         Mat out; 
       bitwise_not(dilate_image,out);   //图像还原);
    */
    Mat canny_output;     
    vector<vector<Point> > contours;      

    vector<Vec4i> hierarchy;    
    Canny(out_image, canny_output, thresh, thresh*3, 3 );     

    //²éÕÒÂÖÀª    

    findContours( canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );     

    //第四个参数用于检测轮廓类型 ，可以自己搜索资料，这里，只检测外围轮廓 
    //第五个参数是轮廓相对于原始坐标的偏移量；很重要。   
    for( int i = 0; i < contours.size(); i++ )     

    {   
        double ConArea = contourArea(contours[i], true);
       // cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：" << ConArea << endl;   ///计算面积
    } 

    /*****************************************
     * 用于面积的筛选，可以在一定取值内剔除不相关的值.
     * ************************************/   
     
     vector <vector<Point>>::iterator iter = contours.begin();
     for (; iter != contours.end();)
     {
         double g_dConArea = contourArea(*iter);
     if (g_dConArea < 195)
     {
         iter = contours.erase(iter);
         
     }
     else
     {
         ++iter;
         }}
     //cout << "【筛选后总共轮廓个数为：" << (int)contours.size() << endl;


    vector<Moments> mu(contours.size() );       

    for( int i = 0; i < contours.size(); i++ )     

    {   

        mu[i] = moments( contours[i], false );   

    }     

    //ŒÆËãÂÖÀªµÄÖÊÐÄ     

    vector<Point2f> mc( contours.size() );      

    for( int i = 0; i < contours.size(); i++ )     

    {   

        mc[i] = Point2d( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );   

    }     

    //计算中心距     

       Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );   
       Point2f center;  //定义圆中心坐标
       float radius;  //定义圆半径      

    for( int i = 0; i< contours.size(); i++ )      

    {         

        Scalar color = Scalar( 255, 0,0);  //颜色通道赋值     
        minEnclosingCircle(Mat(contours[i]), center, radius);
        drawContours( drawing, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point() ); //绘制轮廓        

        circle( drawing, center, radius,  Scalar( 255, 255, 255), -1, 8, 0 );             //绘制近似圆轮廓  

        circle( drawing, center, 4,  Scalar( 255, 0, 0), -1, 8, 0 );          //绘制圆心点
       /***********************************/
     //  double Area =contourArea(contours);
        /**********************************/
    //  rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0,255,0));        //这个函数是模拟矩形框，这里做近似圆处理，注释。      

        char tam[100]; //中心坐标显示的字符串存储数
       
        sprintf(tam, "(%0.0f,%0.0f)",mc[i].x,mc[i].y);       
        putText(drawing, tam, Point(mc[i].x, mc[i].y),FONT_HERSHEY_SIMPLEX, 0.4, cvScalar(255,0,255),1);
        local = mc[i].x*1000 + mc[i].y;
      
    }    
    
          imshow( "Contours", drawing);         
        //moveWindow("image_3",400,20);     //调试用途
 if (waitKey(1)=='q')
            break;
}
    }
 return local;
}





