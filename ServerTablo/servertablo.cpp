#include "servertablo.h"
#include "ui_servertablo.h"

serverTablo::serverTablo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::serverTablo)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);   //coздаëм обьект соkета QUdpSocket
    ui->btnGoalTeamOne->setEnabled(false);
    ui->btnGoalTeamTwo->setEnabled(false);
    ui->textEditHistory->setReadOnly(true);

    udpSocket->bind(QHostAddress::Broadcast, 2222); //задаём широковещательный адрес и порт на который сокет будет получать данные
    connect (udpSocket, SIGNAL (readyRead()), this, SLOT (slotReadingUDPData()));  //для получения и отображения данныx соединяем сигнал сокета со слотом
    qDebug() << "Server start listening udp socket with port 2222";
}

serverTablo::~serverTablo()
{
    delete udpSocket;
    delete ui;
}

void serverTablo::slotReadingUDPData()
{
    count++;
    QHostAddress sender;    //agpec
    quint16 senderPort; //nopт отправителя
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());  //узнаем размер ждущей обработки "датаграммы"
        udpSocket->readDatagram(datagram.data(),datagram.size(), &sender, &senderPort); //читаем данные
        //qDebug()<<datagram.data()<<"IP: " + sender.toString()<<"Port: "+QString("%1").arg(senderPort);
    }
    //QString str = sender.toString();
    /*timerSender = new QTimer();
    connect(timerSender, SIGNAL(timeout()), this, SLOT(slotTimerSender()));
    timerSender->start(200);
    while(timerSenerIsConnected)
        int i=0;
    timerSenerIsConnected = true;
    disconnect(timerSender, SIGNAL(timeout()), this, SLOT(slotTimerSender()));*/
    QThread::msleep(70);   //пауза для завершения установки порта прослушивания в клиентском приложении
    QString str=QString("%1").arg(ui->sb_hours->value())+";"+QString("%1").arg(ui->sb_minutes->value())+";"+QString("%1").arg(ui->sb_seconds->value())+";"
            +ui->lineEditNameTeamOne->text()+";"+QString("%1").arg(ui->sbScoreTeamOne->value())+";"+ui->lineEditNameTeamTwo->text()+";"+QString("%1").arg(ui->sbScoreTeamTwo->value())+";"+history;
    //qDebug()<<"";
    //qDebug() <<"Send to client current info: "+str;
    udpSocket->writeDatagram(str.toUtf8(), QHostAddress::Broadcast, int(senderPort)+1);  //отправляем данные по широковешательному адресу на порт 2222
}

void serverTablo::on_sb_hours_valueChanged(int arg1)
{
    //qDebug() << "Hours: "+QString("%1").arg(arg1);
    if(arg1 == -1)
    {
        ui->sb_seconds->setValue(0);
        ui->sb_minutes->setValue(30);
        ui->sb_hours->setValue(1);
        ui->btn_startpause->setText("Старт");
        disconnect(timerMatch, SIGNAL(timeout()), this, SLOT(slotTimerSeconds()));
        timerMatch = nullptr;
    }
}

void serverTablo::on_sb_minutes_valueChanged(int arg1)
{
    //qDebug() << "Minutes: "+QString("%1").arg(arg1);
    if(arg1 == -1)
    {
        ui->sb_seconds->setValue(59);
        ui->sb_minutes->setValue(59);
        ui->sb_hours->setValue(ui->sb_hours->value() - 1);
    }
}

void serverTablo::on_sb_seconds_valueChanged(int arg1)
{
    //qDebug() << "Seconds: "+QString("%1").arg(arg1);
    if(arg1 == -1)
    {
        if(ui->sb_minutes->value() == 0 && ui->sb_hours->value() == 0)
        {
            disconnect(timerMatch, SIGNAL(timeout()), this, SLOT(slotTimerSeconds()));
            timerMatch = nullptr;
            ui->textEditHistory->append("Матч окончен!");
            history += "Матч окончен!*";

            ui->sb_seconds->setValue(0);
            ui->sb_minutes->setValue(30);
            ui->sb_hours->setValue(1);
            ui->btn_startpause->setText("Старт");
            ui->lineEditNameTeamOne->setEnabled(true);
            ui->lineEditNameTeamTwo->setEnabled(true);
            ui->sb_hours->setEnabled(true);
            ui->sb_minutes->setEnabled(true);
            ui->sb_seconds->setEnabled(true);
            //ui->sbScoreTeamOne->setEnabled(true);
            //ui->sbScoreTeamTwo->setEnabled(true);
            ui->btnGoalTeamOne->setEnabled(false);
            ui->btnGoalTeamTwo->setEnabled(false);
        }
        else
        {
            ui->sb_seconds->setValue(59);
            ui->sb_minutes->setValue(ui->sb_minutes->value() - 1);
        }
    }
}

void serverTablo::on_btn_startpause_clicked()
{
    if(ui->btn_startpause->text() == "Продолжить" || ui->btn_startpause->text() == "Старт")
    {
        if(ui->btn_startpause->text() == "Старт")
        {
            ui->textEditHistory->append("Матч начался!");
            history += "Матч начался!*";
            ui->lineEditNameTeamOne->setEnabled(false);
            ui->lineEditNameTeamTwo->setEnabled(false);
            ui->sb_hours->setEnabled(false);
            ui->sb_minutes->setEnabled(false);
            ui->sb_seconds->setEnabled(false);
            //ui->sbScoreTeamOne->setEnabled(false);
            //ui->sbScoreTeamTwo->setEnabled(false);
            if (ui->lineEditGoalTeamOne->text() != "")  ui->btnGoalTeamOne->setEnabled(true);
            if (ui->lineEditGoalTeamTwo->text() != "")  ui->btnGoalTeamTwo->setEnabled(true);
        }
        else if(ui->btn_startpause->text() == "Продолжить")
        {
            ui->textEditHistory->append("Матч продолжается!");
            history += "Матч продолжается!*";
            ui->lineEditNameTeamOne->setEnabled(false);
            ui->lineEditNameTeamTwo->setEnabled(false);
            ui->sb_hours->setEnabled(false);
            ui->sb_minutes->setEnabled(false);
            ui->sb_seconds->setEnabled(false);
            //ui->sbScoreTeamOne->setEnabled(false);
            //ui->sbScoreTeamTwo->setEnabled(false);
            if (ui->lineEditGoalTeamOne->text() != "")  ui->btnGoalTeamOne->setEnabled(true);
            if (ui->lineEditGoalTeamTwo->text() != "")  ui->btnGoalTeamTwo->setEnabled(true);
        }
        ui->btn_startpause->setText("Пауза");
        timerMatch = new QTimer();
        connect(timerMatch, SIGNAL(timeout()), this, SLOT(slotTimerSeconds()));
        timerMatch->start(1000); //запустим таймер

    }
    else if(ui->btn_startpause->text() == "Пауза")
    {
        ui->textEditHistory->append("Матч приостановлен!");
        history += "Матч приостановлен!*";
        ui->btn_startpause->setText("Продолжить");
        disconnect(timerMatch, SIGNAL(timeout()), this, SLOT(slotTimerSeconds()));
        timerMatch = nullptr;
        ui->lineEditNameTeamOne->setEnabled(true);
        ui->lineEditNameTeamTwo->setEnabled(true);
        ui->sb_hours->setEnabled(true);
        ui->sb_minutes->setEnabled(true);
        ui->sb_seconds->setEnabled(true);
        //ui->sbScoreTeamOne->setEnabled(true);
        //ui->sbScoreTeamTwo->setEnabled(true);
        ui->btnGoalTeamOne->setEnabled(false);
        ui->btnGoalTeamTwo->setEnabled(false);
    }
}

void serverTablo::on_btn_resetTime_clicked()
{
    if (ui->btn_startpause->text() != "Старт")
    {
        ui->textEditHistory->append("Время матча обновлено!");
        history += "Время матча обновлено!*";
        ui->sb_seconds->setValue(0);
        ui->sb_minutes->setValue(30);
        ui->sb_hours->setValue(1);
        if(ui->btn_startpause->text() == "Продолжить")
        {
            ui->btn_startpause->setText("Старт");
            ui->lineEditNameTeamOne->setEnabled(true);
            ui->lineEditNameTeamTwo->setEnabled(true);
            ui->sb_hours->setEnabled(true);
            ui->sb_minutes->setEnabled(true);
            ui->sb_seconds->setEnabled(true);
            //ui->sbScoreTeamOne->setEnabled(true);
            //ui->sbScoreTeamTwo->setEnabled(true);
        }
    }
}

void serverTablo::on_btnResetAllData_clicked()
{
    if(ui->btn_startpause->text() == "Пауза")
    {
        disconnect(timerMatch, SIGNAL(timeout()), this, SLOT(slotTimerSeconds()));
        timerMatch = nullptr;
    }
    ui->textEditHistory->clear();
    ui->textEditHistory->append("Матч вскоре будет перезапущен!");    //Все херня, Миша, давай по новой!
    history = "Матч вскоре будет перезапущен!*";
    ui->sbScoreTeamOne->setValue(0);
    ui->sbScoreTeamTwo->setValue(0);
    ui->lineEditNameTeamOne->setText("Клуб 1");
    ui->lineEditNameTeamTwo->setText("Клуб 2");
    ui->sb_seconds->setValue(0);
    ui->sb_minutes->setValue(30);
    ui->sb_hours->setValue(1);
    ui->btn_startpause->setText("Старт");
    ui->lineEditNameTeamOne->setEnabled(true);
    ui->lineEditNameTeamTwo->setEnabled(true);
    ui->sb_hours->setEnabled(true);
    ui->sb_minutes->setEnabled(true);
    ui->sb_seconds->setEnabled(true);
    //ui->sbScoreTeamOne->setEnabled(true);
    //ui->sbScoreTeamTwo->setEnabled(true);
    ui->btnGoalTeamOne->setEnabled(false);
    ui->btnGoalTeamTwo->setEnabled(false);
}

void serverTablo::slotTimerSeconds()
{
    /* Ежесекундно обновляем данные по текущему времени матча*/
    ui->sb_seconds->setValue(ui->sb_seconds->value()-1);
}

void serverTablo::slotTimerSender()
{
    timerSenerIsConnected = false;
}

void serverTablo::on_btnGoalTeamOne_clicked()
{
    ui->sbScoreTeamOne->setValue(ui->sbScoreTeamOne->value()+1);
    ui->textEditHistory->append(ui->lineEditGoalTeamOne->text()+" ("+ui->lineEditNameTeamOne->text()+") забил гол!");
    history += ui->lineEditGoalTeamOne->text()+" ("+ui->lineEditNameTeamOne->text()+") забил гол!*";
    ui->lineEditGoalTeamOne->clear();
}

void serverTablo::on_btnGoalTeamTwo_clicked()
{
    ui->sbScoreTeamTwo->setValue(ui->sbScoreTeamTwo->value()+1);
    ui->textEditHistory->append(ui->lineEditGoalTeamTwo->text()+" ("+ui->lineEditNameTeamTwo->text()+") забил гол!");
    history += ui->lineEditGoalTeamTwo->text()+" ("+ui->lineEditNameTeamTwo->text()+") забил гол!*";
    ui->lineEditGoalTeamTwo->clear();

}

void serverTablo::on_lineEditGoalTeamOne_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->btn_startpause->text() == "Пауза")  ui->btnGoalTeamOne->setEnabled(true);
    else    ui->btnGoalTeamOne->setEnabled(false);
}

void serverTablo::on_lineEditGoalTeamTwo_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->btn_startpause->text() == "Пауза")  ui->btnGoalTeamTwo->setEnabled(true);
    else    ui->btnGoalTeamTwo->setEnabled(false);
}


