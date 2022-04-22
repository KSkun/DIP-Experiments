//
// Created by KSkun on 2022/4/22.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <QImage>

class HuffmanEncodedImage {
public:
    HuffmanEncodedImage(QImage *image) : image(image) {}

private:
    struct TreeNode {
        quint16 nextID[2];

        TreeNode() {
            nextID[0] = nextID[1] = -1;
        }
    };

    QImage *image;
    quint64 pixelCounts[256];
    QVector<TreeNode> tree;
    QBitArray

    void
};


#endif //HUFFMAN_HUFFMAN_H
