#include "file.h"
#include "ui_file.h"
#include "client.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    qDebug() << "File";
    ui->setupUi(this);
    m_curPath = QString("./filesys/%1").arg(Client::getInstance().m_strLoginName);
    m_userRootDir = m_curPath;
    m_pShareFile = new ShareFile;
    m_fileDownload = false;
    m_fileUpload = false;
    flushFile();
}

File::~File()
{
    delete ui;
}

void File::flushFile()//刷新文件列表函数
{
    qDebug() << "flushFile m_curPath " <<m_curPath;
    //将当前文件路径发送给服务器
    PDU* pdu = mkPDU(m_curPath.size()+1);   //给字符串分配空间需要计算结尾\0 所以加一
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg, m_curPath.toStdString().c_str(), m_curPath.size());
    Client::getInstance().sendPDU(pdu);
    //半包测试
//    PDU* pdu = mkPDU(m_curPath.size()+1);   //给字符串分配空间需要计算结尾\0所以加一
//    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
//    memcpy(pdu->caMsg, m_curPath.toStdString().c_str(), m_curPath.size());
//    Client::getInstance().m_tcpSocket.write((char*)pdu, sizeof(PDU));
//    QTimer::singleShot(1000, [&] {
//        Client::getInstance().m_tcpSocket.write(m_curPath.toStdString().c_str(), m_curPath.size()+1);
//    });
//    free(pdu);
}

void File::showFileList(QList<FileInfo *> pFileList)//展示文件列表
{
    //清空先有的文件列表
    foreach(FileInfo* pFileInfo, m_pFileInfoList){//遍历删除
        delete pFileInfo;
    }
    m_pFileInfoList.clear();
    m_pFileInfoList = pFileList;//赋空

    ui->file_LW->clear();//清空列表框
    foreach(FileInfo* pFileInfo, pFileList){//遍历列表框
        QListWidgetItem* pItem = new QListWidgetItem;   //新建一个项目
        if(pFileInfo->fileType == 0){   //判断文件类型使用不同图标，0为文件夹，1为文件
            pItem->setIcon(QIcon(QPixmap(":/dir.png")));
        }
        else if(pFileInfo->fileType == 1){
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        pItem->setText(pFileInfo->fileName);//按顺序将文件名称加在项目的文本上
        qDebug() << "pFileInfo->fileName:" << pFileInfo->fileName;
        ui->file_LW->addItem(pItem);//在列表小部件的末尾插入一个带有文本标签的项目。
    }
}

void File::uploadFileData()
{
    QFile file(m_strUploadFilePath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "上传文件", "打开文件失败");
        return;
    }
    m_fileUpload = true;
    qint64 ret = 0;
    PDU* datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST;
    while(true){
        ret = file.read(datapdu->caMsg, 4096);
        if(ret == 0) break;
        if(ret < 0){
            QMessageBox::warning(this, "上传文件", "读取文件失败");
            break;
        }
        datapdu->uiMsgLen = ret;
        datapdu->uiPDULen = ret + sizeof (PDU);
        //发送pdu：不能直接调用发送函数，否则会直接释放，需要等待发送完成手动释放
        Client::getInstance().m_tcpSocket.write((char*)datapdu, datapdu->uiPDULen);
    }
    m_fileUpload = false;//更新上传文件的状态：未上传文件
    file.close();
    free(datapdu);//这里再释放
    datapdu = NULL;
}

void File::download_file(qint64 fileSize)
{
    qDebug() << "download_file";
    m_downloadReceived = 0;
    m_downloadTotal = fileSize;
    m_fileDownload = true;
    downloadFile.setFileName(m_strSavePath);
    if(!downloadFile.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "下载文件", "打开文件失败");
        return;
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST;
    Client::getInstance().sendPDU(pdu);
}

void File::downloadFileData(char *buffer, int size)
{
    downloadFile.write(buffer, size);
    m_downloadReceived += size;
    if(m_downloadReceived < m_downloadTotal){
        return;
    }
    downloadFile.close();
    m_fileDownload = false;
    QMessageBox::information(this, "下载文件", "下载完成");
}

void File::on_mkDir_clicked()//新建文件夹槽函数
{
    QString strFileName = QInputDialog::getText(this, "新建文件夹", "请输入文件名");//弹出输入框获取新建的文件名
    if(strFileName.isEmpty()){//判断是否输入
        QMessageBox::information(this, "新建文件夹","文件名不能为空");
    }
    if(strFileName.size() > 32){//名称不能过长
        QMessageBox::information(this, "新建文件夹", "文件名过长");
    }
    PDU* pdu = mkPDU(m_curPath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData, strFileName.toStdString().c_str(), 32); //将文件名放入caData中，过长就放不下了
    memcpy(pdu->caMsg, m_curPath.toStdString().c_str(), m_curPath.size());  //将当前的文件路径放入caMsg

    Client::getInstance().sendPDU(pdu);
}

void File::on_updateFile_clicked()
{
    flushFile();
}

void File::on_rmDir_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();//新建列表项目变量，接收使用currentItem函数将file_LW列表中当前选中的项目
    if(!pItem) {    //判空，若未选择项目
        QMessageBox::warning(this, "删除文件夹", "请选择要删除的文件夹");
        return;
    }
    QString curFileName = pItem->text();//获取需要删除的文件夹的文件名
    foreach(FileInfo* pFileInfo, m_pFileInfoList){//遍历文件列表，判断选择的对象是否为文件夹
        if(pFileInfo->fileName == curFileName && pFileInfo->fileType != 0){
            QMessageBox::warning(this, "删除文件夹", "请选择正确的文件夹");
            return;
        }
    }
    int ret = QMessageBox::question(this, "删除文件夹", "是否删除文件夹");//增加确认，避免误触
    if(ret != QMessageBox::Yes) return;
    QString strDelPath = QString("%1/%2").arg(m_curPath).arg(curFileName);
    PDU* pdu = mkPDU(strDelPath.size());    //构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_REQUEST;
    memcpy(pdu->caMsg, strDelPath.toStdString().c_str(), strDelPath.size()); //将当前选择的文件路径发送给服务器
    qDebug() << "on_rmDir_clicked pdu->caMsg:" << pdu->caMsg;
    Client::getInstance().sendPDU(pdu);     //发送
}

void File::on_rmFile_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();//新建列表项目变量，接收使用currentItem函数将file_LW列表中当前选中的项目
    if(!pItem) {    //判空，若未选择项目
        QMessageBox::warning(this, "删除文件", "请选择要删除的文件");
        return;
    }
    QString curFileName = pItem->text();//获取需要删除的文件的文件名
    foreach(FileInfo* pFileInfo, m_pFileInfoList){//遍历文件列表，判断选择的对象是否为文件
        if(pFileInfo->fileName == curFileName && pFileInfo->fileType != 1){
            QMessageBox::warning(this, "删除文件", "请选择正确的文件");
            return;
        }
    }
    int ret = QMessageBox::question(this, "删除文件", "是否删除文件");//增加确认，避免误触
    if(ret != QMessageBox::Yes) return;
    QString strDelPath = QString("%1/%2").arg(m_curPath).arg(curFileName);
    PDU* pdu = mkPDU(strDelPath.size());    //构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
    memcpy(pdu->caMsg, strDelPath.toStdString().c_str(), strDelPath.size());//将当前选择的文件路径发送给服务器
    qDebug() << "on_rmFile_clicked pdu->caMsg:" << pdu->caMsg;
    Client::getInstance().sendPDU(pdu);     //发送
}

void File::on_rename_clicked()
{
    qDebug() << "on_rename_clicked";
    //优先判断用户是否选择了文件
    QListWidgetItem* pItem = ui->file_LW->currentItem();//新建列表项目变量，接收使用currentItem函数将file_LW列表中当前选中的项目
    if(pItem == NULL) {
        QMessageBox::warning(this, "重命名文件" ,"请选择一个文件");
        return;
    }
    QString oldFileName = pItem->text();    //获取当前选中的项目中的文本
    QString newFileName = QInputDialog::getText(this, "重命名", "请输入新的文件名");//弹出输入框获取新的文件名
    if(newFileName.isEmpty())//判空
    {
        QMessageBox::warning(this, "重命名文件", "请输入文件名");
        return;
    }
    if(newFileName.size() > 32) {//用户名过长
        QMessageBox::warning(this, "重命名文件", "文件名过长");
        return;
    }
    PDU* pdu = mkPDU(m_curPath.size()+1);//加一 给字符串分配空间需要计算结尾\0
    pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
    memcpy(pdu->caData, newFileName.toStdString().c_str(), 32);//填入新的文件名32位
    memcpy(pdu->caData+32, oldFileName.toStdString().c_str(), 32);//填入旧的文件名32位
    memcpy(pdu->caMsg, m_curPath.toStdString().c_str(), m_curPath.size());//填入当前文件路径

    qDebug() << "on_rename_clicked pdu->caMsg:" << pdu->caMsg;
    Client::getInstance().sendPDU(pdu);     //发送
}

void File::on_file_LW_itemDoubleClicked(QListWidgetItem *item)
{
    //获取选中的对象的文本
    QString strDirName = item->text();
    //判断是否为文件夹:通过名字
    foreach(FileInfo* pFileInfo, m_pFileInfoList){
        if(strDirName == pFileInfo->fileName && pFileInfo->fileType != 0){
            QMessageBox::warning(this, "警告", "请选择一个文件夹");
            return;
        }
    }
    //更新当前路径
    m_curPath = QString("%1/%2").arg(m_curPath).arg(strDirName);
    //刷新
    flushFile();
}

void File::on_return_2_clicked()
{
    if(m_curPath == m_userRootDir){
        QMessageBox::warning(this, "警告", "已经是根目录");
        return;
    }
    int index = m_curPath.lastIndexOf("/");
    qDebug() << "index = " << index;
    m_curPath.remove(index, m_curPath.size() - index);
    qDebug() << "m_curPath:" << m_curPath;
    flushFile();
}

void File::on_mvFile_clicked()
{
    //“移动文件”操作
    if(ui->mvFile->text() == "移动文件"){
        //获取要移动的文件
        QListWidgetItem* pItem = ui->file_LW->currentItem();
        if(pItem == NULL){
            QMessageBox::warning(this, "移动文件", "请选择要移动的文件");
            return;
        }
        //提示选中要移动的目录
        QMessageBox::information(this, "移动文件", "请选择要移动到的目录");
        //要移动的文件名和路径作为成员变量存下来
        m_strMoveFileName = pItem->text();
        m_strMovePath = QString("%1/%2").arg(m_curPath).arg(m_strMoveFileName);
        //按钮文字改为：“确认/取消”
        ui->mvFile->setText("确认/取消");
        return;
    }
    //“确认/取消”操作
    //判断用户是否选中目录
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    QString tarPath;
    QString boxMsg;
    if(pItem == NULL){
        //没有选中目录，移动到当前路径下
        tarPath = QString("%1/%2").arg(m_curPath).arg(m_strMoveFileName);
        boxMsg = "是否移动到当前目录下";
    }
    else{
        //选中目录移动到选中的目录下，判断选中的是否为目录
        foreach(FileInfo* pFileInfo, m_pFileInfoList){
            if(pItem->text() == pFileInfo->fileName && pFileInfo->fileType != 0){
                QMessageBox::warning(this, "警告", "选中的不是文件夹");
                return;
            }
        }
        tarPath = QString("%1/%2/%3").arg(m_curPath).arg(pItem->text()).arg(m_strMoveFileName);
        boxMsg = "是否移动到选中的目录下";
    }
    //弹窗提醒用户确认或取消移动
    int ret = QMessageBox::information(this, "移动文件", boxMsg, "确认", "取消");
    //按钮上的字改为移动文件
    ui->mvFile->setText("移动文件");
    if(ret != 0){
        return;
    }

    //构建并发送pdu，路径长度放入caData, 原路径和目标路径放入caMsg
    int srcLen = m_strMovePath.size();
    int tarLen = tarPath.size();
    PDU* pdu = mkPDU(srcLen + tarLen + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    memcpy(pdu->caData, &srcLen, sizeof(int));
    memcpy(pdu->caData+32, &tarLen, sizeof(int));
    memcpy(pdu->caMsg, m_strMovePath.toStdString().c_str(), srcLen);
    memcpy(pdu->caMsg+srcLen+1, tarPath.toStdString().c_str(), tarLen);
    Client::getInstance().sendPDU(pdu);
    flushFile();
}

void File::on_updataFile_clicked()
{
    qDebug() << "on_updataFile_clicked";
    //判断上传文件状态
    if(m_fileUpload){
        qDebug() << "m_fileUpload" << m_fileUpload;
        QMessageBox::warning(this, "上传文件", "已有文件上传，请等待完成");
        return;
    }
    //通过文件弹窗获取要上传的文件
    m_strUploadFilePath.clear();
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    if(m_strUploadFilePath == ""){
        return;
    }
    int index = m_strUploadFilePath.lastIndexOf('/');
    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size()-index-1);//获取文件名
    if(strFileName.size() > 32){
        QMessageBox::warning(this, "上传文件", "文件名过长");
        return;
    }
    //构建pdu，文件名和大小放入caData，当前路径放入caMsg
    PDU* pdu = mkPDU(m_curPath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;

    QFile file(m_strUploadFilePath);
    qint64 fileSize = file.size();//获取文件大小
    memcpy(pdu->caData, strFileName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, &fileSize, sizeof(qint64));
    memcpy(pdu->caMsg, m_curPath.toStdString().c_str(), m_curPath.size());

    Client::getInstance().sendPDU(pdu);
}

void File::on_downloadFile_clicked()
{
    qDebug() << "on_downloadFile_clicked";
    //判断是否正在下载
    if(m_fileDownload){
        QMessageBox::warning(this, "下载文件", "已有正在下载的文件");
        return;
    }
    //选择要下载的文件
    QListWidgetItem* pItem =  ui->file_LW->currentItem();
    //判断是否选择文件
    if(pItem == NULL){
        QMessageBox::warning(this, "下载文件", "请选择要下载的文件");
        return;
    }
    //弹窗选择保存路径
    m_strSavePath = QFileDialog::getSaveFileName();//要下载的文件保存在本地的路径
    //判断是否选择要保存的路径，未选择直接返回
    if(m_strSavePath == ""){
        QMessageBox::information(this, "下载文件", "未选择下载路径");
        return;
    }
    qDebug() << "m_strSavePath" << m_strSavePath;

    m_strDownloadFileName = pItem->text();//将要下载的文件名保存下来

    QString strPath = m_curPath + "/" + m_strDownloadFileName;//要下载的文件在服务器的路径
    //构建pdu，将完整路径放入caMsg
    PDU* pdu = mkPDU(strPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->caMsg, strPath.toStdString().c_str(), strPath.size());
    Client::getInstance().sendPDU(pdu);
}

void File::on_shareFile_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this, "分享文件", "请选择要分享的文件");
        return;
    }
    m_strShareFileName = pItem->text();
    m_pShareFile->updataFriend_LW();
    if(m_pShareFile->isHidden()){
        m_pShareFile->show();
    }
}
