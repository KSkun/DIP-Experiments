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
#define HFM_VALUE_NULL quint8(-1)

class HuffmanEncoder;

struct CodeFileHeader;

struct EncodedImageHeader;

class HuffmanEncoder {
public:
    explicit HuffmanEncoder(const QImage *image);

    explicit HuffmanEncoder(const QString &filePath);

    void toFile(const QString &filePath) const;

    QByteArray *encode(QImage *image, quint64 &dataLen) const;

    QImage *decode(const QByteArray *buffer, quint64 &dataLen) const;


private:
    struct TreeNode {
        quint8 value = HFM_VALUE_NULL;
        quint16 next[2]{HFM_NODE_NULL, HFM_NODE_NULL};

        bool isLeaf() const {
            return next[0] == HFM_NODE_NULL && next[1] == HFM_NODE_NULL;
        }
    };

    QVector<TreeNode> tree;
    quint16 treeRoot = HFM_NODE_NULL;
    QMap<quint8, QBitArray *> codes;

    HuffmanEncoder() = default;

    void iterateTree(quint16 nodeIndex, QBitArray *code);
};

#endif //HUFFMAN_HUFFMAN_H
