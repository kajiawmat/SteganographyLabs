#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_selectButton_clicked();

    void on_saveButton_clicked();

    void on_removeButton_clicked();

private:
    void    SetImageToLabel(const QString& fileName);
    QImage  ConvertToLSBImage(const QImage &image, int index);
    Ui::MainWindow *ui;

    QImage*  m_origImage = nullptr;
};

#endif // MAINWINDOW_H
