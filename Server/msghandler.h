#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include "protocol.h"
#include "operatedb.h"
//#include "mytcpsocket.h"

class MyTcpSocket;//前置声明防止头文件相互包含

class MsgHandler : public QObject
{
    Q_OBJECT
private:
    bool m_fileUpload;//是否在上传文件
    QFile m_UploadFile;//上传的文件对象
    qint64 m_UploadTotal;//上传文件的大小
    qint64 m_UploadReceived;//已上传的大小

    bool m_fileDownload;//是否正在下载文件
    QFile downloadFile;//下载的文件对象


public:
    explicit MsgHandler(QObject *parent = nullptr);
    MyTcpSocket* mytcpsocket;
    void copyDir(QString strSrcPath, QString strDestDir);//递归复制文件夹

    PDU* handleRegist(PDU* pdu);                    //注册
    PDU* handleLogin(PDU* pdu, QString& userName);   //登录
    PDU* handleFindUser(PDU* pdu);                  //查找用户
    PDU* handleOnlineUser();                        //在线用户
    PDU* handleAddFriend(PDU* pdu);                 //添加好友
    PDU* handleAddFriendAgree(PDU* pdu);            //同意添加好友
    PDU* handleOnlineFriend(PDU* pdu);             //刷新好友列表
    PDU* handleDeleteFriend(PDU* pdu);             //删除好友
    void handleChat(PDU* pdu);                      //聊天

    PDU* handleMkDir(PDU* pdu);                     //新建文件夹
    PDU* handleFlushFile(PDU* pdu);                 //刷新文件
    PDU* handleDelDir(PDU* pdu);                    //删除文件夹
    PDU* handleDelFile(PDU* pdu);                   //删除文件
    PDU* handleRenameFile(PDU* pdu);                //文件重命名
    PDU* handleMoveFile(PDU* pdu);                  //移动文件
    PDU* handleUploadFile(PDU* pdu);                //上传文件
    PDU* handleUploadFileData(PDU* pdu);            //上传文件数据
    PDU* handleDownloadFile(PDU* pdu);                //下载文件
    PDU* handleDownloadFileData(MyTcpSocket *socket);            //下载文件数据
    PDU* handleShareFile(PDU* pdu);                  //分享文件
    PDU* handleShareFileAgree(PDU* pdu);                  //同意分享文件







signals:

};

#endif // MSGHANDLER_H
