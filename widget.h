#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTcpSocket>
#include <QGroupBox>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <QComboBox>

QT_BEGIN_NAMESPACE
class QLabel;
class QImage;
QT_END_NAMESPACE
#include "dialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QPushButton *sendBtn;
    QPushButton *connectBtn;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QLineEdit *ipEditor;
    QMenuBar *menuBar;
    QMenu   *toolMenu;
    QAction *connectAction;
    Dialog *dialog;
    QTcpSocket *socket;
    QLabel *imageLabel;
    QImage *jpegImage;
    std::mutex m_mutex;
    std::thread *m_pthread;
    std::thread *m_pshow;
    std::condition_variable cv;
    QComboBox *ispComboBox;

    void createMenu();
    void createEditor();
    void connectSlot();
    void workerThread();
    void showThread();
private slots:
    void connectToServer();
    void EnableSendBtn();
    void readSocket();
    void sendMsg();
    void ispCaseChanged(int);
    void showImage();

signals:
    void showImageSignal();
protected:
    QByteArray recvData;
    QByteArray jpegData;
    bool flag;
    bool running;
    bool splitJpeg();
};

#endif // WIDGET_H
