//
// Created by kskun on 2022/5/7.
//

#include "image.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace canny {
    cv::Mat origin, filtered, gradiant, nms, dualThr, result;

    bool loadImage(const std::string &path) {
        auto originT = cv::imread(path, cv::IMREAD_GRAYSCALE);
        if (originT.empty()) return false;
        origin = cv::Mat::zeros(originT.size(), originT.type());
        cv::normalize(originT, origin, 0, 255, cv::NORM_MINMAX);
        return true;
    }

    std::string getImage(int id) {
        // TODO
    }
}
