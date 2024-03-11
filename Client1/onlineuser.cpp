#include "onlineuser.h"
#include "ui_onlineuser.h"
#include "protocol.h"
#include "client.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::showOnlineUser(QStringList userList)
{
    ui->onlineUser_LW->clear();//展示之前先清空列表栏
    ui->onlineUser_LW->addItems(userList); //展示列表
}

void OnlineUser::on_onlineUser_LW_itemDoubleClicked(QListWidgetItem *item)//双击槽函数添加好友
{
    //获取当前用户名
    QString curName = Client::getInstance().m_strLoginName;
    //获取目标用户名
    QString tarName = item->text();
    PDU* pdu = mkPDU(0);//构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;//添加好友申请
    memcpy(pdu->caData, curName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, tarName.toStdString().c_str(), 32);    
    Client::getInstance().sendPDU(pdu);
}
