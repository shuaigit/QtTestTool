#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QTcpSocket *s, QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTcpSocket *socket;
    QPushButton *cntBtn;
    QLineEdit *ipEditor;
    QHBoxLayout *mainLayout;
public slots:
    void connectToServer();
    void dispalyError(QAbstractSocket::SocketError socketError);
    void closeDialog();
};

#endif // DIALOG_H
