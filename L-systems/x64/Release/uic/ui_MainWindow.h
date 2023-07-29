/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QMenuBar *menuBar;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_angle;
    QLabel *label1;
    QLineEdit *lineEdit_length;
    QLabel *label2;
    QLineEdit *lineEdit_rule;
    QLabel *label3;
    QLineEdit *lineEdit_axiom;
    QLabel *label4;
    QLineEdit *lineEdit_iteration;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(600, 400);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MainWindowClass->setMenuBar(menuBar);
        centralwidget = new QWidget(MainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_angle = new QLineEdit(centralwidget);
        lineEdit_angle->setObjectName(QString::fromUtf8("lineEdit_angle"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_angle);

        label1 = new QLabel(centralwidget);
        label1->setObjectName(QString::fromUtf8("label1"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label1);

        lineEdit_length = new QLineEdit(centralwidget);
        lineEdit_length->setObjectName(QString::fromUtf8("lineEdit_length"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_length);

        label2 = new QLabel(centralwidget);
        label2->setObjectName(QString::fromUtf8("label2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label2);

        lineEdit_rule = new QLineEdit(centralwidget);
        lineEdit_rule->setObjectName(QString::fromUtf8("lineEdit_rule"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_rule);

        label3 = new QLabel(centralwidget);
        label3->setObjectName(QString::fromUtf8("label3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label3);

        lineEdit_axiom = new QLineEdit(centralwidget);
        lineEdit_axiom->setObjectName(QString::fromUtf8("lineEdit_axiom"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_axiom);

        label4 = new QLabel(centralwidget);
        label4->setObjectName(QString::fromUtf8("label4"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label4);

        lineEdit_iteration = new QLineEdit(centralwidget);
        lineEdit_iteration->setObjectName(QString::fromUtf8("lineEdit_iteration"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_iteration);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButton);


        verticalLayout->addLayout(formLayout);

        MainWindowClass->setCentralWidget(centralwidget);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QCoreApplication::translate("MainWindowClass", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindowClass", "Angle:", nullptr));
        label1->setText(QCoreApplication::translate("MainWindowClass", "Length:", nullptr));
        label2->setText(QCoreApplication::translate("MainWindowClass", "Rules:", nullptr));
        label3->setText(QCoreApplication::translate("MainWindowClass", "Axiom:", nullptr));
        label4->setText(QCoreApplication::translate("MainWindowClass", "Iterations:", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindowClass", "Generate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
