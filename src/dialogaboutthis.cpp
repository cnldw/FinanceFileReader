/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogaboutthis.h"
#include "ui_dialogaboutthis.h"
#include "src/publicdefine.h"
#include "src/utils.h"
#include <QSysInfo>

DialogAboutThis::DialogAboutThis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutThis)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    setWindowTitle("关于本程序");
    QString OSVersion=QSysInfo::productType();
    QString OSArch=QSysInfo::currentCpuArchitecture();
    if(OSVersion=="osx"){
        OSVersion="macOS";
    }
    QString QrCode="";
    QString OS32Bit="";
    if (Q_PROCESSOR_WORDSIZE == 4) {
        OS32Bit="32位程序";
    }
    if(!ENABLE_QRCODE){
        QrCode="S版-无二维码分享功能";
    }
    ui->label->setWordWrap(true);
    ui->label->setText("本程序是一个可以解析格式化显示基金/信托/银行理财子各种OFD(类OFD)协议文件、各种固定分隔符CSV文件、各种字段定长文件、各种证券DBF"
                       "文件的工具\r\n可以用于解析如上所述协议/接口规范所交换的各种文件\r\n\r\n作      者："+QByteArray::fromBase64(AUTHOR_NAME)+"\r\n友情赞助："
                       +QByteArray::fromBase64(AUTHOR_PHONE)+"(支付宝)\r\n本程序为本人业余开发,如果这个程序对你带来了方便,你可进行小额赞助,本程序将持续更新改进,带来更好的使用体验...\r\n\r\n操作系统："
                       +OSVersion+"\r\n系统架构："+OSArch+"\r\n程序版本："+VERSION_V+(QrCode.isEmpty()&&OS32Bit.isEmpty()?"":"\r\n其他信息：")+QrCode+(QrCode.isEmpty()||OS32Bit.isEmpty()?OS32Bit:("/"+OS32Bit))
                       +"\r\n编译日期："+Utils::getCompileDate());
}

DialogAboutThis::~DialogAboutThis()
{
    delete ui;
}
