#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createMenu();
    socket = new QTcpSocket(this);
    gridLayout = new QGridLayout;
    snedBtn = new QPushButton(tr("发送"));
    connectBtn = new QPushButton(tr("连接"));
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(connectBtn);
    mainLayout->addWidget(snedBtn);
    gridLayout->setMenuBar(menuBar);
    gridLayout->addWidget(connectBtn, 0, 0);
    gridLayout->addWidget(snedBtn, 1, 1);
    setLayout(gridLayout);
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

void Widget::connectToServer()
{
    dialog = new Dialog(socket, this);
    dialog->exec();
    qDebug() << "Action Trigger";
}
