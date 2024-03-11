/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *file_LW;
    QGridLayout *gridLayout;
    QPushButton *mkDir;
    QPushButton *shareFile;
    QPushButton *rmDir;
    QPushButton *downloadFile;
    QPushButton *return_2;
    QPushButton *rmFile;
    QPushButton *rename;
    QPushButton *mvFile;
    QPushButton *updataFile;
    QPushButton *updateFile;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(400, 300);
        horizontalLayout = new QHBoxLayout(File);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        file_LW = new QListWidget(File);
        file_LW->setObjectName(QString::fromUtf8("file_LW"));
        file_LW->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(file_LW);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mkDir = new QPushButton(File);
        mkDir->setObjectName(QString::fromUtf8("mkDir"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mkDir->sizePolicy().hasHeightForWidth());
        mkDir->setSizePolicy(sizePolicy);
        mkDir->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        mkDir->setFont(font);

        gridLayout->addWidget(mkDir, 1, 0, 1, 1);

        shareFile = new QPushButton(File);
        shareFile->setObjectName(QString::fromUtf8("shareFile"));
        sizePolicy.setHeightForWidth(shareFile->sizePolicy().hasHeightForWidth());
        shareFile->setSizePolicy(sizePolicy);
        shareFile->setMinimumSize(QSize(0, 50));
        shareFile->setFont(font);

        gridLayout->addWidget(shareFile, 2, 1, 1, 1);

        rmDir = new QPushButton(File);
        rmDir->setObjectName(QString::fromUtf8("rmDir"));
        sizePolicy.setHeightForWidth(rmDir->sizePolicy().hasHeightForWidth());
        rmDir->setSizePolicy(sizePolicy);
        rmDir->setMinimumSize(QSize(0, 50));
        rmDir->setFont(font);

        gridLayout->addWidget(rmDir, 2, 0, 1, 1);

        downloadFile = new QPushButton(File);
        downloadFile->setObjectName(QString::fromUtf8("downloadFile"));
        sizePolicy.setHeightForWidth(downloadFile->sizePolicy().hasHeightForWidth());
        downloadFile->setSizePolicy(sizePolicy);
        downloadFile->setMinimumSize(QSize(0, 50));
        downloadFile->setFont(font);

        gridLayout->addWidget(downloadFile, 3, 1, 1, 1);

        return_2 = new QPushButton(File);
        return_2->setObjectName(QString::fromUtf8("return_2"));
        sizePolicy.setHeightForWidth(return_2->sizePolicy().hasHeightForWidth());
        return_2->setSizePolicy(sizePolicy);
        return_2->setMinimumSize(QSize(0, 50));
        return_2->setFont(font);

        gridLayout->addWidget(return_2, 0, 1, 1, 1);

        rmFile = new QPushButton(File);
        rmFile->setObjectName(QString::fromUtf8("rmFile"));
        sizePolicy.setHeightForWidth(rmFile->sizePolicy().hasHeightForWidth());
        rmFile->setSizePolicy(sizePolicy);
        rmFile->setMinimumSize(QSize(0, 50));
        rmFile->setFont(font);

        gridLayout->addWidget(rmFile, 3, 0, 1, 1);

        rename = new QPushButton(File);
        rename->setObjectName(QString::fromUtf8("rename"));
        sizePolicy.setHeightForWidth(rename->sizePolicy().hasHeightForWidth());
        rename->setSizePolicy(sizePolicy);
        rename->setMinimumSize(QSize(0, 50));
        rename->setFont(font);

        gridLayout->addWidget(rename, 1, 1, 1, 1);

        mvFile = new QPushButton(File);
        mvFile->setObjectName(QString::fromUtf8("mvFile"));
        sizePolicy.setHeightForWidth(mvFile->sizePolicy().hasHeightForWidth());
        mvFile->setSizePolicy(sizePolicy);
        mvFile->setMinimumSize(QSize(0, 50));
        mvFile->setFont(font);

        gridLayout->addWidget(mvFile, 4, 0, 1, 1);

        updataFile = new QPushButton(File);
        updataFile->setObjectName(QString::fromUtf8("updataFile"));
        sizePolicy.setHeightForWidth(updataFile->sizePolicy().hasHeightForWidth());
        updataFile->setSizePolicy(sizePolicy);
        updataFile->setMinimumSize(QSize(0, 50));
        updataFile->setFont(font);

        gridLayout->addWidget(updataFile, 4, 1, 1, 1);

        updateFile = new QPushButton(File);
        updateFile->setObjectName(QString::fromUtf8("updateFile"));
        sizePolicy.setHeightForWidth(updateFile->sizePolicy().hasHeightForWidth());
        updateFile->setSizePolicy(sizePolicy);
        updateFile->setMinimumSize(QSize(0, 50));
        updateFile->setFont(font);

        gridLayout->addWidget(updateFile, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        mkDir->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        shareFile->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        rmDir->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        downloadFile->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        return_2->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        rmFile->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        rename->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        mvFile->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        updataFile->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        updateFile->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
