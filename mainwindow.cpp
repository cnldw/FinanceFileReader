#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    //第一个标签
    statusLabelOne = new QLabel;
    statusLabelOne->setMinimumSize(100, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelOne);
    statusLabelOne->setText(tr("解析耗时:"));

    //第二个标签
    statusLabelTwo = new QLabel;
    statusLabelTwo->setMinimumSize(150, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelTwo);
    //设置标签内容
    statusLabelTwo->setText(tr("总记录数:"));

    //第三个标签
    statusLabelThree = new QLabel;
    statusLabelThree->setMinimumSize(230, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelThree);
    //设置标签内容
    statusLabelThree->setText(tr("文件:"));

    //第四个标签
    statusLabelFour = new QLabel;
    statusLabelFour->setMinimumSize(150, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelFour);
    //设置标签内容
    statusLabelFour->setText(tr("文件内0行0列"));
    statusLabelFour->setToolTip(tr("此处显示当前选择的字段在原文件中的行和列"));

    //第五个标签
    statusLabelFive = new QLabel;
    statusLabelFive->setMinimumSize(195, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelFive);
    //设置标签内容
    statusLabelFive->setText(tr(""));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete statusLabelOne;
    statusLabelOne=NULL;
    delete statusLabelTwo;
    statusLabelTwo=NULL;
    delete statusLabelThree;
    statusLabelThree=NULL;
    delete statusLabelFour;
    statusLabelFour=NULL;
    delete statusLabelFive;
    statusLabelFive=NULL;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()){
        return;
    }
    if(urls.count()>1){
        statusBar_disPlay("拖进来一个文件试试~,文件太多啦");
        return;
    }
    QString fileName = urls.first().toLocalFile();
    //判断是否是文件夹
    QFileInfo fileinfo(fileName);
    if(fileinfo.isDir()){
        statusBar_disPlay("拖进来一个文件试试~,不接受文件夹");
        return;
    }
    if (fileName.isEmpty())
    {        return;
    }
    else{
        filePath=fileName;
        ui->filePathLineText->setText(filePath);
        display_fileName(filePath);
        statusBar_disPlay(NULL);
    }
}

/**
  *清除状态栏的信息
 * @brief MainWindow::clear_statusBar
 */
void MainWindow::clear_statusBar(){
    statusLabelOne->setText(tr("解析耗时:"));
    statusLabelTwo->setText(tr("总记录数:"));
    statusLabelThree->setText(tr("文件:"));
    statusLabelFour->setText(tr("文件内0行0列"));
}

/**
  *打开文件窗口
 * @brief MainWindow::open_file_Dialog
 */
void MainWindow::open_file_Dialog(){
    QString   file = QFileDialog::getOpenFileName(this, tr("打开文件..."), ".", tr("所有文件(*.*)"));
    if(file.length() != 0) {
        filePath=file;
        ui->filePathLineText->setText(filePath);
        display_fileName(filePath);
    } else {
        //放弃了读取文件
    }
}
/**
  *显示加载的记录总数
 * @brief MainWindow::display_rowsCount
 * @param rowsCount
 */
void MainWindow::display_rowsCount(int rowsCount){

    statusLabelTwo->setText(tr("总记录数:")+QString::number(rowsCount, 10)+tr("行"));
}


/**
 * 用于加载文件后在状态栏显示加载的文件
 * @brief MainWindow::display_fileName
 * @param filePath
 */
void MainWindow::display_fileName(QString filePath){
    int first = filePath.lastIndexOf ("/");
    QString fileName = filePath.right (filePath.length ()-first-1);
    statusLabelThree->setText(tr("文件：")+fileName);
}

void MainWindow::statusBar_disPlay(QString text){
    statusLabelFive->setText(text);
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
