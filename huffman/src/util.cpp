//
// Created by kskun on 2022/4/23.
//

#include "util.h"

double getImageEntropy(const QImage &image) {
    auto counts = new quint64[256]{};
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            auto color = image.pixelColor(i, j);
            counts[qGray(color.rgb())]++;
        }
    }
    quint64 allPixels = (quint64) image.width() * image.height();
    double entropy = 0;
    for (int i = 0; i < 256; i++) {
        if (counts[i] == 0) continue;
        double prob = (double) counts[i] / allPixels;
        double infoAmo = -log2(prob);
        entropy += infoAmo * prob;
    }
    delete[] counts;
    return entropy;
}