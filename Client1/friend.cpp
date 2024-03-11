#include "friend.h"
#include "ui_friend.h"
#include "protocol.h"
#include "client.h"
#include "index.h"
#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_pOnlineUser = new OnlineUser;
    m_pChat = new Chat;
    flushFriend();  //刷新函数加入构造函数，登录后自动刷新好友列表
}

Friend::~Friend()
{
    delete ui;
    delete m_pChat;
    delete m_pOnlineUser;
}

void Friend::flushFriend()//刷新好友列表函数
{
    QString userName = Client::getInstance().m_strLoginName;//获取当前客户端用户名
    PDU* pdu = mkPDU(0);        //构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_FRIEND_REQUEST;
    memcpy(pdu->caData, userName.toStdString().c_str()/*QString类型要转类型*/, 32);//存入当前客户端用户名
    qDebug() << "flushFriend";
    Client::getInstance().sendPDU(pdu);//发送
}

void Friend::showOnlineFriend(QStringList friendList)
{
    qDebug() << "showOnlineFriend";
    ui->friend_LW->clear();//展示之前先清空列表栏
    ui->friend_LW->addItems(friendList); //展示列表
}

void Friend::on_findFriend_clicked()//查找用户，点击查找用户按钮后通过这个槽函数发送请求
{
    //首先弹出一个输入框，使用QInputDialog::getText()函数，返回QString类型，用findUserName接收
    QString findUserName = QInputDialog::getText(this, "查找用户", "请输入用户名");
    if(findUserName.isEmpty()){//判空
        return;
    }
    //构建pdu，配置参数：
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FIND_USER_REQUEST;//消息类型
    memcpy(pdu->caData, findUserName.toStdString().c_str(), findUserName.size());//将findUserName转String类型存入caData中作为实际参数
    qDebug() << "on_findFriend_clicked pdu->caData:" << pdu->caData;

    Client::getInstance().sendPDU(pdu);
}

void Friend::on_onlineUser_clicked()//查找当前在线用户
{
    if(m_pOnlineUser->isHidden()){//界面展示，若为隐藏则显示
        m_pOnlineUser->show();
    }
    PDU* pdu = mkPDU(0);    //构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_REQUEST;//查找在线用户消息类型

    Client::getInstance().sendPDU(pdu);
}

void Friend::on_updataFriend_clicked()//刷新好友槽函数，点击调用刷新好友列表函数
{
    flushFriend();
}


void Friend::on_delFriend_clicked()
{
    QListWidgetItem* pItem = ui->friend_LW->currentItem();//新建列表项目变量，接收使用currentItem函数将friend_LW列表中当前选中的项目
    if(!pItem) {    //判空，若未选择项目
        QMessageBox::information(this, "删除好友", "请选择要删除的好友");
        return;
    }

    int ret = QMessageBox::question(this, "删除好友", "是否删除好友");//增加确认，避免误触
    if(ret != QMessageBox::Yes) return;
    QString tarName = pItem->text();    //获取当前项目中的文本，为目标客户端用户名
    QString curName = Client::getInstance().m_strLoginName; //获取当前客户端用户名
    PDU* pdu = mkPDU(0);    //构建pdu
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
    memcpy(pdu->caData, curName.toStdString().c_str(), 32);
    mempcpy(pdu->caData+32, tarName.toStdString().c_str(), 32);   //存入两个用户名
    qDebug() << "on_delFriend_clicked pdu->caData:" << pdu->caData;
    Client::getInstance().sendPDU(pdu);     //发送
}

void Friend::on_chat_clicked()
{
    QListWidgetItem* pItem = ui->friend_LW->currentItem();//提取好友框中选中的项目
    if(!pItem) {    //判断是否选择
        QMessageBox::information(this, "聊天", "请选择要聊天的好友");
        return;
    }
     if(m_pChat->isHidden()){   //避免重复展示聊天框
         m_pChat->show();
     }
     m_pChat->m_strChatName = pItem->text();    //提取选中的好友名，赋值给m_strChatName
}
