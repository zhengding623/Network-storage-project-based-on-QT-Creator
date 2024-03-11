#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    QString m_filePath;
    void loadConfig();
    static Server &getInstance();

private:
    QString m_strIP;
    quint16 m_usPort;
    Server(const Server& instance) = delete;
    Server& operator= (const Server&) = delete;
    Ui::Server *ui;

};

#endif // SERVER_H
