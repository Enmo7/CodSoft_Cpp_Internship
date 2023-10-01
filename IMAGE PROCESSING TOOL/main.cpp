#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QMessageBox>

#include <opencv2/opencv.hpp>

using namespace cv;

class ImageProcessingTool : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessingTool(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
    }

private slots:
    void openImage()
    {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg)"));
        if (!filePath.isEmpty())
        {
            loadImage(filePath);
        }
    }

    void applyGrayscaleFilter()
    {
        if (!image.empty())
        {
            cvtColor(image, processedImage, COLOR_BGR2GRAY);
            displayProcessedImage();
        }
    }

    void applyBlurFilter()
    {
        if (!image.empty())
        {
            blur(image, processedImage, Size(5, 5));
            displayProcessedImage();
        }
    }

    void saveImage()
    {
        if (!processedImage.empty())
        {
            QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("PNG Image (*.png)"));
            if (!saveFilePath.isEmpty())
            {
                imwrite(saveFilePath.toStdString(), processedImage);
                QMessageBox::information(this, tr("Image Saved"), tr("The processed image has been saved."));
            }
        }
    }

private:
    void setupUI()
    {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        QLabel *imageLabel = new QLabel(this);
        QLabel *processedImageLabel = new QLabel(this);

        layout->addWidget(imageLabel);
        layout->addWidget(processedImageLabel);

        QPushButton *openButton = new QPushButton(tr("Open Image"), this);
        QPushButton *grayscaleButton = new QPushButton(tr("Grayscale"), this);
        QPushButton *blurButton = new QPushButton(tr("Blur"), this);
        QPushButton *saveButton = new QPushButton(tr("Save Image"), this);

        QSlider *brightnessSlider = new QSlider(Qt::Horizontal, this);
        QSlider *contrastSlider = new QSlider(Qt::Horizontal, this);

        // Connect button and slider signals to appropriate slots

        connect(openButton, &QPushButton::clicked, this, &ImageProcessingTool::openImage);
        connect(grayscaleButton, &QPushButton::clicked, this, &ImageProcessingTool::applyGrayscaleFilter);
        connect(blurButton, &QPushButton::clicked, this, &ImageProcessingTool::applyBlurFilter);
        connect(saveButton, &QPushButton::clicked, this, &ImageProcessingTool::saveImage);

        layout->addWidget(openButton);
        layout->addWidget(grayscaleButton);
        layout->addWidget(blurButton);
        layout->addWidget(saveButton);
        layout->addWidget(brightnessSlider);
        layout->addWidget(contrastSlider);

        QMainWindow::setCentralWidget(centralWidget);
    }

    void loadImage(const QString &filePath)
    {
        image = imread(filePath.toStdString());
        if (!image.empty())
        {
            processedImage = image.clone();
            displayImage();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open the image."));
        }
    }

    void displayImage()
    {
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        QLabel *imageLabel = findChild<QLabel *>();
        if (imageLabel)
        {
            imageLabel->setPixmap(QPixmap::fromImage(qImage));
            imageLabel->adjustSize();
        }
    }

    void displayProcessedImage()
    {
        QImage qProcessedImage(processedImage.data, processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_Grayscale8);
        QLabel *processedImageLabel = findChild<QLabel *>();
        if (processedImageLabel)
        {
            processedImageLabel->setPixmap(QPixmap::fromImage(qProcessedImage));
            processedImageLabel->adjustSize();
        }
    }

    Mat image;
    Mat processedImage;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageProcessingTool tool;
    tool.show();

    return app.exec();
}

#include "main.moc" // Include the moc file for Qt's meta-object compiler
