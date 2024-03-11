#include "server.h"
#include "mytcpserver.h"
#include "operatedb.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    OperateDb::getInstance().connectSQL();
    Server w;
    w.show();

    return a.exec();
}
