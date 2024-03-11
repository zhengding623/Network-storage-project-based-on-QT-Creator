#ifndef CLIENT_H
#define CLIENT_H

#include "ui_client.h"
#include "protocol.h"
#include "reshandler.h"
#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    ~Client();

    QTcpSocket m_tcpSocket;
    QString m_strLoginName;
    ResHandler* m_prh;
    QString m_filePath;
    QByteArray buffer;


    void loadConfig();
    void mtcpSocket();
    void showConnect();
    PDU* readPDU();
    void handleRes(PDU* pdu);
    void recvMsg();
    void sendPDU(PDU* pdu);

    static Client& getInstance();

private slots:
    void on_regist_PB_clicked();
    void on_login_PB_clicked();

private:
    Ui::Client *ui;
    QString m_strIP;
    quint16 m_usPort;

    explicit Client(QWidget *parent = nullptr);
    Client(const Client& instance) = delete;
    Client& operator = (const Client&) = delete;
    //void on_PushButton_clicked();

};
#endif // CLIENT_H
