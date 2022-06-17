#ifndef CLIENTTABLO_H
#define CLIENTTABLO_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QThread>
#include <QTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class clientTablo; }
QT_END_NAMESPACE

class clientTablo : public QMainWindow
{
    Q_OBJECT

public:
    clientTablo(QWidget *parent = nullptr);
    ~clientTablo();


private:
    Ui::clientTablo *ui;
    QUdpSocket *udpSocket;  //указатель на объект класса QUdpSocket
    quint16 portThisClient = 1;
    /* Да сам объект QTimer */
    QTimer *timer;
    bool allowRequest = true;

public slots:
    void slotReadingUDPData();  //чтение данных с udpSocket
    void slotTimerTimeOut();

};
#endif // CLIENTTABLO_H
