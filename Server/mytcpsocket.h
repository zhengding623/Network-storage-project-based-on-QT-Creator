#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include "protocol.h"
#include "msghandler.h"
#include <QTcpSocket>
#include <QObject>

//class MsgHandler;

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();

    MsgHandler* msghandler;

    QTcpSocket m_tcpSocket;
    QString m_strName;
    MsgHandler* m_pmh;
    QByteArray buffer;

    void clientOffline();
    void recvMsg(); 
    PDU* handlePDU(PDU* pdu);
    void writePDU(PDU* repdu);

signals:
    void offline(MyTcpSocket* mysocket);



};

#endif // MYTCPSOCKET_H
