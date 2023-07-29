#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "glwidget.h"
#include <GL/gl.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindowClass ui;
    GLWidget* glWidget;
};
