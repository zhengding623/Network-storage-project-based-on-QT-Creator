#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>

class OperateDb : public QObject
{
    Q_OBJECT
private:
    explicit OperateDb(QObject *parent = nullptr);
    //单例模式
    OperateDb(const OperateDb& instance) = delete;
    OperateDb& operator= (const OperateDb&) = delete;
    QSqlDatabase m_db;//数据库对象
public:
    ~OperateDb();
    static OperateDb& getInstance();
    void connectSQL();//连接数据库
    bool handleRegist(const char* name, const char* pwd);//注册，数据库添加行
    bool handleLogin(const char* name, const char* pwd);//登录，数据库查询是否存在，将online字段设为1
    void handleOffline(const char* name);//离线，将对应的online字段设为0
    int handleFindUser(const char* name);//查找用户，数据库查询对应的name字段
    QStringList handleOnlineUser();//在线用户，查询online字段为1的用户
    int handleAddFriend(const char* curName, const char* tarName);//添加好友，查询是否满足添加条件
    void handleAddFriendAgree(const char* curName, const char* tarName);//同意添加好友，在friend关系表中添加对应的用户名
    QStringList handleOnlineFriend(const char* name);//刷新好友，返回一个列表
    bool handleDeleteFriend(const char* curName, const char* tarName);//删除好友函数
signals:

};

#endif // OPERATEDB_H
