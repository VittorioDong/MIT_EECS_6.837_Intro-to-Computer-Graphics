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
#include <QtWidgets/QHBoxLayout>
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
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_angle;
    QLabel *label_2;
    QLineEdit *lineEdit_length;
    QLabel *label_5;
    QLineEdit *lineEdit_iteration;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_axiom;
    QLineEdit *lineEdit_rule;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(1177, 772);
        centralwidget = new QWidget(MainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(190, 400, 221, 101));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(15);
        pushButton->setFont(font);
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(110, 160, 411, 201));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_angle = new QLineEdit(layoutWidget);
        lineEdit_angle->setObjectName(QString::fromUtf8("lineEdit_angle"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_angle->sizePolicy().hasHeightForWidth());
        lineEdit_angle->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(15);
        lineEdit_angle->setFont(font1);

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_angle);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_length = new QLineEdit(layoutWidget);
        lineEdit_length->setObjectName(QString::fromUtf8("lineEdit_length"));
        sizePolicy1.setHeightForWidth(lineEdit_length->sizePolicy().hasHeightForWidth());
        lineEdit_length->setSizePolicy(sizePolicy1);
        lineEdit_length->setFont(font1);

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_length);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        lineEdit_iteration = new QLineEdit(layoutWidget);
        lineEdit_iteration->setObjectName(QString::fromUtf8("lineEdit_iteration"));
        sizePolicy1.setHeightForWidth(lineEdit_iteration->sizePolicy().hasHeightForWidth());
        lineEdit_iteration->setSizePolicy(sizePolicy1);
        lineEdit_iteration->setFont(font1);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_iteration);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        lineEdit_axiom = new QLineEdit(layoutWidget);
        lineEdit_axiom->setObjectName(QString::fromUtf8("lineEdit_axiom"));
        sizePolicy1.setHeightForWidth(lineEdit_axiom->sizePolicy().hasHeightForWidth());
        lineEdit_axiom->setSizePolicy(sizePolicy1);
        lineEdit_axiom->setFont(font1);

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_axiom);

        lineEdit_rule = new QLineEdit(layoutWidget);
        lineEdit_rule->setObjectName(QString::fromUtf8("lineEdit_rule"));
        sizePolicy1.setHeightForWidth(lineEdit_rule->sizePolicy().hasHeightForWidth());
        lineEdit_rule->setSizePolicy(sizePolicy1);
        lineEdit_rule->setFont(font1);

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_rule);


        horizontalLayout->addWidget(widget);


        verticalLayout->addLayout(horizontalLayout);

        MainWindowClass->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1177, 26));
        MainWindowClass->setMenuBar(menuBar);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QCoreApplication::translate("MainWindowClass", "L-systems Renderer", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindowClass", "Rendering", nullptr));
        label->setText(QCoreApplication::translate("MainWindowClass", "Angle", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindowClass", "Length", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindowClass", "iteration", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindowClass", "Axiom", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindowClass", "Rules", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
