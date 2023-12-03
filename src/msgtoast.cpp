/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/msgtoast.h"

Toast::Toast(QWidget *parent)
    : QWidget(parent)
{
    this->toast_info=new QPushButton(this);
    //初始化时设置为不显示,因为组件默认初始化的时候会在屏幕左上角闪现一下,体验不佳
    this->toast_info->setVisible(false);
    this->toast_info->setFocusPolicy(Qt::NoFocus);
    this->toast_info->setFont(QFont("Microsoft YaHei"));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->master_timer_id=startTimer(200);
}

void Toast::makeText(QString text, ToastTime::Time time,ToastType::Type type,QWidget * parent)
{
    this->toast_msg_list.append(ToastMsg(text,time,type,parent));
}

void Toast::showMsg(QString text, ToastTime::Time time,ToastType::Type type,QWidget * parent)
{
    instance().makeText(text,time,type,parent);
}

Toast &Toast::instance()
{
    static Toast instance;
    instance.show();
    return instance;
}

void Toast::exit()
{
    this->close();
}

Toast::~Toast()
{
    delete toast_info;
    toast_msg_list.clear();
}

void Toast::fadeToastAnimation(int time)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(time);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}

void Toast::raiseToastAnimation(int time)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(time);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void Toast::adaptPage(QWidget * parent,int width, int height)
{
    if(width<50){
        this->toast_info->setGeometry(0,0,50,height);
    }else {
        this->toast_info->setGeometry(0,0,width,height);
    }
    //如传了父组件,则toast显示到父组件的中间位置,否则显示到屏幕下方,目前暂未实现父窗口移动跟随
    if(parent==nullptr){
        QScreen *scr=QApplication::primaryScreen();
        this->setGeometry((scr->availableSize().width()-toast_info->width())*0.5,scr->availableSize().height()*0.85,toast_info->width(),toast_info->height());
    }
    else{
        int x=parent->frameGeometry().x();
        int y=parent->frameGeometry().y();
        int pw=parent->frameGeometry().width();
        int ph=parent->frameGeometry().height();

        this->setGeometry(x+pw*0.5-toast_info->width()*0.5,y+ph*0.85,toast_info->width(),toast_info->height());
    }
}

void Toast::addMsg(QString msg)
{
    Toast::showMsg(msg);
}

void Toast::removeMsg(QString msg)
{
    this->toast_msg_list.removeOne(ToastMsg(msg,ToastTime::ToastTime_short,ToastType::Type::ToastType_info,nullptr));
}

void Toast::appExit()
{
    this->exit();
}

void Toast::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==this->master_timer_id){
        if(this->toast_msg_list.length()>0){
            this->msg_timer_id=startTimer(this->toast_msg_list.first().time_toInt());
            killTimer(this->master_timer_id);//暂停监控
            adaptPage(this->toast_msg_list.first().parent(),this->toast_msg_list.first().text().toLocal8Bit().length()*7.5+30);
            if(this->toast_msg_list.first().type()==ToastType::Type::ToastType_info){
                this->toast_info->setStyleSheet("border-radius:17px;color:#FFFFFF;font-weight:bold;background-color:#1890FF");
            }
            else if(this->toast_msg_list.first().type()==ToastType::Type::ToastType_warn){
                this->toast_info->setStyleSheet("border-radius:17px;color:#FFFFFF;font-weight:bold;background-color:#DC3545");
            }
            else{
                this->toast_info->setStyleSheet("border-radius:17px;color:#FFFFFF;font-weight:bold;background-color:#1890FF");
            }
            this->toast_info->setText(this->toast_msg_list.first().text());
            this->toast_info->setVisible(true);
            raiseToastAnimation(100);
            this->setVisible(true);
        }else {
            this->setVisible(false);
        }
    }else if (event->timerId()==msg_timer_id) {
        this->toast_msg_list.removeFirst();
        killTimer(this->msg_timer_id);
        this->master_timer_id=startTimer(200);//开启监控
        fadeToastAnimation(100);
    }else {
        makeText("i don't know the toast timer id:"+QString::number(event->timerId()));
    }
}

bool ToastMsg::operator ==(const ToastMsg &msg)
{
    if(this->m_text==msg.m_text){
        return true;
    }else {
        return false;
    }
}
