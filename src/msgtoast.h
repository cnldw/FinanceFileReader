/**
 *from qt_-lib_toast
 **/

#ifndef MSGTOAST_H
#define MSGTOAST_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QScreen>
#include <QVector>
#include <QDebug>
#include <QPropertyAnimation>

namespace ToastTime {
enum Time{
    ToastTime_short=1,
    ToastTime_normal=2,
    ToastTime_long=3,
};
}

namespace ToastType {
enum Type{
    ToastType_info=1,
    ToastType_warn=2,
};
}

class ToastMsg{
private:
    QString m_text;
    ToastTime::Time m_time;
    ToastType::Type m_type;
    QWidget * m_parent;
public:
    inline ToastMsg(){}
    inline ToastMsg(QString text,ToastTime::Time time,ToastType::Type type,QWidget * parent):m_text(text),m_time(time),m_type(type),m_parent(parent){}
    inline QString text(){return m_text;}
    inline ToastTime::Time time(){return m_time;}
    inline ToastType::Type type(){return m_type;}
    inline int time_toInt(){return m_time*1000;}
    inline QWidget * parent(){return m_parent;}
    bool operator ==(const ToastMsg& msg);
};

class Toast : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE void makeText(QString text,ToastTime::Time time=ToastTime::Time::ToastTime_short,ToastType::Type type=ToastType::Type::ToastType_info,QWidget * parent=nullptr);
    static void showMsg(QString text,ToastTime::Time time=ToastTime::Time::ToastTime_short,ToastType::Type type=ToastType::Type::ToastType_info,QWidget * parent=nullptr);
    static Toast& instance();
    Q_INVOKABLE void exit();
private:
    Toast(QWidget *parent = nullptr);
    ~Toast();
    QPushButton *toast_info=nullptr;
    QString m_text;
    QVector<ToastMsg> toast_msg_list;
    int master_timer_id;
    int msg_timer_id;
    void fadeToastAnimation(int time);
    void raiseToastAnimation(int time);
    void adaptPage(QWidget * parent,int width,int height=40);
public slots:
    void addMsg(QString msg);
    void removeMsg(QString msg);
    void appExit();
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};
#endif // MSGTOAST_H
