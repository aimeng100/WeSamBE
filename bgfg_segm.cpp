
// minimalistic foreground-background segmentation sample, based off OpenCV's bgfg_segm sample

//If this code helps with your work/research, please consider citing

//Shengqin Jiang, Xiaobo Lu, WeSamBE: A Weight-Sample-Based Method for Background Subtraction**.
//IEEE Transactions on Circuits and Systems for Video Technology, DOI 10.1109/TCSVT.2017.2711659, 2017 


//This code is based on the [SuBSENSE](https://bitbucket.org/pierre_luc_st_charles/subsense)
//so thanks to the original authors/maintainers for releasing the code.
//



#include "BackgroundSubtractorSuBSENSE.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;


static void help() {
    printf("\nMinimalistic example of foreground-background segmentation in a video sequence using\n"
            "OpenCV's BackgroundSubtractor interface; will analyze frames from the default camera\n"
            "or from a specified file.\n\n"
            "Usage: \n"
            "  ./bgfg_segm [--camera]=<use camera, true/false>, [--file]=<path to file> \n\n");
}

const char* keys = {
    "{c  |camera   |true     | use camera or not}"
    "{f  |file     |tree.avi | movie file path  }"
};

int main(int argc, const char** argv) {
    help();
    cv::CommandLineParser parser(argc, argv, keys);
    const bool bUseDefaultCamera = parser.get<bool>("camera");
    const std::string sVideoFilePath = parser.get<std::string>("file");
    cv::VideoCapture oVideoInput;
    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;
    if(bUseDefaultCamera) {
        oVideoInput.open(0);
        oVideoInput >> oCurrInputFrame;
    }
    else {
        oVideoInput.open(sVideoFilePath);
        oVideoInput >> oCurrInputFrame;
        oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
    }
    parser.printParams();
    if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
        if(bUseDefaultCamera)
            printf("Could not open default camera.\n");
        else
            printf("Could not open video file at '%s'.\n",sVideoFilePath.c_str());
        return -1;
    }
    oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
    oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());
    cv::Mat oSequenceROI(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI to the algorithm (ex: for CDnet, use ROI.bmp)
    cv::namedWindow("input",cv::WINDOW_NORMAL);
    cv::namedWindow("segmentation mask",cv::WINDOW_NORMAL);
    cv::namedWindow("reconstructed background",cv::WINDOW_NORMAL);
    BackgroundSubtractorSuBSENSE oBGSAlg;
    oBGSAlg.initialize(oCurrInputFrame,oSequenceROI);
    for(int k=0;;++k) {
        oVideoInput >> oCurrInputFrame;
        if(oCurrInputFrame.empty())
            break;
        oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
        imshow("input",oCurrInputFrame);
        imshow("segmentation mask",oCurrSegmMask);
        imshow("reconstructed background",oCurrReconstrBGImg);
        if(cv::waitKey(1)==27)
            break;
	£ý
    return 0;
}

