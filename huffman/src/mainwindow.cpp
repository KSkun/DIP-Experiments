#include <QFileDialog>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    delete image;
    delete encoder;
}

void MainWindow::on_buttonQuit_clicked() {
    exit(0);
}

void MainWindow::on_buttonFileImage_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            "打开要编码的文件",
            QDir::currentPath(),
            "图片 (*.png *.jpg *.jpeg *.bmp)"
    ));
    if (file.isEmpty()) return;

    fileImage = file;
    fileEncoded = fileCode = "";

    auto newImage = new QImage(fileImage);
    if (newImage->isNull()) {
        ui->statusBar->showMessage("打开文件失败");
        return;
    }
    if (!newImage->isGrayscale()) {
        ui->statusBar->showMessage("图片不是灰阶图");
        return;
    }
    delete image;
    image = newImage;
    imageEntropy = getImageEntropy(*image);
    imageBinLen = 8;
    imageLenAvg = IMAGE_INFO_UNAVAILABLE;

    refreshFileView();
    refreshImageView();
    refreshInfoView();
}

void MainWindow::on_buttonFileEncoded_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            "打开要解码的文件",
            QDir::currentPath(),
            "已编码文件 (*.bin)"
    ));
    if (file.isEmpty()) return;

    fileEncoded = file;
    fileImage = "";

    delete image;
    image = nullptr;
    imageEntropy = IMAGE_INFO_UNAVAILABLE;
    imageBinLen = IMAGE_INFO_UNAVAILABLE;

    refreshFileView();
    refreshImageView();
    refreshInfoView();
}

void MainWindow::on_buttonFileCode_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            "打开解码使用的编码表",
            QDir::currentPath(),
            "编码表 (*.code.bin)"
    ));
    if (file.isEmpty()) return;

    fileCode = file;
    fileImage = "";

    imageEntropy = IMAGE_INFO_UNAVAILABLE;
    imageBinLen = IMAGE_INFO_UNAVAILABLE;

    refreshFileView();
}

void MainWindow::refreshImageView() {
    ui->statusBar->showMessage("");
    if (image) {
        ui->labelImageView->setPixmap(
                QPixmap::fromImage(*image).scaled(
                        ui->labelImageView->size(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                ));
    }
}

void MainWindow::refreshFileView() {
    QFontMetrics fontMatrics(ui->labelFileImage->font());
    auto labelWidth = ui->labelFileImage->size().width();

    ui->labelFileImage->setText(fontMatrics.elidedText(fileImage, Qt::ElideMiddle, labelWidth));
    ui->labelFileEncoded->setText(fontMatrics.elidedText(fileEncoded, Qt::ElideMiddle, labelWidth));
    ui->labelFileCode->setText(fontMatrics.elidedText(fileCode, Qt::ElideMiddle, labelWidth));
}

void MainWindow::refreshInfoView() {
    if (image) {
        ui->labelSize->setText(QString::asprintf("%d x %d 像素", image->width(), image->height()));
    } else {
        ui->labelSize->setText("");
    }
    if (imageEntropy != IMAGE_INFO_UNAVAILABLE) {
        ui->labelEntropy->setText(QString::asprintf("%f bit/像素", imageEntropy));
    } else {
        ui->labelEntropy->setText("");
    }
    if (imageBinLen != IMAGE_INFO_UNAVAILABLE) {
        ui->labelLength->setText(QString::asprintf("%d bit/像素", imageBinLen));
    } else {
        ui->labelLength->setText("");
    }
    if (imageLenAvg != IMAGE_INFO_UNAVAILABLE) {
        ui->labelLengthAvg->setText(QString::asprintf("%f bit/像素", imageLenAvg));
    } else {
        ui->labelLengthAvg->setText("");
    }
}

void MainWindow::on_buttonEncode_clicked() {
    ui->statusBar->showMessage("");
    delete encoder;
    encoder = new HuffmanEncoder(image);
    quint64 encodedLen;
    auto data = encoder->encode(image, encodedLen);
    imageLenAvg = (double) encodedLen / (image->width() * image->height());

    fileEncoded = fileImage + ".bin";
    int suffixCount = 0;
    while (QFile::exists(fileEncoded)) {
        fileEncoded = fileImage + QString::asprintf("_%d.bin", ++suffixCount);
    }
    QFile fFileEncoded(fileEncoded);
    fFileEncoded.open(QIODevice::WriteOnly);
    fFileEncoded.write(*data);
    fFileEncoded.close();
    delete data;

    if (suffixCount == 0) fileCode = fileImage + ".code.bin";
    else fileCode = fileImage + QString::asprintf("_%d.code.bin", suffixCount);
    encoder->toFile(fileCode);

    refreshInfoView();
    refreshFileView();
}

void MainWindow::on_buttonDecode_clicked() {
    ui->statusBar->showMessage("");
    delete encoder;
    delete image;
    encoder = nullptr;
    try {
        encoder = new HuffmanEncoder(fileCode);
    } catch (std::runtime_error &e) {
        ui->statusBar->showMessage(e.what());
        return;
    }

    QFile fFileEncoded(fileEncoded);
    fFileEncoded.open(QFile::ReadOnly);
    auto data = fFileEncoded.readAll();
    fFileEncoded.close();
    quint64 encodedLen;
    image = encoder->decode(&data, encodedLen);
    if (image == nullptr) {
        ui->statusBar->showMessage("图片解码失败");
        return;
    }
    imageEntropy = getImageEntropy(*image);
    imageBinLen = 8;
    imageLenAvg = (double) encodedLen / (image->width() * image->height());

    refreshImageView();
    refreshInfoView();
}
