#ifndef FILE_H
#define FILE_H
#include "protocol.h"
#include "sharefile.h"
#include <QWidget>
#include <QListWidgetItem>
#include <QFile>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    QString m_curPath;//客户端记录当前文件夹路径
    QString m_userRootDir;//用户根目录

    //移动文件需要的属性
    QString m_strMoveFileName;//要移动的文件名
    QString m_strMovePath;//要移动的路径

    //上传文件需要的属性
    bool m_fileUpload;//判断是否有文件上传，一次只能传一个文件
    QString m_strUploadFilePath;//上传文件的路径

    //下载文件需要的属性
    bool m_fileDownload;//判断是否有文件下载
    qint64 m_downloadTotal;//下载文件的总大小
    qint64 m_downloadReceived;//已经下载的大小
    QString m_strSavePath;//保存文件的路径
    QString m_strDownloadFileName;//下载的文件名
    QFile downloadFile;//下载的文件对象

    ShareFile* m_pShareFile;//要分享的文件对象
    QString m_strShareFileName;//要分享的文件名

    QList<FileInfo*> m_pFileInfoList;
    void flushFile();
    void showFileList(QList<FileInfo*> pFileList);
    void uploadFileData();//上传文件数据函数
    void download_file(qint64 fileSize);//下载文件函数
    void downloadFileData(char* buffer, int size);//下载文件数据函数
private slots:
    void on_mkDir_clicked();

    void on_updateFile_clicked();

    void on_rmDir_clicked();

    void on_rmFile_clicked();

    void on_rename_clicked();

    void on_file_LW_itemDoubleClicked(QListWidgetItem *item);

    void on_return_2_clicked();

    void on_mvFile_clicked();

    void on_updataFile_clicked();

    void on_downloadFile_clicked();

    void on_shareFile_clicked();

private:
    Ui::File *ui;

};

#endif // FILE_H
