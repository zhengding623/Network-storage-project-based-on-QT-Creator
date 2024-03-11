/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *user_LE;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *pwd_LE;
    QPushButton *regist_PB;
    QSpacerItem *horizontalSpacer;
    QPushButton *login_PB;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(297, 253);
        centralwidget = new QWidget(Client);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        user_LE = new QLineEdit(centralwidget);
        user_LE->setObjectName(QString::fromUtf8("user_LE"));

        horizontalLayout->addWidget(user_LE);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        pwd_LE = new QLineEdit(centralwidget);
        pwd_LE->setObjectName(QString::fromUtf8("pwd_LE"));
        pwd_LE->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(pwd_LE);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 3);

        regist_PB = new QPushButton(centralwidget);
        regist_PB->setObjectName(QString::fromUtf8("regist_PB"));

        gridLayout->addWidget(regist_PB, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(104, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        login_PB = new QPushButton(centralwidget);
        login_PB->setObjectName(QString::fromUtf8("login_PB"));

        gridLayout->addWidget(login_PB, 1, 2, 1, 1);

        Client->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Client);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 297, 21));
        Client->setMenuBar(menubar);
        statusbar = new QStatusBar(Client);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Client->setStatusBar(statusbar);

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QMainWindow *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "\345\256\242\346\210\267\347\253\257", nullptr));
        label->setText(QCoreApplication::translate("Client", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Client", "\345\257\206  \347\240\201\357\274\232", nullptr));
        regist_PB->setText(QCoreApplication::translate("Client", "\346\263\250\345\206\214", nullptr));
        login_PB->setText(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
