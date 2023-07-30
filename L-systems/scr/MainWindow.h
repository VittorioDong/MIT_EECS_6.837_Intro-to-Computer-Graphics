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
    void on_actionHelp_Document_triggered(bool checked);

private:
    Ui::MainWindowClass ui;
    GLWidget* glWidget;
};
