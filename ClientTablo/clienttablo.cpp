#include "clienttablo.h"
#include "ui_clienttablo.h"

clientTablo::clientTablo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::clientTablo)
{
    ui->setupUi(this);
    ui->textEditHistory->setReadOnly(true);
    ui->lineEditTimer->setText("0:00:00");
    ui->frame->setLineWidth(5);
    ui->labelScoreTeamOne->setLineWidth(10);
    udpSocket = new QUdpSocket(this);   //coздаëм обьект соkета QUdpSocket
    connect (udpSocket, SIGNAL (readyRead()), this, SLOT (slotReadingUDPData()));  //для получения и отображения данныx соединяем сигнал сокета со слотом
    udpSocket->writeDatagram(QString("Connect").toUtf8(), QHostAddress::Broadcast, 2222);  //отправляем данные по широковешательному адресу на порт 2222
    portThisClient += udpSocket->localPort();
    udpSocket->abort();
    udpSocket->bind(QHostAddress::Broadcast, portThisClient); //задаём широковещательный адрес и порт на который сокет будет получать данные
    portThisClient = 1;
    /* При первом запуске приложения поместим текущее время в QLabel
     * */
    //qDebug() << QTime::currentTime().toString("hh:mm:ss");

    /* Инициализируем Таймер и подключим его к слоту,
     * который будет обрабатывать timeout() таймера
     * */
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerTimeOut()));
    timer->start(300); //запустим таймер
}

clientTablo::~clientTablo()
{
    delete udpSocket;
    delete ui;
}

void clientTablo:: slotReadingUDPData()  //чтение данных
{
    QHostAddress sender;    //agpec
    quint16 senderPort; //nopт отправителя
    QByteArray datagram;
    while(udpSocket->hasPendingDatagrams())
    {

        datagram.resize(udpSocket->pendingDatagramSize());  //узнаем размер ждущей обработки "датаграммы"
        udpSocket->readDatagram(datagram.data(),datagram.size(), &sender, &senderPort); //читаем данные
        //ui->textEditMessages->append(QString(datagram) + " IP: " + sender.toString() + " Port: " + QString("%1").arg(senderPort));
        //qDebug()<<datagram.data()<<"IP: " + sender.toString()<<"Port: "+QString("%1").arg(senderPort);
    }
    QStringList datagramMatch = QString(datagram.data()).split(";");
    QString hours = datagramMatch.value(0);
    QString minutes = datagramMatch.value(1);
    QString seconds = datagramMatch.value(2);
    if(minutes.length() == 1)   minutes.prepend("0");
    if(seconds.length() == 1)   seconds.prepend("0");
    ui->lineEditTimer->setText(hours+":"+minutes+":"+seconds);
    QString nameTeamOne = datagramMatch.value(3);
    QString scoreTeamOne = datagramMatch.value(4);
    QString nameTeamTwo = datagramMatch.value(5);
    QString scoreTeamTwo = datagramMatch.value(6);
    ui->labelNameTeamOne->setText(nameTeamOne);
    ui->labelNameTeamTwo->setText(nameTeamTwo);
    ui->labelScoreTeamOne->setText(scoreTeamOne);
    ui->labelScoreTeamTwo->setText(scoreTeamTwo);
    QString history = datagramMatch.value(7);
    QStringList historyList = history.split("*");
    int i = 0;
    ui->textEditHistory->clear();
    //qDebug() << history;
    while (i < historyList.length() - 1)
    {
        //qDebug() << "count:"<<i;
        ui->textEditHistory->append(" "+historyList.value(i));
        ++i;
    }
}

void clientTablo::slotTimerTimeOut()
{
    /* Ежесекундно обновляем данные по текущему времени
     * Перезапускать таймер не требуется
     * */
    //allowRequest = false;
    //ui->label->setText(QTime::currentTime().toString("hh:mm:ss"));
    udpSocket->writeDatagram(QString("Connect").toUtf8(), QHostAddress::Broadcast, 2222);  //отправляем данные по широковешательному адресу на порт 2222
    portThisClient += udpSocket->localPort();
    udpSocket->abort();
    udpSocket->bind(QHostAddress::Broadcast, portThisClient); //задаём широковещательный адрес и порт на который сокет будет получать данные
    portThisClient = 1;
}

