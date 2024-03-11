/********************************************************************************
** Form generated from reading UI file 'client1.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT1_H
#define UI_CLIENT1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client1
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Client1)
    {
        if (Client1->objectName().isEmpty())
            Client1->setObjectName(QString::fromUtf8("Client1"));
        Client1->resize(800, 600);
        centralwidget = new QWidget(Client1);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(40, 10, 196, 24));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        Client1->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Client1);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        Client1->setMenuBar(menubar);
        statusbar = new QStatusBar(Client1);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Client1->setStatusBar(statusbar);

        retranslateUi(Client1);

        QMetaObject::connectSlotsByName(Client1);
    } // setupUi

    void retranslateUi(QMainWindow *Client1)
    {
        Client1->setWindowTitle(QCoreApplication::translate("Client1", "Client1", nullptr));
        pushButton->setText(QCoreApplication::translate("Client1", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client1: public Ui_Client1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT1_H
