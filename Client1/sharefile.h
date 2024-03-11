#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>

namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();
    void updataFriend_LW();

private slots:
    void on_allSelect_PB_clicked();

    void on_cancelAllSelect_PB_clicked();

    void on_OK_PB_clicked();

private:
    Ui::ShareFile *ui;
};

#endif // SHAREFILE_H
