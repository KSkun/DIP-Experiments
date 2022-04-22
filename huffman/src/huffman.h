//
// Created by KSkun on 2022/4/22.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <queue>
#include <QImage>
#include <QBitArray>
#include <QMap>

#define HFM_NODE_NULL quint16(-1)

struct CodeFileHeader;

class HuffmanEncoder {
public:
    HuffmanEncoder() = default;

    static HuffmanEncoder *fromImage(QImage *image);

    static HuffmanEncoder *fromByteArray(QByteArray *buffer) {
        auto encoder = new HuffmanEncoder;
        // TODO
        return encoder;
    }

    QByteArray *toByteArray() const {
        // TODO
        return nullptr;
    }

    QByteArray *encode(QImage *image) const {
        if (treeRoot == HFM_NODE_NULL) {
            return nullptr;
        }
        return nullptr; // TODO
    }

    QImage *decode(QByteArray *buffer) const {
        if (treeRoot == HFM_NODE_NULL) {
            return nullptr;
        }
        return nullptr; // TODO
    }



private:
    friend CodeFileHeader;

    struct TreeNode {
        quint16 next[2]{HFM_NODE_NULL, HFM_NODE_NULL};

        bool isLeaf() {
            return next[0] == HFM_NODE_NULL && next[1] == HFM_NODE_NULL;
        }
    };

    QVector<TreeNode> tree;
    quint16 treeRoot = HFM_NODE_NULL;
    QMap<quint8, QBitArray *> codes;

    void iterateTree(quint16 nodeIndex, QBitArray *code, const quint8 values[]);
};

#endif //HUFFMAN_HUFFMAN_H
