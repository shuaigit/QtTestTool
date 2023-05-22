#include "widget.h"
#include "ui_widget.h"

#include <QByteArray>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QMap>
#include <QString>
#include <QVector>


using namespace std;

static QMap<QString, QString> case2Describe = {
    {"MC50TEST-1", "isp drop info test"},
    {"MC50TEST-2", "vin test"},
    {"MC50TEST-3081", "get userdata by 3a stat"},
};

static QVector<QString> Case = {
    {"MC50TEST-1"},
};

static QByteArray imageData;
static QByteArray jpegbeginstrArray("\xff\xd9");

void testfun() {
    QByteArray hexArray("\x11\xff\xd9");
    QByteArray orignalArray("shuaibangguo");
    int gindex = hexArray.indexOf(jpegbeginstrArray);
    qDebug() << gindex;
    QByteArray choppedArray = orignalArray.chopped(2);
    qDebug() << orignalArray << choppedArray;
}
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createMenu();
    createEditor();
    socket = new QTcpSocket(this);
    gridLayout = new QGridLayout;
    sendBtn = new QPushButton(tr("发送"));
    imageLabel = new QLabel(tr("jpeg"), this);
    sendBtn->setEnabled(false);
    connectBtn = new QPushButton(tr("连接"));
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(connectBtn);
    mainLayout->addWidget(sendBtn);
    gridLayout->setMenuBar(menuBar);
    gridLayout->addWidget(imageLabel, 0, 0);
    gridLayout->addWidget(sendBtn, 1, 0);

    QGroupBox *ispGroup = new QGroupBox(tr("ISP"));
    QLabel *caseLabel = new QLabel(tr("CaseID:"));
    ispComboBox = new QComboBox(this);
    QMap<QString, QString>::Iterator it;
    for (it = case2Describe.begin(); it != case2Describe.end(); it++) {
        ispComboBox->addItem(tr(it.key().toStdString().c_str()));
    }
    for (auto &it : Case) {
        qDebug() << it;
    }
#if 0
    ispComboBox->addItem(tr("MC50TEST-1"));
    ispComboBox->addItem(tr("MC50TEST-2"));
    ispComboBox->addItem(tr("MC50TEST-3"));
    ispComboBox->addItem(tr("MC50TEST-4"));
#endif
    QGridLayout *ispLayout = new QGridLayout;
    ispLayout->addWidget(caseLabel, 0, 0);
    ispLayout->addWidget(ispComboBox, 1, 0);
    ispGroup->setLayout(ispLayout);
    gridLayout->addWidget(ispGroup, 2, 0);
    setLayout(gridLayout);
    connect(socket, &QAbstractSocket::connected, this, &Widget::EnableSendBtn);
    connect(socket, &QTcpSocket::readyRead, this, &Widget::readSocket);
    connect(sendBtn, &QPushButton::clicked, this, &Widget::sendMsg);
    connect(ispComboBox, SIGNAL(activated(int)), this, SLOT(ispCaseChanged(int)));
    testfun();
    running = true;
    m_pthread = new std::thread(&Widget::workerThread, this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createMenu()
{
    menuBar = new QMenuBar;
    toolMenu = new QMenu(tr("&Tool"), this);
    connectAction = toolMenu->addAction((tr("&Connect")));
    menuBar->addMenu(toolMenu);
    connect(connectAction, &QAction::triggered, this, &Widget::connectToServer);
}

void Widget::createEditor()
{
//    recvEditor->setPlainText(tr("接收信息"));
//    sendEditor->setPlainText(tr("发送信息"));
}

void Widget::connectToServer()
{
    dialog = new Dialog(socket, this);
    dialog->exec();
//    qDebug() << "Action Trigger";
}

void Widget::EnableSendBtn()
{
    sendBtn->setEnabled(true);
}

void Widget::readSocket()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    qDebug() << socket->bytesAvailable();
    recvData += socket->readAll();
    flag = true;
    lck.unlock();
    cv.notify_all();
}

void Widget::sendMsg()
{
#if 1
    qint64 alsend = socket->write("MC50TEST-1");
    qDebug() << alsend;
#endif
}

void Widget::ispCaseChanged(int index)
{
    socket->write(ispComboBox->currentText().toStdString().c_str());
    qDebug() << ispComboBox->currentText();
}

void Widget::workerThread()
{
    while (running) {
        std::unique_lock<std::mutex> lck(m_mutex);
        cv.wait(lck, [=]{return flag;});
        if (splitJpeg()) {
            showImage();
        }
        flag = false;
    }
}

void Widget::showThread()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    cv.wait(lck);
//    showImage();
}

void Widget::showImage()
{
    QImage jpeg;
    jpeg.loadFromData(jpegData);
    imageLabel->setPixmap(QPixmap::fromImage(jpeg));
    qDebug() << "show jpeg,jpeg size is" << jpegData.size() << "jpeg:" << jpeg.size();
    jpegData.clear();
}

bool Widget::splitJpeg()
{
    int tail = recvData.indexOf(jpegbeginstrArray);
    if (tail != -1) {
        jpegData = recvData.left(tail+2);
        recvData = recvData.remove(0, tail+2);
        return true;
    }
    if (true == jpegData.isEmpty()) {
        return false;
    }
    return false;
}
