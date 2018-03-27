#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include<QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFile>
#include <QFileInfo>
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
    QLabel *statusLabelFour;
    QLabel *statusLabelFive;
    QString filePath;

    /**
     * @brief clear_statusBar
     */
    void clear_statusBar();
    /**
     * @brief display_rowsCount
     * @param rowsCount
     */
    void display_rowsCount(int rowsCount);
    /**
     * @brief display_fileName
     * @param filePath
     */
    void display_fileName(QString filePath);
    /**
     * @brief open_file_Dialog
     */
    void open_file_Dialog();

    void statusBar_disPlay(QString text);
};

#endif // MAINWINDOW_H
