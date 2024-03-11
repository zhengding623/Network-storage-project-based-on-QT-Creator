#include "mytcpserver.h"
#include "mytcpsocket.h"

#include <QDebug>

MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::deletSocket(MyTcpSocket *mysocket)//删除离线用户的socket
{
    m_tcpSocketList.removeOne(mysocket);//移除函数，稍晚一点移除
    mysocket->deleteLater();
    mysocket = NULL;

    foreach(MyTcpSocket *pSocket, m_tcpSocketList){
        qDebug() << "deletSocket: " << pSocket->m_strName;
    }
}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "新客户端连接";
    MyTcpSocket* pTcpSocket = new MyTcpSocket;//申请新属性的链表
    pTcpSocket->setSocketDescriptor(handle);//将句柄绑定套接字
    m_tcpSocketList.append(pTcpSocket);//绑定好后添加进成员变量的链表中

    connect(pTcpSocket, &MyTcpSocket::offline, this, &MyTcpServer::deletSocket);//连接移除socket的信号槽
}

void MyTcpServer::resend(char *tarName, PDU *pdu)//转发函数，将pdu转发给目标客户端
{
    qDebug() << "resend";
    if(tarName == NULL || pdu == NULL){
        return;
    }
    for(int i = 0; i < m_tcpSocketList.size(); i++){//遍历已连接的socket，找到目标客户端的socket
        if(tarName == m_tcpSocketList.at(i)->m_strName){
            m_tcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);//使用目标客户端的socket发送
            qDebug() << "i=" << i;
            break;//用户名唯一
        }
    }
}
