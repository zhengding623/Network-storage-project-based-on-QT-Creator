#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "onlineuser.h"
#include "chat.h"

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    OnlineUser* m_pOnlineUser;
    Chat* m_pChat;
    QStringList mFriendList;
    void flushFriend();
    void showOnlineFriend(QStringList friendList);


private slots:
    void on_findFriend_clicked();

    void on_onlineUser_clicked();

    void on_updataFriend_clicked();


    void on_delFriend_clicked();

    void on_chat_clicked();

private:
    Ui::Friend *ui;
};

#endif // FRIEND_H
