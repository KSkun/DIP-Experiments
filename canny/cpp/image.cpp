//
// Created by kskun on 2022/5/7.
//

#include "image.h"

#include <iostream>
#include <queue>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <base64.h>

namespace canny {
    cv::Mat images[6];
    cv::Mat gradX, gradY;

    bool loadImage(const std::string &path) {
        images[0] = cv::imread(path, cv::IMREAD_GRAYSCALE);
        return !images[0].empty();
    }

    std::string getImage(int id) {
        std::vector<uchar> buf;
        cv::imencode(".png", images[id], buf);
        auto encoded = "data:image/png;base64," + base64_encode(buf.data(), buf.size());
        return encoded;
    }

    void gaussianFilter(float sigma) {
        // odd number not less than sigma * 6
        int kernSize = ceil(sigma * 6);
        if (kernSize % 2 == 0) kernSize++;
        images[1] = cv::Mat::zeros(images[0].size(), images[0].type());
        cv::GaussianBlur(images[0], images[1], {kernSize, kernSize}, sigma, sigma);
    }

    void getGradient(int kernType) {
        cv::Mat kernX, kernY;
        assert(kernType >= 1 && kernType <= 3);
        if (kernType == 1) {
            // robert
            kernX = (cv::Mat_<float>(2, 2) << -1, 0, 0, 1);
            kernY = (cv::Mat_<float>(2, 2) << 0, -1, 1, 0);
        } else if (kernType == 2) {
            // prewitt
            kernX = (cv::Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
            kernY = (cv::Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        } else if (kernType == 3) {
            // sobel
            kernX = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
            kernY = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        }
        cv::Mat normalized = cv::Mat::zeros(images[1].size(), CV_32F);
        normalized.forEach<float>([](float &color, const int position[]) {
            auto oriColor = images[1].at<uchar>(position);
            color = oriColor / 255.0f;
        });
        cv::filter2D(normalized, gradX, -1, kernX);
        cv::filter2D(normalized, gradY, -1, kernY);
        // get gradient amplitude
        auto size = images[1].size();
        images[2] = cv::Mat::zeros(size, CV_8U);
        images[2].forEach<uchar>([](uchar &color, const int position[]) {
            auto gx = gradX.at<float>(position), gy = gradY.at<float>(position);
            color = (uchar) (sqrt(gx * gx + gy * gy) * 255.0f);
        });
    }

    static int offsets[4][2][2] = {
            {{0, 1}, {0, -1}},
            {{1, 1}, {-1, -1}},
            {{1, 0}, {-1, 0}},
            {{1, -1}, {-1, 1}}
    };

    void nonMaxSuppression() {
        auto size = images[2].size();
        images[3] = cv::Mat::zeros(size, CV_8U);
        images[3].forEach<uchar>([&size](uchar &color, const int position[]) {
            auto K = images[2].at<uchar>(position);
            auto gx = gradX.at<float>(position), gy = gradY.at<float>(position);
            auto alpha = atan(gy / (gx + 1e-6));
            // map to 4 directions
            int direction;
            if (alpha >= 3 * M_PI / 8 || alpha < -3 * M_PI / 8) direction = 0;
            else if (alpha >= -M_PI / 8 && alpha < M_PI / 8) direction = 2;
            else if (alpha > 0) direction = 1;
            else direction = 3;

            // suppress when one neighbor larger
            bool suppressed = false;
            for (int i = 0; i < 2; i++) {
                auto nx = position[0] + offsets[direction][i][0],
                        ny = position[1] + offsets[direction][i][1];
                if (nx < 0 || nx >= size.height || ny < 0 || ny >= size.width) continue;
                auto g1 = images[2].at<uchar>(nx, ny);
                if (K < g1) suppressed = true;
            }
            if (!suppressed) color = K;
        });
    }

    void doubleThreshold(float thrLow, float thrHigh) {
        thrLow *= 255;
        thrHigh *= 255;
        auto size = images[3].size();
        images[4] = cv::Mat::zeros(size, CV_8U);
        images[4].forEach<uchar>([thrLow, thrHigh](uchar &color, const int position[]) {
            auto oriColor = images[3].at<uchar>(position);
            if (oriColor >= thrLow) color = 100;
            if (oriColor >= thrHigh) color = 255;
        });
    }

    void connectEdges() {
        auto size = images[3].size();
        images[5] = cv::Mat::zeros(size, CV_8U);
        std::queue<std::pair<int, int>> toVisit;
        bool *visited = new bool[size.height * size.width];
        std::fill(visited, visited + size.height * size.width, false);
        for (int i = 0; i < size.height; i++) {
            for (int j = 0; j < size.width; j++) {
                auto color = images[4].at<uchar>(i, j);
                if (color == 255) {
                    visited[i * size.width + j] = true;
                    toVisit.push({i, j});
                }
            }
        }
        // check all pixels' 8-connected pixels in gNH
        while (!toVisit.empty()) {
            auto p = toVisit.front();
            toVisit.pop();
            images[5].at<uchar>(p.first, p.second) = 255;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 2; j++) {
                    auto nx = p.first + offsets[i][j][0], ny = p.second + offsets[i][j][1];
                    if (nx < 0 || nx >= size.height || ny < 0 || ny >= size.width) continue;
                    auto color = images[4].at<uchar>(nx, ny);
                    if (color > 0 && !visited[nx * size.width + ny]) {
                        visited[nx * size.width + ny] = true;
                        toVisit.push({nx, ny});
                    }
                }
            }
        }
    }
}
