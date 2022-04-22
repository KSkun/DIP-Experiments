//
// Created by KSkun on 2022/4/22.
//

#include "huffman.h"

void HuffmanEncoder::iterateTree(quint16 nodeIndex, QBitArray *code, const quint8 values[]) {
    auto &node = tree[nodeIndex];
    if (node.isLeaf()) {
        auto value = values[nodeIndex];
        if (code) codes[value] = code;
        else codes[value] = new QBitArray(1);
        return;
    }
    if (node.next[0] != HFM_NODE_NULL) {
        QBitArray *nextCode;
        if (code) {
            nextCode = new QBitArray(code->size() + 1);
            *nextCode |= *code;
        } else {
            nextCode = new QBitArray(1);
        }
        nextCode->setBit(nextCode->size() - 1, false);
        iterateTree(node.next[0], nextCode, values);
    }
    if (node.next[1] != HFM_NODE_NULL) {
        QBitArray *nextCode;
        if (code) {
            nextCode = new QBitArray(code->size() + 1);
            *nextCode |= *code;
        } else {
            nextCode = new QBitArray(1);
        }
        nextCode->setBit(nextCode->size() - 1, true);
        iterateTree(node.next[1], nextCode, values);
    }
    delete code;
}

HuffmanEncoder *HuffmanEncoder::fromImage(QImage *image) {
    auto encoder = new HuffmanEncoder;
    auto counts = new quint64[512]{};
    auto values = new quint8[256]{};
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            auto color = image->pixelColor(i, j);
            counts[qGray(color.rgb())]++;
        }
    }

    std::priority_queue<std::pair<quint64, quint16>, std::vector<std::pair<quint64, quint16>>, std::less<>> q;
    for (int i = 0; i < 256; i++) {
        if (counts[i] == 0) continue;
        auto index = encoder->tree.size();
        encoder->tree.push_back(TreeNode());
        values[index] = i;
        q.push({counts[i], index});
    }
    delete[] counts;
    while (q.size() > 1) {
        auto t1 = q.top(); q.pop();
        auto t2 = q.top(); q.pop();
        auto index = encoder->tree.size();
        encoder->tree.push_back({t1.second, t2.second});
        q.push({t1.first + t2.first, index});
    }
    encoder->treeRoot = q.top().second;
    encoder->iterateTree(encoder->treeRoot, nullptr, values);
    delete[] values;
    // TODO test
    return encoder;
}

#define ENCODED_IMAGE_MAGIC 0x12345678abcdefgh
#define CODE_FILE_MAGIC 0xabcdefgh12345678

struct EncodedImageHeader {
    quint64 magicNumber;
    quint64 width, height;
    quint64 length;
    const quint8 *data;
};

struct CodeFileHeader {
    quint64 magicNumber;
    quint64 length;
    const HuffmanEncoder::TreeNode *data;
};
