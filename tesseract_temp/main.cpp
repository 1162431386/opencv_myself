#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "tesseract/baseapi.h"
#include "allheaders.h"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image;
    image=imread("/home/ww/Desktop/1.jpeg");  
	imshow("Sample", image);

    tesseract::TessBaseAPI tessearct_api;
    const char  *languagePath = "/home/ww/tesseract/tessdata";  //训练数据路径 
    const char *languageType = "eng";                       //语言类型
    // 可设置白名单
    //tessearct_api.SetVariable("tessedit_char_whitelist", "0123456789");
    //设置识别模式
    tessearct_api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    
     tessearct_api.SetImage((uchar*)image.data, image.cols, 
		image.rows, image.step[1], image.step[0]);

    int nRet = tessearct_api.Init(languagePath, languageType,tesseract::OEM_DEFAULT);
    if (nRet != 0) {
        printf("初始化字库失败！");
        return -1;
    }
       /*暂时不懂系列 */
    //tessearct_api.ProcessPages（image，retry_config，timeout_ms，renderer）;
    //tessearct_api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    //tessearct_api.SetImage(seg_image.data, seg_image.cols, seg_image.rows, 1, seg_image.cols);  //输入图像
    
    string out = string(tessearct_api.GetUTF8Text());
    cout<<"the out result:"<<out<<endl;
    waitKey(0);
    return 0;
}


