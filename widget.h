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
    QPushButton *snedBtn;
    QPushButton *connectBtn;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QLineEdit *ipEditor;
    QTextEdit *msgEditor;
    QMenuBar *menuBar;
    QMenu   *toolMenu;
    QAction *connectAction;
    Dialog *dialog;
    QTcpSocket *socket;

    void createMenu();
private slots:
    void connectToServer();
};

#endif // WIDGET_H
