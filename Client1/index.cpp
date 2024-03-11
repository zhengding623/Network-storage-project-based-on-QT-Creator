#include "index.h"
#include "ui_index.h"

Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
}

Index::~Index()
{
    delete ui;
}

Friend *Index::getFriend()
{
    return ui->friendPage;
}

File *Index::getFile()
{
    return ui->filePage;
}

void Index::on_friend_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);//将好友列表优先显示
    //点击好友按钮后：
    //将文件按钮设置为透明
    ui->file_PB->setStyleSheet("QPushButton{border:none;background-color:rgba(255, 255, 255, 0);pandding:25px;}");
    //将好友按钮设置为不透明
    ui->friend_PB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);pandding:25px;}");
}

void Index::on_file_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    //点击文件按钮后：
    //将好友按钮设置为透明
    ui->friend_PB->setStyleSheet("QPushButton{border:none;background-color:rgba(255, 255, 255, 0);pandding:25px;}");
    //将文件按钮设置为不透明
    ui->file_PB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);pandding:25px;}");
    //Index::getInstance().getFile()->flushFile();
}

Index &Index::getInstance()
{
    static Index instance;
    return instance;
}
