#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QTcpServer>
#include "mytcpsocket.h"


class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer &getInstance();
    void incomingConnection(qintptr handle);

    void resend(char* tarName, PDU* pdu);

private:
    MyTcpServer();

    MyTcpServer(const MyTcpServer& instance) = delete;
    MyTcpServer& operator= (const MyTcpServer&) = delete;

    QList<MyTcpSocket*> m_tcpSocketList;//新建成员变量QList类型的链表

public slots:
    void deletSocket(MyTcpSocket* mysocket);
};

#endif // MYTCPSERVER_H
