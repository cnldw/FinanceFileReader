#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include<QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFile>
#include <QUrl>
#include <QLabel>
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
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_fileOpen_triggered();

    void on_aboutOpen_triggered();

    void on_actionAboutQt_triggered();

    void on_pushButtonOpenFile_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *statusLabelOne;
    QLabel *statusLabelTwo;
    QLabel *statusLabelThree;
    QString filePath;

    void display_fileName(QString filePath);
    void open_file_Dialog();
};

#endif // MAINWINDOW_H
