#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QTcpSocket *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    socket = s;
    resize(200,100);
    cntBtn = new QPushButton(tr("连接"), this);
    connect(cntBtn, &QPushButton::clicked, this, &Dialog::connectToServer);
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
    socket->connectToHost(ipEditor->text(), 5678);
}
