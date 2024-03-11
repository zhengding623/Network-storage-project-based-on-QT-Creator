#include "mytcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
#include "msghandler.h"
#include <QDebug>


MyTcpSocket::MyTcpSocket()
{
    m_pmh = new MsgHandler;
    connect(this, &QTcpSocket::readyRead, this, &MyTcpSocket::recvMsg);
    connect(this, &QTcpSocket::disconnected, this, &MyTcpSocket::clientOffline);//断开连接后调用用户离线函数

}

MyTcpSocket::~MyTcpSocket()
{
    delete m_pmh;
}

void MyTcpSocket::writePDU(PDU *repdu)//发送函数
{
    if(repdu == NULL) return;
    write((char*)repdu, repdu->uiPDULen);
    free(repdu);
    repdu = NULL;
}

PDU *MyTcpSocket::handlePDU(PDU *pdu)//具体根据消息返回类型进行不同的功能
{
    qDebug() << "\n\n\nServer recvMsg 消息类型：" << pdu->uiMsgType
                 << "消息内容：" << pdu->caMsg
                 << "参数内容：" << pdu->caData;
    //类型具体在protocol.h头文件中以枚举类型定义
    switch(pdu->uiMsgType){ //判断类型
        case ENUM_MSG_TYPE_REGIST_REQUEST://注册
            return m_pmh->handleRegist(pdu);
            break;

        case ENUM_MSG_TYPE_LOGIN_REQUEST://登录
            return m_pmh->handleLogin(pdu, m_strName);
            break;

        case ENUM_MSG_TYPE_FIND_USER_REQUEST://查找用户
            return m_pmh->handleFindUser(pdu);
            break;

        case ENUM_MSG_TYPE_ONLINE_USER_REQUEST://在线用户
            return m_pmh->handleOnlineUser();
            break;

        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST://添加好友申请
            return m_pmh->handleAddFriend(pdu);
            break;

        case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST://处理同意好友申请
            return m_pmh->handleAddFriendAgree(pdu);
            break;

        case ENUM_MSG_TYPE_ONLINE_FRIEND_REQUEST://刷新好友列表
            return m_pmh->handleOnlineFriend(pdu);
            break;

        case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST://删除好友
            return m_pmh->handleDeleteFriend(pdu);
            break;

        case ENUM_MSG_TYPE_CHAT_REQUEST://聊天
            m_pmh->handleChat(pdu);
            break;

        case ENUM_MSG_TYPE_MKDIR_REQUEST://新建文件夹
            return m_pmh->handleMkDir(pdu);
            break;

        case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST://刷新文件夹
            return m_pmh->handleFlushFile(pdu);
            break;

        case ENUM_MSG_TYPE_DELETE_DIR_REQUEST://删除文件夹
            return m_pmh->handleDelDir(pdu);
            break;

        case ENUM_MSG_TYPE_DELETE_FILE_REQUEST://删除文件
            return m_pmh->handleDelFile(pdu);
            break;

        case ENUM_MSG_TYPE_RENAME_FILE_REQUEST://文件重命名
            return m_pmh->handleRenameFile(pdu);
            break;

        case ENUM_MSG_TYPE_MOVE_FILE_REQUEST://移动文件
            return m_pmh->handleMoveFile(pdu);
            break;

        case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST://上传文件
            return m_pmh->handleUploadFile(pdu);
            break;

        case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST://上传文件
            return m_pmh->handleUploadFileData(pdu);
            break;

        case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST://下载文件
            return m_pmh->handleDownloadFile(pdu);
            break;

        case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST://下载文件数据
            return m_pmh->handleDownloadFileData(this);
            break;

        case ENUM_MSG_TYPE_SHARE_FILE_REQUEST://分享文件
            return m_pmh->handleShareFile(pdu);
            break;

        case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST://分享文件
            return m_pmh->handleShareFileAgree(pdu);
            break;

    default:
            break;
    }
    return NULL;
}

void MyTcpSocket::recvMsg()
{
    //处理粘包半包问题
    qDebug() << "recvMsg 接受消息长度：" << this->bytesAvailable();
    QByteArray data = this->readAll(); //将消息读取出来
    buffer.append(data);    //新建成员变量buffer，接收pdu的信息加入buffer中
    while(buffer.size() >= int(sizeof(PDU))){   //循环处理pdu信息,若小于最小pdu则说明不足一个包，不处理
        PDU* pdu = (PDU*)buffer.data();    //读取pdu,其中包含信息大小
        if(buffer.size() < int(pdu->uiPDULen)){ //若buffer的大小小于pdu中信息的大小，说明为半包，跳出循环不处理
            qDebug() << "buffer.size():" << buffer.size();
            break;
        }
        PDU* repdu = handlePDU(pdu);    //整包处理
        writePDU(repdu);
        buffer.remove(0, pdu->uiPDULen);    //删除处理过的信息，处理粘包问题
    }
}

void MyTcpSocket::clientOffline()   //处理用户下线
{
    qDebug() << "clientOffline";
    OperateDb::getInstance().handleOffline(m_strName.toStdString().c_str());//调用数据库离线函数
    emit offline(this);//发送信号

}
