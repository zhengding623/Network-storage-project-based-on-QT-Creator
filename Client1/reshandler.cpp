#include "reshandler.h"
#include "client.h"
#include "index.h"
#include <QDebug>
#include <QMessageBox>

ResHandler::ResHandler()
{

}

void ResHandler::handleRegist(PDU *pdu)
{
    qDebug() << "handleRegist";

    bool ret;   //用于接收从服务器返回的结果
    memcpy(&ret, pdu->caData, sizeof(bool));
    qDebug() << "ret = " << ret;
    //返回结果为：1   注册成功
    //返回结果为：0   注册失败
    //弹窗显示
    if(ret){
        QMessageBox::information(&Client::getInstance()/*在客户端显示，通过单例模式 使用地址*/, "注册", "注册成功");
    }
    else{
        QMessageBox::information(&Client::getInstance(), "注册", "注册失败，用户名已存在");
    }
}

void ResHandler::handleLogin(PDU *pdu)
{
    qDebug() << "handleLogin";

    bool ret;   //用于接收结果
    memcpy(&ret, pdu->caData, sizeof(bool));
    //返回值为1：登录成功，直接进入主页面，将登录界面隐藏；
    //返回值为0：登录失败，弹窗警告
    if(ret){
        Index::getInstance().show();
        Client::getInstance().hide();/*在客户端显示，通过单例模式调用函数*/
    }
    else{
        QMessageBox::information(&Client::getInstance(), "登录", "登录失败，密码错误或用户不存在");
    }
}

void ResHandler::handleFindUser(PDU *pdu)
{
    qDebug() << "handleFindUser";

    int ret;//用于接收返回值
    char caName[32] = {'\0'};//用于接收用户名
    memcpy(caName, pdu->caData, 32);
    memcpy(&ret, pdu->caData+32, sizeof(int));
    qDebug() << "ret = " << ret;
    QString msg;
    Friend* friendPage = Index::getInstance().getFriend();
    //返回值为-1：用户不存在
    //返回值为1：用户在线
    //返回值为0：用户离线
    if(ret == -1){
        QMessageBox::information(friendPage, "查找用户", QString("%1 用户不存在").arg(caName));
    }
    else if(ret == 0){
        QMessageBox::information(friendPage, "查找用户", QString("%1 用户离线").arg(caName));
    }
    else if(ret == 1){ 
        int res = QMessageBox::information(friendPage, "查找用户", QString("%1 用户在线").arg(caName), "添加好友", "取消");//若用户在线则提供添加好友的选项
        if(res == 0){       //选择添加好友
            //获取当前用户名
            QString curName = Client::getInstance().m_strLoginName;
            //目标用户名caName
            PDU* pdu = mkPDU(0);//构建pdu
            pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;//添加好友申请
            memcpy(pdu->caData, curName.toStdString().c_str(), 32);
            memcpy(pdu->caData+32, caName, 32);
            Client::getInstance().sendPDU(pdu);
        }
    }

}

void ResHandler::handleOnlineUser(PDU *pdu, QString& userName)
{
    qDebug() << "handleOnlineUser";

    QStringList userList;       //创建一个用户列表
    int size = pdu->uiMsgLen/32;//用户的个数
    char caTmp[32] = {'\0'};    //用户名临时放入这个变量中
    for(int i = 0; i < size; i++){
        memcpy(caTmp, pdu->caMsg+i*32, 32);
        if(userName == QString(caTmp)){//若名字为本客户端则跳过
            continue;
        }
        userList.append(QString(caTmp));//在用户列表中填入当前的用户名
    }
    //单例模式在好友界面中展示在线用户列表栏，栏中显示列表内容
    Index::getInstance().getFriend()->m_pOnlineUser->showOnlineUser(userList);
}

void ResHandler::handleAddFriend(PDU *pdu)
{
    int ret;//用于接收返回值
    memcpy(&ret, pdu->caData, sizeof(int));
    qDebug() << "handleAddFriend ret = " << ret;
    QString msg;
    //返回值为-2：已是好友
    //返回值为-1：添加错误
    //返回值为1：发送成功
    //返回值为0：用户离线
    if(ret == -1){
        msg = "添加错误";
    }
    else if(ret == 0){
        msg = "用户离线";
    }
    else if(ret == -2){
        msg = "对方已经是你的好友";
    }
    else{
        return;
    }
    QMessageBox::information(Index::getInstance().getFriend(), "添加好友", msg);
}

void ResHandler::handleAddFriendRequest(PDU *pdu)
{
    char caCurName[32] = {'\0'};//将申请好友的客户端name保存下来
    memcpy(caCurName, pdu->caData, 32);
    int ret = QMessageBox::question(Index::getInstance().getFriend(),
                                    "好友申请",
                                    QString("是否同意 %1 的好友请求").arg(caCurName));//弹窗询问是否同意，是返回1，否返回0
    if(ret != QMessageBox::Yes){//如果不同意则直接返回
        return ;
    }
    PDU* repdu = mkPDU(0);//同意则构建返回pdu
    repdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;//添加类型为同意好友申请的响应
    memcpy(repdu->caData, caCurName, 32);
    memcpy(repdu->caData+32, pdu->caData+32, 32);
    Client::getInstance().sendPDU(repdu);
}

void ResHandler::handleAddFriendAgree()
{
    QMessageBox::information(
                Index::getInstance().getFriend(),
                "添加好友",
                "添加好友成功"
                );
    Index::getInstance().getFriend()->flushFriend();
}

void ResHandler::handleOnlineFriend(PDU *pdu)
{
    qDebug() << "ResHandler handleOnlineFriend";

    QStringList friendList;       //创建一个用户列表
    int size = pdu->uiMsgLen/32;  //用户的个数
    qDebug() << size;
    char caTmp[32] = {'\0'};    //用户名临时放入这个变量中
    for(int i = 0; i < size; i++){

        memcpy(caTmp, pdu->caMsg+i*32, 32);
        friendList.append(QString(caTmp));//在用户列表中填入当前的用户名
    }
    //单例模式在好友界面中展示在线好友列表栏，栏中显示列表内容

    Index::getInstance().getFriend()->mFriendList = friendList;
    Index::getInstance().getFriend()->showOnlineFriend(friendList);
}

void ResHandler::handleDeleteFriend(PDU *pdu)
{
    qDebug() << "ResHandler handleDeleteFriend";

    bool ret;//用于接收返回值
    memcpy(&ret, pdu->caData, sizeof(int));
    qDebug() << "handleDeleteFriend ret = " << ret;
    QString msg;
    //返回值为true：删除成功
    //返回值为false：删除失败
    if(ret == false){
        QMessageBox::information(Index::getInstance().getFriend(), "删除好友", "删除失败");
    }
    Index::getInstance().getFriend()->flushFriend();//删除后刷新好友列表
}

void ResHandler::handleChat(PDU *pdu)
{
    qDebug() << "handleChat";
    Chat* c = Index::getInstance().getFriend()->m_pChat;//单例模式打开聊天框
    if(c->isHidden()){//避免重复打开
        c->show();
    }
    char curName[32] = {'\0'};
    memcpy(curName, pdu->caData, 32);
    qDebug() << "curName:" << curName;
    c->m_strChatName = curName; //更新聊天名字变量
    c->updateShow_TE(QString("%1：%2").arg(curName).arg((char*)pdu->caMsg));//拼接发送信息的用户名
}

void ResHandler::handleMkDir(PDU *pdu)
{
    qDebug() << "handleMkDir";

    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));//接受结果
    if(ret == false){
        QMessageBox::information(Index::getInstance().getFile(), "新建文件夹", "新建文件夹失败");
    }
    Index::getInstance().getFile()->flushFile();
}

void ResHandler::handleFlushFile(PDU *pdu)
{
    int iCount = pdu->uiMsgLen / sizeof(FileInfo);//计算文件个数：pdu长度 / 单个文件属性的大小
    qDebug() << "iCount:" << iCount;
    QList<FileInfo*> pFileList; //创建列表接收文件属性
    for(int i = 0; i < iCount; i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo, pdu->caMsg+i*sizeof(FileInfo), sizeof(FileInfo));
        pFileList.append(pFileInfo);//插入文件列表
    }
    Index::getInstance().getFile()->showFileList(pFileList);//单例调用列表显示函数
}

void ResHandler::handleDelDir(PDU *pdu)
{
    qDebug() << "handleDelDir";

    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));//接受结果
    if(ret == false){   //加判断
        QMessageBox::information(Index::getInstance().getFile(), "删除文件夹", "删除文件夹失败");
    }
    Index::getInstance().getFile()->flushFile();//刷新文件列表
}

void ResHandler::handleDelFile(PDU *pdu)
{
    qDebug() << "handleDelFile";

    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));//接受结果
    if(ret == false){ //加判断
        QMessageBox::information(Index::getInstance().getFile(), "删除文件", "删除文件失败");
    }
    Index::getInstance().getFile()->flushFile();//刷新文件列表
}

void ResHandler::handleRenameFile(PDU *pdu)
{
    qDebug() << "handleRenameFile";

    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));//接受结果
    if(ret == false){
        QMessageBox::information(Index::getInstance().getFile(), "重命名文件", "重命名文件失败");
    }
    Index::getInstance().getFile()->flushFile();//刷新文件列表
}

void ResHandler::handleMoveFile(PDU *pdu)
{
    qDebug() << "handleMoveFile";

    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));//接受结果
    if(ret == false){
        QMessageBox::information(Index::getInstance().getFile(), "移动文件", "移动文件失败");
    }
    Index::getInstance().getFile()->flushFile();//刷新文件列表
}

void ResHandler::handleUploadFile(PDU *pdu)
{
    qDebug() << "handleUploadFile";
    int ret;
    memcpy(&ret, pdu->caData, sizeof (int));
    if(ret == 0){
        Index::getInstance().getFile()->uploadFileData();
    }
    else{
        QMessageBox::information(&Client::getInstance(), "上传文件", "上传文件失败");
    }
}

void ResHandler::handleUploadFileData(PDU *pdu)
{
    int ret;
    memcpy(&ret, pdu->caData, sizeof (int));
    if(ret == 0){
        Index::getInstance().getFile()->uploadFileData();
    }
    else{
        QMessageBox::information(&Client::getInstance(), "上传文件", "上传文件成功");
    }
    Index::getInstance().getFile()->flushFile();
}

void ResHandler::handleDownloadFile(PDU *pdu)
{
    qDebug() << "handleDownloadFile";
    int ret;
    memcpy(&ret, pdu->caData, sizeof (int));
    if(ret == 0){
        qint64 fileSize;
        memcpy(&fileSize, pdu->caData+32, sizeof(qint64));
        Index::getInstance().getFile()->download_file(fileSize);
    }
    else{
        QMessageBox::information(&Client::getInstance(), "下载文件", "下载文件成功");
    }
    Index::getInstance().getFile()->flushFile();
}

void ResHandler::handleDownloadFileData(PDU *pdu)
{
    qDebug() << "handleDownloadFileData";
    Index::getInstance().getFile()->downloadFileData(pdu->caMsg, pdu->uiMsgLen);
}

void ResHandler::handleShareFile()
{
    qDebug() << "handleShareFile";
    QMessageBox::information(Index::getInstance().getFile(), "分享文件", "分享成功");
}

void ResHandler::handleShareFileRequest(PDU *pdu)
{
    qDebug() << "handleShareFileRequest";
    QString strSharePath = QString(pdu->caMsg);//取出路径
    int index = strSharePath.lastIndexOf('/');
    QString strFileName = strSharePath.right(strSharePath.size()-index-1);//取出文件名（最后一个“/”之后）
    //询问是否接收文件
    int ret = QMessageBox::question(
                Index::getInstance().getFile()->m_pShareFile, "分享文件",
                QString("%1 分享文件：%2\n是否接受？").arg(pdu->caData).arg(strFileName));
    if(ret != QMessageBox::Yes) return;
    PDU* repdu = mkPDU(pdu->uiMsgLen);
    repdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST;
    memcpy(repdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);//接收方的用户名
    memcpy(repdu->caMsg, pdu->caMsg, pdu->uiMsgLen);
    Client::getInstance().sendPDU(repdu);
}

void ResHandler::handleShareFileAgree(PDU *pdu)
{
    int ret;
    memcpy(&ret, pdu->caData, sizeof (int));
    if(ret == 0){
        QMessageBox::information(&Client::getInstance(), "分享文件", "接收文件失败");
    }
    else{
        QMessageBox::information(&Client::getInstance(), "分享文件", "接收文件成功");
    }
    Index::getInstance().getFile()->flushFile();
}
