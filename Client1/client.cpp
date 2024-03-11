#include "client.h"
#include "protocol.h"
#include "ui_client.h"
#include "index.h"
#include <QFile>
#include <QtDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    m_prh = new ResHandler;

    loadConfig();

    QObject::connect(&m_tcpSocket, &QTcpSocket::connected, this, &Client::showConnect);//信号槽，连接成功后弹窗提示
    QObject::connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &Client::recvMsg);//当有新的数据到来时，QTcpSocket会发射readyRead()信号，调用recvMsg函数
    /*每一次，在当前用来存放读数据的位置中(也就是可读缓冲区)，有可读数据可用，都会发送这个信号。
     * 当新的数据到来的时候，这个信号还会再发送且仅发送一次。
     * 比如:新的网络数据到达你的网络socket，或者新的数据块添加到你的设备。*/

    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
}

Client::~Client()
{
    delete ui;
    delete m_prh;
}

void Client::loadConfig(){  //加载配置函数
    //配置文件:  IP地址
    //          端口号
    QFile file(":/client.config");  //读取文件
       if(file.open(QIODevice::ReadOnly)){  //以只读权限打开文件
           QByteArray baData = file.readAll();  //读取文件全部内容
           QString strData = QString(baData);   //将文件内容转为QString类型存入strData中
           QStringList strList = strData.split("\r\n");     //以“\r\n”为标志分开字符串
           m_strIP = strList.at(0);         //前面为IP地址，后面为端口（转int类型）
           m_usPort = strList.at(1).toUInt();
           m_filePath = strList.at(2); //文件路径
           qDebug() << "Client  IP:" << m_strIP << "Port:" << m_usPort << "Path:" << m_filePath;
           file.close();    //关闭文件
       }
       else{    //否则提示打开失败
           QMessageBox::warning(this, "open file", "open file failed");
       }
}

void Client::showConnect(){     //连接服务器提示弹窗
    QMessageBox::information(this, "连接服务器", "连接成功");
}

void Client::handleRes(PDU *pdu)
{
    qDebug() << "Client recvMsg 消息类型：" << pdu->uiMsgType
             <<        "消息内容："         << pdu->caMsg;
    //类型具体在protocol.h头文件中以枚举类型定义
    switch (pdu->uiMsgType) {//判断消息类型
        case ENUM_MSG_TYPE_REGIST_RESPOND://注册
            m_prh->handleRegist(pdu);
            break;//一定要有break！！！

        case ENUM_MSG_TYPE_LOGIN_RESPOND://登录
            m_prh->handleLogin(pdu);
            break;

        case ENUM_MSG_TYPE_FIND_USER_RESPOND://查找用户
            m_prh->handleFindUser(pdu);
            break;

        case ENUM_MSG_TYPE_ONLINE_USER_RESPOND://查找在线用户
            m_prh->handleOnlineUser(pdu, m_strLoginName);
            break;
        case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND://添加好友的响应
            m_prh->handleAddFriend(pdu);
            break;
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST://其他客户端的好友申请
            m_prh->handleAddFriendRequest(pdu);
            break;
        case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND://其他客户端的好友申请
            m_prh->handleAddFriendAgree();
            break;
        case ENUM_MSG_TYPE_ONLINE_FRIEND_RESPOND://刷新好友列表
            m_prh->handleOnlineFriend(pdu);
            break;
        case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND://删除好友
            m_prh->handleDeleteFriend(pdu);
            break;
        case ENUM_MSG_TYPE_CHAT_REQUEST://聊天
            m_prh->handleChat(pdu);
            break;
        case ENUM_MSG_TYPE_MKDIR_RESPOND://创建文件夹
            m_prh->handleMkDir(pdu);
            break;
        case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND://刷新文件列表
            m_prh->handleFlushFile(pdu);
            break;
        case ENUM_MSG_TYPE_DELETE_DIR_RESPOND://删除文件夹
            m_prh->handleDelDir(pdu);
            break;
        case ENUM_MSG_TYPE_DELETE_FILE_RESPOND://删除文件
            m_prh->handleDelFile(pdu);
            break;
        case ENUM_MSG_TYPE_RENAME_FILE_RESPOND://重命名文件
            m_prh->handleRenameFile(pdu);
            break;
        case ENUM_MSG_TYPE_MOVE_FILE_RESPOND://移动文件
            m_prh->handleMoveFile(pdu);
            break;
        case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND://上传文件
            m_prh->handleUploadFile(pdu);
            break;
        case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND://上传文件数据
            m_prh->handleUploadFileData(pdu);
            break;
        case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND://下载文件
            m_prh->handleDownloadFile(pdu);
            break;
        case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_RESPOND://下载文件数据
            m_prh->handleDownloadFileData(pdu);
            break;
        case ENUM_MSG_TYPE_SHARE_FILE_RESPOND://分享文件
            m_prh->handleShareFile();
            break;
        case ENUM_MSG_TYPE_SHARE_FILE_REQUEST://处理分享文件
            m_prh->handleShareFileRequest(pdu);
            break;
        case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND://同意接受分享文件
            m_prh->handleShareFileAgree(pdu);
            break;
        default:
            break;
    }
}

void Client::recvMsg()
{
    //处理粘包半包问题
    qDebug() << "recvMsg 接受消息长度：" << m_tcpSocket.bytesAvailable();
    QByteArray data = m_tcpSocket.readAll();
    buffer.append(data);    //新建成员变量buffer，接收pdu的信息加入buffer中
    while(buffer.size() >= int(sizeof(PDU))){   //循环处理pdu信息,若小于最小pdu则说明不足一个包，不处理
        PDU* pdu = (PDU*)buffer.data();    //读取pdu,其中包含信息大小
        if(buffer.size() < int(pdu->uiPDULen)){ //若buffer的大小小于pdu中信息的大小，说明为半包，跳出循环不处理
            break;
        }
        handleRes(pdu);
        buffer.remove(0, pdu->uiPDULen);    //删除处理过的信息，处理粘包问题
    }
}

void Client::sendPDU(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

Client& Client::getInstance(){
    static Client instance;
    return instance;
}

void Client::on_regist_PB_clicked() //槽函数，通过点击注册按钮发送请求
{
    //将用户名和密码读取出来后统一放入pdu的caData中
    QString strName = ui->user_LE->text();
    QString strPwd = ui->pwd_LE->text();
    if(strName.isEmpty() ||  strPwd.isEmpty()){
        QMessageBox::information(this, "失败", "用户名或密码不能为空");
        return;
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;//消息类型
    //caData[64]前32放入用户名，后32放入密码
    memcpy(pdu->caData, strName.toStdString().c_str(), strName.size());
    memcpy(pdu->caData+32, strPwd.toStdString().c_str(), strPwd.size());
    sendPDU(pdu);//通过m_tcpSocket连接服务器发送请求
}

void Client::on_login_PB_clicked()//登录槽函数
{
    QString strName = ui->user_LE->text();  //读取用户名和密码
    QString strPwd = ui->pwd_LE->text();
    m_strLoginName = strName;           //登录时将用户名留在m_strLoginName中
    if(strName.isEmpty() ||  strPwd.isEmpty()){ //判空
        QMessageBox::information(this, "失败", "用户名或密码不能为空");
        return;
    }
    PDU* pdu = mkPDU(0);    //创建pdu用于与服务器沟通
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;   //类型为登录类型
    //caData[64]前32放入用户名，后32放入密码
    memcpy(pdu->caData, strName.toStdString().c_str(), strName.size());
    memcpy(pdu->caData+32, strPwd.toStdString().c_str(), strPwd.size());
    sendPDU(pdu);
}
