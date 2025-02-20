#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImageReader>
#include <QDragEnterEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setLayout(ui->verticalLayout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    QString filePath = urls.first().toLocalFile();
    SetImageToLabel(filePath);
}

void MainWindow::SetImageToLabel(const QString& filePath)
{
    QImageReader reader(filePath);
    if (reader.canRead())
    {
        m_origImage = new QImage(reader.read().convertToFormat(QImage::Format_RGB32));

        int index = ui->comboBox->currentIndex();
        if (ui->comboBox->currentIndex() == 0)
        {
            ui->label->setPixmap(QPixmap::fromImage(*m_origImage));
        }
        else
        {
            QImage LSBImage = ConvertToLSBImage(*m_origImage, index - 1);
            ui->label->setPixmap(QPixmap::fromImage(LSBImage));
        }

        ui->label->setAlignment(Qt::AlignCenter);

        ui->saveButton->setEnabled(true);
    }
}

QImage MainWindow::ConvertToLSBImage(const QImage& image, int index)
{
    auto LSBImage = image;

    for(long long i=0;i<image.height();i++)
        for(long long j=0;j<image.width();j++)
        {
            QColor color = image.pixelColor(j,i);


            color.setRed(color.red() & 1 << index ? 255 : 0);
            color.setBlue(color.blue() & 1 << index ? 255 : 0);
            color.setGreen(color.green() & 1 << index ? 255 : 0);

            LSBImage.setPixelColor(j,i,color);
        }

    return LSBImage;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(m_origImage == nullptr)
        return;

    if (index == 0)
    {
        ui->label->setPixmap(QPixmap::fromImage(*m_origImage));
    }
    else
    {
        QImage LSBImage = ConvertToLSBImage(*m_origImage, index - 1);
        ui->label->setPixmap(QPixmap::fromImage(LSBImage));
    }
}

void MainWindow::on_selectButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите изображение", "", "Изображения (*.png *.jpg *.bmp *.gif *.pgm)");

    if(filePath == "")
        return;

    SetImageToLabel(filePath);
}

void MainWindow::on_removeButton_clicked()
{
    ui->label->clear();
    ui->saveButton->setEnabled(false);
}

void MainWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Сохранить как", "image", "Изображения (*.png *.jpg *.bmp *.gif *.pgm)");
    ui->label->pixmap()->toImage().save(filePath);
}
