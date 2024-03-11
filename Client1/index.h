#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include "friend.h"
#include "file.h"

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    ~Index();

    Friend* getFriend();
    File* getFile();
    static Index& getInstance();

private slots:
    void on_friend_PB_clicked();
    void on_file_PB_clicked();

private:
    Ui::Index *ui;

    explicit Index(QWidget *parent = nullptr);
    Index(const Index& instance) = delete;
    Index& operator= (const Index&) = delete;
};

#endif // INDEX_H
