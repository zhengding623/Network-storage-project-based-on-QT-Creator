#include "server.h"
#include "ui_server.h"
#include "mytcpserver.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    //加载配置
    loadConfig();
    //监听
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP), m_usPort);
}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()   //加载配置函数
{
    //配置文件:  IP地址
    //          端口号
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){//以只读权限打开文件
       QByteArray baData = file.readAll();//读取文件全部内容
       QString strData = QString(baData);//将文件内容转为QString类型存入strData中
       QStringList strList = strData.split("\r\n"); //以“\r\n”为标志分开字符串
       m_strIP = strList.at(0);//前面为IP地址，后面为端口（转short类型）
       m_usPort = strList.at(1).toUShort();
       m_filePath = strList.at(2); //文件路径
       qDebug() << "Server  IP:" << m_strIP << "Port:" << m_usPort << "Path:" << m_filePath;
       file.close();//关闭文件
    }
    else{   //否则提示打开失败
       QMessageBox::warning(this, "open file", "open file failed");
    }
}

Server &Server::getInstance()
{
  static Server instance;
  return instance;
}
