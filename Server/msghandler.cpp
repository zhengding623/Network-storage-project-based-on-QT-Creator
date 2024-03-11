#include "msghandler.h"
#include "mytcpserver.h"
#include "server.h"
#include <QDir>

MsgHandler::MsgHandler(QObject *parent) : QObject(parent)
{
    m_fileUpload = false;
    m_fileDownload = false;
}

void MsgHandler::copyDir(QString strSrcPath, QString strDestDir)
{
    //创建目标目录
    QDir dir;
    dir.mkdir(strDestDir);
    //获取原目录下的文件信息列表
    dir.setPath(strSrcPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    bool ret = 1;
    QString srcTmp;
    QString destTmp;
    //遍历目录下的文件
    for(int i = 0; i <fileInfoList.size(); i++){
        if(fileInfoList[i].isFile()){
            srcTmp = strSrcDir
        }
    }
    return ret;
}

PDU *MsgHandler::handleRegist(PDU *pdu)
{
    char caName[32] = {'\0'};//创建两个变量接收从客户端发送来的用户名和密码
    char caPwd[32] = {'\0'};
    memcpy(caName, pdu->caData, 32);
    memcpy(caPwd, pdu->caData+32, 32);
    qDebug() << "recvMsg regist caName:" << caName
             << "caPwd:" << caPwd;
    bool ret = OperateDb::getInstance().handleRegist(caName, caPwd);//调用数据库控制注册的函数
    qDebug() << "tcpSocket ret = " << ret;
    if(ret){//注册成功新建一个用户名的文件夹
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getInstance().m_filePath).arg(caName));
    }
    PDU* repdu = mkPDU(0);  //创建返回客户端的pdu
    repdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;//注册返回类型
    memcpy(repdu->caData, &ret, sizeof(bool));
    return repdu;
}

PDU *MsgHandler::handleLogin(PDU *pdu,QString& userName)
{
    char caName[32] = {'\0'};//创建两个变量接收从客户端发送来的用户名和密码
    char caPwd[32] = {'\0'};
    memcpy(caName, pdu->caData, 32);
    memcpy(caPwd, pdu->caData+32, 32);
    qDebug() << "recvMsg regist caName:" << caName
             << "caPwd:" << caPwd;
    bool ret = OperateDb::getInstance().handleLogin(caName, caPwd);//调用数据库的登录函数
    qDebug() << "tcpSocket ret = " << ret;
    if(ret) userName = caName;//把登录的用户名存在服务器
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
    memcpy(repdu->caData, &ret, sizeof(bool));  //将结果返回客户端
    return repdu;
}

PDU *MsgHandler::handleFindUser(PDU *pdu)
{
    qDebug() << "handleFindUser";
    char caName[32] = {'\0'};
    memcpy(caName, pdu->caData, 32);//将客户端发送过来的pdu的caData复制给caName
    qDebug() << "recvMsg findUser caName:" << caName
             << "caPwd:" << caName;
    int ret = OperateDb::getInstance().handleFindUser(caName);//caName作为参数，单例模式调用查找用户函数
    qDebug() << "mytcpsocket recvMsg ret =" << ret;
    PDU* repdu= mkPDU(0);   //创建返回pdu，配置参数
    repdu->uiMsgType = ENUM_MSG_TYPE_FIND_USER_RESPOND;//返回类型
    memcpy(repdu->caData, caName, 32);//把名字存下来，放入caData返回给客户端
    memcpy(repdu->caData+32, &ret, sizeof(int));//结果存在caData名字之后发送给客户端
    return repdu;
}

PDU* MsgHandler::handleOnlineUser()
{
    qDebug() << "handleOnlineUser";
    QStringList result = OperateDb::getInstance().handleOnlineUser();//创建新的QStringList类型接收从数据库返回的结果
    uint uiMsgLen = result.size()*32;//长度：每个名字32位，所以总长度*32
    PDU* repdu= mkPDU(uiMsgLen);
    repdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_RESPOND;
    for(int i = 0; i < result.size(); i++){//读取result里的名字，每个名字占32位
        memcpy(repdu->caMsg+i*32/*数据偏移，每个都偏32*/, result.at(i).toStdString().c_str(), 32);
    }
    return repdu;
}

PDU *MsgHandler::handleAddFriend(PDU *pdu)
{
    char caCurName[32] = {'\0'};//创建两个变量接收pdu的两个客户端的名字
    char caTarName[32] = {'\0'};
    memcpy(caCurName, pdu->caData, 32);
    memcpy(caTarName, pdu->caData+32, 32);
    qDebug() << "recvMsg handleAddFriend caCurName:" << caCurName
             << "caTarName:" << caTarName;
    int ret = OperateDb::getInstance().handleAddFriend(caCurName, caTarName);//调用数据库的添加好友函数
    qDebug() << "handleAddFriend tcpSocket ret = " << ret;
    if(ret == 1){   //若为1，则目标用户在线，可以向其发送好友申请
        MyTcpServer::getInstance().resend(caTarName, pdu);//向目标客户端转发添加好友消息
    }
    PDU* repdu = mkPDU(0);//将服务器的响应发送给当前客户端
    repdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;//服务器对申请添加好友的当前客户端的响应
    memcpy(repdu->caData, &ret, sizeof(int));
    return repdu;
}

PDU *MsgHandler::handleAddFriendAgree(PDU *pdu)
{
    char caCurName[32] = {'\0'};//创建两个变量接收pdu的两个客户端的名字
    char caTarName[32] = {'\0'};
    memcpy(caCurName, pdu->caData, 32);
    memcpy(caTarName, pdu->caData+32, 32);
    qDebug() << "handleAddFriendAgree caCurName:" << caCurName
             << "caTarName:" << caTarName;
    OperateDb::getInstance().handleAddFriendAgree(caCurName, caTarName);//调用数据库操作关系表
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND;
    MyTcpServer::getInstance().resend(caCurName, repdu);
    return repdu;
}

PDU *MsgHandler::handleOnlineFriend(PDU *pdu)
{
    qDebug() << "handleOnlineFriend";
    char caName[32] = {'\0'};       //将pdu中的用户名取出
    memcpy(caName, pdu->caData, 32);
    qDebug() << "caName:" << caName;
    QStringList result = OperateDb::getInstance().handleOnlineFriend(caName);//创建新的QStringList类型接收从数据库返回的结果
    uint uiMsgLen = result.size()*32;//长度：每个名字32位，所以总长度*32
    PDU* repdu= mkPDU(uiMsgLen);
    repdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_FRIEND_RESPOND;
    qDebug() << "handleOnlineFriend result.size=" << result.size();
    for(int i = 0; i < result.size(); i++){//读取result里的名字，每个名字占32位
        memcpy(repdu->caMsg+i*32/*数据偏移，每个都偏32*/, result.at(i).toStdString().c_str(), 32);
    }
    return repdu;
}

PDU *MsgHandler::handleDeleteFriend(PDU *pdu)
{
    qDebug() << "handleDeleteFriend";
    char caCurName[32] = {'\0'};//创建两个变量接收pdu的两个客户端的名字
    char caTarName[32] = {'\0'};
    memcpy(caCurName, pdu->caData, 32);
    memcpy(caTarName, pdu->caData+32, 32);
    qDebug() << "handleDeleteFriend caCurName:" << caCurName
             << "caTarName:" << caTarName;
    bool ret = OperateDb::getInstance().handleDeleteFriend(caCurName, caTarName);//调用数据库的删除好友函数
    qDebug() << "ret" << ret;
    PDU* repdu = mkPDU(0);  //构建返回pdu，将结果填入caData
    repdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
    memcpy(repdu->caData, &ret, sizeof(int));
    return repdu;
}

void MsgHandler::handleChat(PDU *pdu)
{
    qDebug() << "handleChat";
    char tarName[32] = {'\0'};  //接受目标用户名
    memcpy(tarName, pdu->caData+32, 32);
    qDebug() << "tarName:" << tarName;

    MyTcpServer::getInstance().resend(tarName, pdu);  //转发函数
}

PDU *MsgHandler::handleMkDir(PDU *pdu)
{
    qDebug() << "handleMkDir";
    QString curFilePath = pdu->caMsg;//读取文件路径
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_RESPOND;
    bool ret = false;//创建文件的结果放入ret中
    QDir dir;   //通过dir操作文件
    //如果创建文件夹的所在路径不存在，返回false
    if(!dir.exists(curFilePath)){
        qDebug() << "!dir.exists(curFilePath)";
        memcpy(repdu->caData, &ret, sizeof(bool));
        return repdu;
    }
    char fileName[32]= {'\0'};
    memcpy(fileName, pdu->caData, 32);
    //拼接：新的文件路径为当前文件路径加文件名
    QString strNewPath = QString("%1/%2").arg(curFilePath).arg(fileName);
    //若拼接后的文件路径存在，或创建失败
    if(dir.exists(strNewPath) || !dir.mkdir(strNewPath)){
        qDebug() << "dir.exists(strNewPath) || !dir.mkdir(strNewPath)";
        memcpy(repdu->caData, &ret, sizeof(bool));
        return repdu;
    }
    //创建成功：
    ret = true;
    memcpy(repdu->caData, &ret, sizeof(bool));
    return repdu;
}

PDU *MsgHandler::handleFlushFile(PDU *pdu)
{
    qDebug() << "handleFlushFile";
    QString strCurPath = pdu->caMsg;//接收当前文件路径
    QDir dir(strCurPath);//新建QDir变量指向路径文件
    QFileInfoList fileInfoList = dir.entryInfoList();//调用函数直接返回目录中所有文件，返回的列表存入fileInfoList变量中
    //caMsg大小计算：所有文件的数量-2 * 一个文件的属性
    PDU* repdu = mkPDU((fileInfoList.size()-2) * sizeof(FileInfo));//-2是除去 . 和 ..；其中点指向当前目录，点点指向父目录，在最高层次的根目录中，点点和点相同。
    repdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
    FileInfo* pFileInfo = NULL;
    QString strFileName;
    for(int i = 0, j = 0; i < fileInfoList.size(); i++){
        //直接从文件名判断 点 和 点点，将这两个从列表中剔除
        if(fileInfoList[i].fileName() == QString(".") || fileInfoList[i].fileName() == QString("..")) {
            continue;
        }
        strFileName = fileInfoList[i].fileName();//接受当前遍历到的文件名
        pFileInfo = (FileInfo* )repdu->caMsg + j++;//偏移，指针与整数相加，整数与指针指向的类型相乘再相加
        memcpy(pFileInfo->fileName, strFileName.toStdString().c_str(), strFileName.size());
        if(fileInfoList[i].isDir()){//区分文件类型
            pFileInfo->fileType = 0;//文件夹
        }
        else if(fileInfoList[i].isFile()){
            pFileInfo->fileType = 1;//文件
        }
        qDebug() << "handleFlushFile strFileName:" << strFileName;
    }
    return repdu;
}

PDU *MsgHandler::handleDelDir(PDU *pdu)
{
    char* pPath = pdu->caMsg;//获取目标路径
    QFileInfo fileInfo(pPath);//路径为参数对目标文件属性进行操作
    bool ret = false;
    if(fileInfo.isDir()){//判断属性为文件夹
        QDir dir;
        dir.setPath(pPath);//将dir的对象设置为目标文件
        ret = dir.removeRecursively();//递归移除，将文件夹目录下所以文件全部移除
    }
    PDU* repdu = mkPDU(0);  //构建repdu
    repdu->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_RESPOND;
    memcpy(repdu->caData, &ret, sizeof (bool));//返回结果
    return repdu;
}

PDU *MsgHandler::handleDelFile(PDU *pdu)
{
    char* pPath = pdu->caMsg;//获取目标路径
    QFileInfo fileInfo(pPath);//路径为参数对目标文件属性进行操作
    bool ret = false;
    if(fileInfo.isFile()){//判断属性为文件
        QDir dir;
        ret = dir.remove(pPath);//直接移除
    }
    PDU* repdu = mkPDU(0);//构建repdu
    repdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_RESPOND;
    memcpy(repdu->caData, &ret, sizeof (bool));//返回结果
    return repdu;
}

PDU *MsgHandler::handleRenameFile(PDU *pdu)
{
    //char类型接收需要定义好长度否则在memcpy中会报错
    //memcpy参数类型为指针，定义好长度才可以作为指针类型的参数否则为char类型（数组类型退化为指针）
    char newFileName[32] = {'\0'};//接收新文件名
    char oldFileName[32] = {'\0'};//接收旧文件名
    memcpy(newFileName, pdu->caData, 32);
    memcpy(oldFileName, pdu->caData+32, 32);
    char* pPath = pdu->caMsg;   //接收路径
    qDebug() << "pPath:" << pPath;
    //拼接新旧文件路径
    QString newFilePath = QString("%1/%2").arg(pPath).arg(newFileName);
    QString oldFilePath = QString("%1/%2").arg(pPath).arg(oldFileName);
    qDebug() << "newFilePath:" << newFileName
             << "oldFilePath:" << oldFileName;
    QDir dir;
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
    bool ret = dir.rename(oldFilePath, newFilePath);//rename函数：填入旧的和新的文件路径
    memcpy(repdu->caData, &ret, sizeof(bool));//返回结果
    qDebug() << "handleRenameFile ret:" << ret;
    return repdu;
}

PDU *MsgHandler::handleMoveFile(PDU *pdu)
{
    int srcLen = 0;
    int tarLen = 0;
    memcpy(&srcLen, pdu->caData, sizeof(int));
    memcpy(&tarLen, pdu->caData+32, sizeof(int));

    char* pSrcPath = new char[srcLen+1];
    char* pTarPath = new char[tarLen+33];
    memset(pSrcPath, '\0', srcLen+1);
    memset(pTarPath, '\0', srcLen+33);

    memcpy(pSrcPath, pdu->caMsg, srcLen+1);
    memcpy(pTarPath, pdu->caMsg+srcLen+1, tarLen+1);

    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
    bool ret = QFile::rename(pSrcPath, pTarPath);
    memcpy(repdu->caData, &ret, sizeof(bool));

    delete [] pSrcPath;
    delete [] pTarPath;
    pSrcPath = NULL;
    pTarPath = NULL;
    return repdu;
}

PDU *MsgHandler::handleUploadFile(PDU *pdu)
{
    qDebug() << "handleUploadFile";
    int ret = 0;
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
    if(m_fileUpload){
        qDebug() << "handleUploadFile m_Upload:" << m_fileUpload;
        ret = 1;
        memcpy(repdu->caData, &ret, sizeof(bool));
        return repdu;
    }
    char caFileName[32] = {'\0'};
    qint64 fileSize = 0;
    memcpy(caFileName, pdu->caData, 32);
    memcpy(&fileSize, pdu->caData+32, sizeof(qint64));

    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
    m_UploadFile.setFileName(strPath);
    if(m_UploadFile.open(QIODevice::WriteOnly)){
        m_fileUpload = true;
        m_UploadTotal = fileSize;
        m_UploadReceived = 0;
        qDebug() << "handleUploadFile ret:" << ret;
    }
    else{
        qDebug() << "handleUploadFile ret:" << ret;
        ret = -1;
    }
    memcpy(repdu->caData, &ret, sizeof(int));
    return repdu;
}

PDU *MsgHandler::handleUploadFileData(PDU *pdu)
{
    //caMsg中的文件内容写入上传文件对象
    m_UploadFile.write(pdu->caMsg, pdu->uiMsgLen);
    //更新已接收文件大小
    m_UploadReceived += pdu->uiMsgLen;
    //没有全部接收完，不相应，继续接收
    if(m_UploadReceived < m_UploadTotal){
        return NULL;
    }
    //关闭文件，更新上传文件状态
    m_UploadFile.close();
    m_fileUpload = false;
    //响应结果
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND;
    bool ret = m_UploadReceived == m_UploadTotal;
    memcpy(repdu->caData, &ret, sizeof(bool));
    return repdu;
}

PDU *MsgHandler:: handleDownloadFile(PDU *pdu)
{
    qDebug() << "handleDownloadFile";
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
    int ret = 0;
    //判断是否有文件在下载
    if(m_fileDownload){
        ret = 1;
        memcpy(repdu->caData, &ret, sizeof (int));
        return repdu;
    }
    //设置文件属性
    QFileInfo fileInfo(pdu->caMsg);//通过caMsg中的路径读取文件属性
    qint64 fileSize = fileInfo.size();//在文件属性中得到文件大小
    downloadFile.setFileName(pdu->caMsg);//得到文件名
    if(downloadFile.open(QIODevice::ReadOnly)){//只读打开文件，变更下载状态
        m_fileDownload = true;
    }
    else{//打开失败
        ret = -1;
    }
    qDebug() << "handleDownloadFile ret" << ret;
    memcpy(repdu->caData, &ret, sizeof(int));
    memcpy(repdu->caData+32, &fileSize, sizeof(qint64));
    return repdu;
}

PDU *MsgHandler::handleDownloadFileData(MyTcpSocket *socket)
{
    qDebug() << "handleDownloadFileData";
    PDU* repdu = mkPDU(4096);
    repdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_RESPOND;
    qint64 ret = 0;
    int i = 0;
    while(1){ 
        qDebug() << "Downloading..." << ++i;
        ret = downloadFile.read(repdu->caMsg, 4096);
        if(ret <= 0) {break;}
        repdu->uiMsgLen = ret;
        repdu->uiPDULen = ret + sizeof(PDU);
        socket->write((char*)repdu, repdu->uiPDULen);//通过socket直接发送
    }
    m_fileDownload = false;
    downloadFile.close();

    free(repdu);
    repdu = NULL;
    return NULL;
}

PDU *MsgHandler::handleShareFile(PDU *pdu)
{
    qDebug() << "handleShareFile";
    //取出当前用户名和好友数量
    char strCurName[32] = {'\0'};
    int friendNum = 0;
    memcpy(strCurName, pdu->caData, 32);
    memcpy(&friendNum, pdu->caData+32, sizeof(int));

    //构建转发pdu，文件路径放入caMsg, 当前用户名放入caData
    int size = friendNum*32;//好友名称的大小
    PDU* resendpdu = mkPDU(pdu->uiMsgLen-size);//全部减去好友大小等于路径大小
    resendpdu->uiMsgType = pdu->uiMsgType;
    memcpy(resendpdu->caMsg, pdu->caMsg+size/*路径在最后需要偏移*/, pdu->uiMsgLen-size);
    memcpy(resendpdu->caData, strCurName, 32);

    //遍历好友转发
    char caRecvName[32] = {'\0'};
    for(int i = 0; i <friendNum; i++){//转发给每个好友（每个好友名不超过32位）
        memcpy(caRecvName, pdu->caMsg+i*32, 32);
        MyTcpServer::getInstance().resend(caRecvName, resendpdu);
    }
    free(resendpdu);
    resendpdu = NULL;

    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
    return repdu;
}

PDU *MsgHandler::handleShareFileAgree(PDU *pdu)
{
    //拼接接受路径：用户根目录
    QString strRecvPath = QString("%1/%2").arg(Server::getInstance().m_filePath).arg(pdu->caData);
    QString strSharePath = pdu->caMsg;
    //获取文件名，拼接接受的完整路径
    QString strShareFileName = strSharePath.right(strSharePath.size() - strSharePath.lastIndexOf('/'));
    QString strPath = strRecvPath + "/" + strShareFileName;
    //通过QFileInfo对象进行复制
    QFileInfo fileInfo(strSharePath);
    bool ret = 0;
    if(fileInfo.isFile()){
        ret = QFile::copy(strSharePath, strPath);
        qDebug() << "handleShareFileAgree is file. ret:" << ret;
    }
    else if(fileInfo.isDir()){
        ret = copyDir(strSharePath, strPath);
    }
    //构建、发送pdu
    PDU* repdu = mkPDU(0);
    repdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
    memcpy(repdu->caData, &ret, sizeof(bool));
    return repdu;

}
