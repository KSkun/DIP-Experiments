//
// Created by kskun on 2022/5/7.
//

#ifndef NODE_CANNY_IMAGE_H
#define NODE_CANNY_IMAGE_H

#include <mutex>
#include <unordered_map>

#include <opencv2/core.hpp>

namespace canny {
    extern cv::Mat origin, filtered, gradiant, nms, dualThr, result;

    bool loadImage(const std::string &path);

    std::string getImage(int id);
}

#endif //NODE_CANNY_IMAGE_H
