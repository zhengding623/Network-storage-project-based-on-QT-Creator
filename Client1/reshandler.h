#ifndef RESHANDLER_H
#define RESHANDLER_H
#include "protocol.h"
#include <QString>

class ResHandler
{
public:
    ResHandler();
    //好友系统
    void handleRegist(PDU* pdu);    //注册
    void handleLogin(PDU* pdu);     //登录
    void handleFindUser(PDU* pdu);  //查找用户
    void handleOnlineUser(PDU* pdu, QString& userName); //在线用户
    void handleAddFriend(PDU* pdu); //添加好友申请
    void handleAddFriendRequest(PDU* pdu);//同意好友申请
    void handleAddFriendAgree();    //添加好友成功响应
    void handleOnlineFriend(PDU *pdu);     //刷新好友列表
    void handleDeleteFriend(PDU* pdu);      //删除好友
    void handleChat(PDU* pdu);      //删除好友
    //文件系统
    void handleMkDir(PDU* pdu);      //新建文件夹
    void handleFlushFile(PDU* pdu);     //刷新文件
    void handleDelDir(PDU* pdu);     //删除文件夹
    void handleDelFile(PDU* pdu);     //删除文件
    void handleRenameFile(PDU* pdu);     //重命名文件
    void handleMoveFile(PDU* pdu);     //移动文件
    void handleUploadFile(PDU* pdu);     //上传文件
    void handleUploadFileData(PDU* pdu);     //上传文件数据
    void handleDownloadFile(PDU* pdu);     //下载文件
    void handleDownloadFileData(PDU* pdu);     //下载文件数据
    void handleShareFile();         //分享文件
    void handleShareFileRequest(PDU* pdu); //分享文件处理（接受）
    void handleShareFileAgree(PDU* pdu); //同意接受文件










};

#endif // RESHANDLER_H
