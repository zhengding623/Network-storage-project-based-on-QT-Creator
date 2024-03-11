#include "chat.h"
#include "ui_chat.h"
#include "client.h"
#include "protocol.h"


Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::updateShow_TE(QString strMsg)
{
    ui->chat_TE->append(strMsg);
}

void Chat::on_send_PB_clicked()
{
    QString strChat = ui->input_LE->text(); //提取输入框中的内容
    if(strChat.isEmpty()) return;   //判断是否输入内容
    QString tarName = m_strChatName;    //m_strChatName为需要发送消息的用户名
    QString curName = Client::getInstance().m_strLoginName;
    PDU* pdu = mkPDU(strChat.size()+1); //构建pdu，+1给换行符预留位置
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData, curName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, tarName.toStdString().c_str(), 32);//放入双方用户名
    memcpy(pdu->caMsg, strChat.toStdString().c_str(), strChat.size());//放入消息
    Client::getInstance().sendPDU(pdu);
    ui->input_LE->clear();//清空输入框
}
