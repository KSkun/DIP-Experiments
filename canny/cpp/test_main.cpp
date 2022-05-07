//
// Created by kskun on 2022/5/7.
//

#include <iostream>
#include <opencv2/highgui.hpp>

#include "image.h"

int main() {
    if (!canny::loadImage("lena.jpg")) {
        std::cerr << "load failed" << std::endl;
        return -1;
    }
    cv::imshow("test", canny::images[0]);
    cv::waitKey();
    canny::gaussianFilter(2.0f);
    cv::imshow("test", canny::images[1]);
    cv::waitKey();
    canny::getGradient(3);
    cv::imshow("test", canny::images[2]);
    cv::waitKey();
    canny::nonMaxSuppression();
    cv::imshow("test", canny::images[3]);
    cv::waitKey();
    canny::doubleThreshold(0.04, 0.15);
    cv::imshow("test", canny::images[4]);
    cv::waitKey();
    canny::connectEdges();
    cv::imshow("test", canny::images[5]);
    cv::waitKey();
    return 0;
}