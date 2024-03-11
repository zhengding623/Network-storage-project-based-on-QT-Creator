#include "operatedb.h"
#include "protocol.h"
#include "mytcpsocket.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>


OperateDb::OperateDb(QObject *parent) : QObject(parent)
{
        m_db = QSqlDatabase::addDatabase("QMYSQL");
}

OperateDb::~OperateDb()
{
    m_db.close();
}

OperateDb &OperateDb::getInstance()
{
    static OperateDb instance;
    return instance;
}

void OperateDb::connectSQL()    //连接数据库
{
    m_db.setHostName("localhost");  //主机名称
    m_db.setDatabaseName("luan");   //数据库名称
    m_db.setPort(3306);             //端口号
    m_db.setUserName("LUAN");       //用户名
    m_db.setPassword("123456");     //密码
    if(m_db.open()){
        qDebug() << "数据库连接成功";
    }
    else{
        QMessageBox::critical(0, "Database Error", m_db.lastError().text());
    }
}

bool OperateDb::handleRegist(const char *name, const char *pwd) //注册
{
    if(name == NULL || pwd == NULL){//用户名和密码判空
        return false;
    }
    //判断用户是否存在
    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    QSqlQuery q;
    if(!q.exec(sql) || q.next()){ //next有返回值说明用户存在 或运行失败都返回false
        return false;
    }

    //若不存在则添加用户
    sql = QString("insert into user_info(name, pwd) value('%1' ,'%2')").arg(name).arg(pwd);
    return q.exec(sql);
}

bool OperateDb::handleLogin(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL){
        return false;
    }
    //判断用户名和密码
    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    QSqlQuery q;
    if(!q.exec(sql) || !q.next()){ //next有返回值说明用户存在
        return false;
    }

    //将online置为1即可登录成功
    sql = QString("update user_info set online = 1 where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    qDebug() << "handleLogin insert user sql:" << sql;
    return q.exec(sql);
}

void OperateDb::handleOffline(const char *name)//用户离线函数
{
    qDebug() << "handleOffline";
    if(name == NULL){//名字判空
        qDebug() << "handleOffline name is NULL";
        return;
    }
    //SQL语句：将指定的name字段的online字段置为0
    QString sql = QString("update user_info set online=0 where name = '%1'").arg(name);
    qDebug() << "handleLogin update user online sql:" << sql;
    QSqlQuery q;//执行
    q.exec(sql);
}

int OperateDb::handleFindUser(const char *name)
{
    //与数据库连接，返回值为-1：用户不存在
    //            返回值为1：用户在线
    //            返回值为0：用户离线
    if(name == NULL){//对数据库进行操作，先判空
        return -1;
    }
    //对数据库操作使用SQL语句，赋值给sql
    QString sql = QString("select online from user_info where name = '%1'").arg(name);//在user_info表中查找name字段和online字段
    qDebug() << "handleFindUser select user online sql = " << sql;
    QSqlQuery q;//执行SQL语句的变量
    q.exec(sql);//执行
    //q.next()会执行SQL语句，想要看他的返回值需要创建一个新的变量来接收他，否则就会运行下去
    //如下行代码运行后会使if语句里的q.next()没有返回值（因为运行完了）
    //qDebug() << "handleFindUser q.next():" << q.next();
    if(q.next()){
        //若执行后有返回值（用户存在）返回online字段的结果
        return q.value(0).toInt();
    }
    //否则返回-1
    return -1;
}

QStringList OperateDb::handleOnlineUser()//查找在线用户
{
    //SQL语句，查找在user_info表中online字段为1的用户name
    QString sql = QString("select name from user_info where online=1");
    qDebug() << "handleFindUser select user online sql = " << sql;
    QStringList result;//返回的结果用列表接收
    QSqlQuery q;
    q.exec(sql);//执行，遍历列表，存入result中，返回result
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}

int OperateDb::handleAddFriend(const char *curName, const char *tarName)
{
    if(curName == NULL || tarName == NULL){//判空
        return -1;//发生错误
    }
    /*SQL语句：子查询，先在user_info表中查询当前用户名和目标用户名
      然后再friend表中user_id和friend_id分别查询（互为好友只记录一条）
      最后将两种情况做或处理*/
    QString sql = QString(R"(
                          select * from friend where (
                            user_id = (select id from user_info where name = '%1')
                            and
                            friend_id = (select id from user_info where name = '%2')
                          )
                          or
                          (
                            friend_id = (select id from user_info where name = '%3')
                            and
                            user_id = (select id from user_info where name = '%4')
                          );
                          )").arg(curName).arg(tarName).arg(curName).arg(tarName);
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -2;//已经是好友
    }
    sql = QString("select online from user_info where name='%1'").arg(tarName);//判断是否在线
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();//0离线，1在线
    }
    return -1;
}

void OperateDb::handleAddFriendAgree(const char *curName, const char *tarName)
{
    if(curName == NULL || tarName == NULL){//判空
        return;
    }
    /*SQL语句：查询user_info中两个用户的id
     *        在关系表中分别添加两个用户的id为user_id和friend_id*/
    QString sql = QString(R"(
                          insert into friend(user_id, friend_id)
                          select u1.id, u2.id
                          from user_info u1, user_info u2
                          where u1.name='%1' and u2.name='%2';
                          )").arg(curName).arg(tarName);
    QSqlQuery q;
    q.exec(sql);
}

QStringList OperateDb::handleOnlineFriend(const char* name)//在线好友列表
{
    QStringList result;//返回的结果用列表接收
    result.clear();
    if(name == NULL) {
        return result;
    }
    //SQL语句:查询是否为好友，查询name的id后用于在friend关系表中查询是否为好友，其ID可能在user_id字段中，也可能在friend_id字段中
    //       然后再回到user_info表中获取到对应id的name，最后判断其是否在线。
    QString sql = QString(R"(
                          select name from user_info where id in(
                            select user_id from friend where friend_id=(select id from user_info where name='%1')
                            union
                            select friend_id from friend where user_id=(select id from user_info where name='%1')
                          )and online=1;
                        )").arg(name);
    qDebug() << "OperateDb handleOnlineFriend";
    QSqlQuery q;
    q.exec(sql);//执行，遍历列表，存入result中，返回result
    while(q.next()){
        result.append(q.value(0).toString());
    }
    qDebug() << "OperateDb handleOnlineFriend result=" << result.size();
    return result;
}

bool OperateDb::handleDeleteFriend(const char *curName, const char *tarName)
{
    if(curName == NULL || tarName == NULL){//判空
        return false;//错误
    }
    //SQL语句：先写出where条件（同添加好友的查询），然后先进行select查找，避免出错。
    //若找到则再进行删除。
    QString friendWhere = QString(R"(
                          where (
                            user_id = (select id from user_info where name = '%1')
                            and
                            friend_id = (select id from user_info where name = '%2')
                          )
                          or
                          (
                            friend_id = (select id from user_info where name = '%3')
                            and
                            user_id = (select id from user_info where name = '%4')
                          );
                          )").arg(curName).arg(tarName).arg(curName).arg(tarName);
    QString sql = QString("select * from friend %1").arg(friendWhere);//优先查找，注意拼接格式
    QSqlQuery q;
    q.exec(sql);
    if(!q.next()){  //执行后判断是否有返回值，若没有则不是好友，不能删除，返回失败
       return false;
    }
    sql = QString("delete from friend %1").arg(friendWhere);    //删除好友，注意拼接格式
    q.exec(sql);
    return true;
}




