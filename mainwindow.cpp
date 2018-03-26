#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    //第一个标签
    statusLabelOne = new QLabel;
    statusLabelOne->setMinimumSize(150, 20); // 设置标签最小大小
    // statusLabelOne->setFrameShape(QFrame::WinPanel); // 设置标签形状
    //  statusLabelOne->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(statusLabelOne);
    statusLabelOne->setText(tr("解析耗时:"));

    //第二个标签
    statusLabelTwo = new QLabel;
    statusLabelTwo->setMinimumSize(200, 20); // 设置标签最小大小
    // statusLabelTwo->setFrameShape(QFrame::WinPanel); // 设置标签形状
    //  statusLabelTwo->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(statusLabelTwo);
    //设置标签内容
    statusLabelTwo->setText(tr("记录数："));

    //第三个标签
    statusLabelThree = new QLabel;
    statusLabelThree->setMinimumSize(250, 20); // 设置标签最小大小
    // statusLabelTwo->setFrameShape(QFrame::WinPanel); // 设置标签形状
    //  statusLabelTwo->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(statusLabelThree);
    //设置标签内容
    statusLabelThree->setText(tr("文件："));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;
    ui->filePathLineText->setText(fileName);
}

/**
  *打开文件窗口
 * @brief MainWindow::open_file_Dialog
 */
void MainWindow::open_file_Dialog(){
    filePath = QFileDialog::getOpenFileName(this, tr("打开文件..."), ".", tr("所有文件(*.*)"));
    if(filePath.length() != 0) {
        ui->filePathLineText->setText(filePath);
        display_fileName(filePath);
    } else {
        //放弃了读取文件
    }
}

void MainWindow::display_fileName(QString filePath){
    int first = filePath.lastIndexOf ("/");
    QString fileName = filePath.right (filePath.length ()-first-1);
    statusLabelThree->setText(tr("文件：")+fileName);
}
void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

void MainWindow::on_aboutOpen_triggered()
{
    QMessageBox::about(this,tr("关于本程序"),tr("本程序是一个可以解析格式化显示开放式基金数据交换协议的工具，用于解析如上所述协议所交换的各种文件\r\n作者:793554262@qq.com(刘德位)"));
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("关于QT"));
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    open_file_Dialog();
}
