#ifndef SERVERTABLO_H
#define SERVERTABLO_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QThread>
#include <QTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class serverTablo; }
QT_END_NAMESPACE

class serverTablo : public QMainWindow
{
    Q_OBJECT

public:
    serverTablo(QWidget *parent = nullptr);
    ~serverTablo();

private:
    Ui::serverTablo *ui;
    QUdpSocket* udpSocket;  //указатель на объект класса QUdpSocket
    int count = 0;
    QTimer* timerMatch;
    QTimer* timerSender;
    bool timerSenerIsConnected = true;
    QString history="";

public slots:
    void slotReadingUDPData();


private slots:
    void on_sb_hours_valueChanged(int arg1);
    void on_btn_startpause_clicked();
    void slotTimerSeconds();
    void slotTimerSender();
    void on_btn_resetTime_clicked();
    void on_sb_minutes_valueChanged(int arg1);
    void on_sb_seconds_valueChanged(int arg1);
    void on_btnGoalTeamOne_clicked();
    void on_btnGoalTeamTwo_clicked();
    void on_lineEditGoalTeamOne_textChanged(const QString &arg1);
    void on_lineEditGoalTeamTwo_textChanged(const QString &arg1);
    void on_btnResetAllData_clicked();
};

#endif // SERVERTABLO_H
