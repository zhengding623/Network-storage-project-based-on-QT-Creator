/********************************************************************************
** Form generated from reading UI file 'friend.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIEND_H
#define UI_FRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Friend
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *friend_LW;
    QVBoxLayout *verticalLayout;
    QPushButton *chat;
    QPushButton *updataFriend;
    QPushButton *delFriend;
    QSpacerItem *verticalSpacer;
    QPushButton *findFriend;
    QPushButton *onlineUser;

    void setupUi(QWidget *Friend)
    {
        if (Friend->objectName().isEmpty())
            Friend->setObjectName(QString::fromUtf8("Friend"));
        Friend->resize(400, 300);
        horizontalLayout = new QHBoxLayout(Friend);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        friend_LW = new QListWidget(Friend);
        friend_LW->setObjectName(QString::fromUtf8("friend_LW"));
        friend_LW->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(friend_LW);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        chat = new QPushButton(Friend);
        chat->setObjectName(QString::fromUtf8("chat"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(16);
        chat->setFont(font);

        verticalLayout->addWidget(chat);

        updataFriend = new QPushButton(Friend);
        updataFriend->setObjectName(QString::fromUtf8("updataFriend"));
        updataFriend->setFont(font);

        verticalLayout->addWidget(updataFriend);

        delFriend = new QPushButton(Friend);
        delFriend->setObjectName(QString::fromUtf8("delFriend"));
        delFriend->setFont(font);

        verticalLayout->addWidget(delFriend);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        findFriend = new QPushButton(Friend);
        findFriend->setObjectName(QString::fromUtf8("findFriend"));
        findFriend->setFont(font);

        verticalLayout->addWidget(findFriend);

        onlineUser = new QPushButton(Friend);
        onlineUser->setObjectName(QString::fromUtf8("onlineUser"));
        onlineUser->setFont(font);

        verticalLayout->addWidget(onlineUser);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Friend);

        QMetaObject::connectSlotsByName(Friend);
    } // setupUi

    void retranslateUi(QWidget *Friend)
    {
        Friend->setWindowTitle(QCoreApplication::translate("Friend", "Form", nullptr));
        chat->setText(QCoreApplication::translate("Friend", "\350\201\212\345\244\251", nullptr));
        updataFriend->setText(QCoreApplication::translate("Friend", "\345\210\267\346\226\260", nullptr));
        delFriend->setText(QCoreApplication::translate("Friend", "\345\210\240\351\231\244\345\245\275\345\217\213", nullptr));
        findFriend->setText(QCoreApplication::translate("Friend", "\346\237\245\346\211\276\347\224\250\346\210\267", nullptr));
        onlineUser->setText(QCoreApplication::translate("Friend", "\345\234\250\347\272\277\347\224\250\346\210\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Friend: public Ui_Friend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIEND_H
