#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QTcpSocket *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    socket = s;
//    resize(200,100);
    cntBtn = new QPushButton(tr("连接"), this);
    connect(cntBtn, &QPushButton::clicked, this, &Dialog::connectToServer);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Dialog::dispalyError);
    connect(socket, &QAbstractSocket::connected, this, &Dialog::closeDialog);
    ipEditor = new QLineEdit(this);
    ipEditor->setText("192.168.3.10");
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(ipEditor);
    mainLayout->addWidget(cntBtn);
    setLayout(mainLayout);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::connectToServer()
{
    socket->connectToHost(ipEditor->text(), 8888);
}

void Dialog::dispalyError(QAbstractSocket::SocketError socketError)
{
//    qDebug() << socketError << "xxx";
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Local Fortune Client"),
                                 tr("The host was not found. Please make sure "
                                    "that the server is running and that the "
                                    "server name is correct."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Connection Error"),tr("The connection was refused by the peer (or timed out)."));
        break;
    }
}

void Dialog::closeDialog()
{
    close();
}
