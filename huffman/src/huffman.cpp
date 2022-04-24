//
// Created by KSkun on 2022/4/22.
//

#include <QFile>

#include "huffman.h"

void HuffmanEncoder::iterateTree(quint16 nodeIndex, QBitArray *code) {
    auto &node = tree[nodeIndex];
    if (node.isLeaf()) {
        auto value = node.value;
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
        iterateTree(node.next[0], nextCode);
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
        iterateTree(node.next[1], nextCode);
    }
    delete code;
}

HuffmanEncoder::HuffmanEncoder(const QImage *image) {
    auto counts = new quint64[512]{};
    auto values = new quint8[256]{};
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            auto color = image->pixelColor(i, j);
            counts[qGray(color.rgb())]++;
        }
    }

    std::priority_queue<std::pair<quint64, quint16>, std::vector<std::pair<quint64, quint16>>, std::greater<>> q;
    for (int i = 0; i < 256; i++) {
        if (counts[i] == 0) continue;
        auto index = tree.size();
        tree.push_back({(quint8) i, HFM_NODE_NULL, HFM_NODE_NULL});
        values[index] = i;
        q.push({counts[i], index});
    }
    delete[] counts;
    while (q.size() > 1) {
        auto t1 = q.top();
        q.pop();
        auto t2 = q.top();
        q.pop();
        auto index = tree.size();
        tree.push_back({HFM_VALUE_NULL, t1.second, t2.second});
        q.push({t1.first + t2.first, index});
    }
    treeRoot = q.top().second;
    iterateTree(treeRoot, nullptr);
    delete[] values;
}

struct EncodedImageHeader {
    quint64 magicNumber;
    quint64 width, height;
    quint64 length;
};

#define ENCODED_IMAGE_MAGIC 0x12345678abcdefff
#define ENCODED_IMAGE_HEADER_SIZE sizeof(EncodedImageHeader)

QByteArray *HuffmanEncoder::encode(QImage *image, quint64 &dataLen) const {
    if (treeRoot == HFM_NODE_NULL) {
        return nullptr;
    }
    QVector<char> buffer;
    quint64 bufferSize = 0;
    char bufferEle = 0;
    int bufferEleOff = 0;
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            auto color = image->pixelColor(i, j);
            auto gray = qGray(color.rgb());
            auto code = codes[gray];
            for (int k = 0; k < code->size(); k++) {
                bufferEle |= (code->at(k) << bufferEleOff);
                bufferEleOff++;
                if (bufferEleOff == 8) {
                    buffer.push_back(bufferEle);
                    bufferSize += 8;
                    bufferEle = 0;
                    bufferEleOff = 0;
                }
            }
        }
    }
    if (bufferEleOff > 0) {
        buffer.push_back(bufferEle);
        bufferSize += bufferEleOff;
    }
    dataLen = bufferSize;
    auto header = EncodedImageHeader{
            ENCODED_IMAGE_MAGIC,
            (quint64) image->width(),
            (quint64) image->height(),
            bufferSize
    };
    auto bufferAllSize = ENCODED_IMAGE_HEADER_SIZE + buffer.size() * sizeof(quint8);
    auto bufferAll = new char[bufferAllSize];
    memcpy(bufferAll, &header, ENCODED_IMAGE_HEADER_SIZE);
    auto bufferData = bufferAll + ENCODED_IMAGE_HEADER_SIZE;
    for (int i = 0; i < buffer.size(); i++) {
        bufferData[i] = buffer[i];
    }
    auto byteArr = new QByteArray(bufferAll, bufferAllSize);
    delete[] bufferAll;
    return byteArr;
}

QImage *HuffmanEncoder::decode(const QByteArray *buffer, quint64 &dataLen) const {
    if (treeRoot == HFM_NODE_NULL) {
        return nullptr;
    }
    auto header = (EncodedImageHeader *) buffer->data();
    if (header->magicNumber != ENCODED_IMAGE_MAGIC) {
        return nullptr;
    }
    dataLen = header->length;

    auto data = (char *) header + ENCODED_IMAGE_HEADER_SIZE;
    auto treeIdx = treeRoot;
    auto x = 0, y = 0;
    auto image = new QImage(header->width, header->height, QImage::Format_Grayscale8);
    for (int i = 0; i < header->length; i++) {
        auto bit = ((data[i / 8] >> (i % 8)) & 1);
        treeIdx = tree[treeIdx].next[bit];
        if (tree[treeIdx].isLeaf()) {
            auto color = tree[treeIdx].value;
            image->setPixelColor(x, y, qRgb(color, color, color));
            treeIdx = treeRoot;
            y++;
            if (y == header->height) {
                x++;
                y = 0;
            }
        }
    }
    //assert(x == header->width && y == 0);
    return image;
}

struct CodeFileHeader {
    quint64 magicNumber;
    quint64 length;
    quint16 treeRoot;
};

#define CODE_FILE_MAGIC 0xabcdefff12345678
#define CODE_FILE_HEADER_SIZE sizeof(CodeFileHeader)

void HuffmanEncoder::toFile(const QString &filePath) const {
    if (treeRoot == HFM_NODE_NULL) {
        return;
    }
    auto header = CodeFileHeader{
            CODE_FILE_MAGIC,
            (quint64) tree.size(),
            treeRoot
    };
    QFile file(filePath);
    file.open(QFile::WriteOnly);
    file.write((char *) &header, CODE_FILE_HEADER_SIZE);
    for (auto &node: tree) {
        file.write((char *) &node, sizeof(TreeNode));
    }
    file.close();
}

HuffmanEncoder::HuffmanEncoder(const QString &filePath) {
    assert(QFile::exists(filePath));
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    CodeFileHeader header;
    file.read((char *) &header, CODE_FILE_HEADER_SIZE);
    if (header.magicNumber != CODE_FILE_MAGIC) {
        throw std::runtime_error("此文件不是编码表文件");
    }
    treeRoot = header.treeRoot;
    for (int i = 0; i < header.length; i++) {
        TreeNode node;
        file.read((char *) &node, sizeof(TreeNode));
        tree.push_back(node);
    }
    iterateTree(treeRoot, nullptr);
}
