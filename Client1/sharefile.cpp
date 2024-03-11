#include "sharefile.h"
#include "ui_sharefile.h"
#include "index.h"
#include "client.h"


ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updataFriend_LW()
{
    ui->friend_LW->clear();
    ui->friend_LW->addItems(Index::getInstance().getFriend()->mFriendList);//mFriendList是Friend定义的变量，在刷新好友的响应里更新
}

void ShareFile::on_allSelect_PB_clicked()
{
    for(int i = 0; i < ui->friend_LW->count(); i++){
        ui->friend_LW->item(i)->setSelected(1);
    }
}

void ShareFile::on_cancelAllSelect_PB_clicked()
{
    for(int i = 0; i < ui->friend_LW->count(); i++){
        ui->friend_LW->item(i)->setSelected(0);
    }
}

void ShareFile::on_OK_PB_clicked()
{
    //拼接分享文件的完整路径
     QString strCurPath = Index::getInstance().getFile()->m_curPath;
     QString strShareFileName = Index::getInstance().getFile()->m_strShareFileName;
     QString strPath = strCurPath + "/" + strShareFileName;
     //获取选择的好友和好友数量
     QList<QListWidgetItem*> pItems = ui->friend_LW->selectedItems();
     int friendNums = pItems.size();
     //构建PDU
     PDU* pdu = mkPDU(friendNums*32 + strPath.size() +1);//偏移量：每个用户名不超过32位
     pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
     //当前用户名和好友数量放入caData
     memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
     memcpy(pdu->caData+32, &friendNums, sizeof(int));
     //选择的好友名和完整路径放入caMsg
     for(int i = 0; i < friendNums; i++){
         memcpy(pdu->caMsg+32*i, pItems.at(i)->text().toStdString().c_str(), 32);
     }
     //文件完整路径
     memcpy(pdu->caMsg+32*friendNums, strPath.toStdString().c_str(), strPath.size());

     Client::getInstance().sendPDU(pdu);//发送PDU

}
