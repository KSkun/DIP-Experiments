//
// Created by kskun on 2022/5/7.
//

#ifndef NODE_CANNY_IMAGE_H
#define NODE_CANNY_IMAGE_H

#include <mutex>
#include <unordered_map>

#include <opencv2/core.hpp>

namespace canny {
    extern cv::Mat images[6];

    bool loadImage(const std::string &path);

    std::string getImage(int id);

    void gaussianFilter(float sigma);

    void getGradient(int kernType);

    void nonMaxSuppression();

    void doubleThreshold(float thrLow, float thrHigh);

    void connectEdges();
}

#endif //NODE_CANNY_IMAGE_H
