#include "MainWindow.h"
#include "glwidget.h"
#include<qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    glWidget = new GLWidget(this);
    ui.horizontalLayout->addWidget(glWidget);
    // 预设
    ui.lineEdit_angle->setText("25");
    ui.lineEdit_length->setText("0.01");
    ui.lineEdit_iteration->setText("5");
    ui.lineEdit_axiom->setText("X");
    ui.lineEdit_rule->setText("X->F+[[X]-X]-F[-FX]+X,F->FF");
    ui.lineEdit_backColor->setText("0,0,0");
    ui.lineEdit_lineColor->setText("255,255,255");
}

void MainWindow::on_actionHelp_Document_triggered(bool checked)
{
    QMessageBox userGuideBox;
    userGuideBox.setWindowTitle("User Guide");

    // Assuming that you have the user guide text in a QString
    QString userGuideText;
    userGuideText += QStringLiteral("1. 'Angle'：输入角度（浮点数）。\n");
    userGuideText += QStringLiteral("2. 'Length'：输入线段长度（浮点数），该数值需要根据 'Iteration' 的值进行调整，建议小于1。\n");
    userGuideText += QStringLiteral("3. 'Iteration'：输入迭代次数（正整数）。\n");
    userGuideText += QStringLiteral("4. 'Axiom'：输入起始值（一个大写字母）。\n");
    userGuideText += QStringLiteral("5. 'Rules'：输入生成规则（字符串），多个规则使用英文逗号','分隔，例如：X->F+[[X]-X]-F[-FX]+X,F->FF。\n");
    userGuideText += QStringLiteral("6. 'Background Color'（可以不填）：输入背景颜色，例如：100,100,100。默认为黑色。\n");
    userGuideText += QStringLiteral("7. 'Line Color' （可以不填）：输入线段颜色，例如：100,100,100。默认为白色。");
    userGuideBox.setText(userGuideText);
    userGuideBox.exec();
}

void MainWindow::on_pushButton_clicked()
{
    float angle = ui.lineEdit_angle->text().toFloat();
    float length = ui.lineEdit_length->text().toFloat();
    int iterations = ui.lineEdit_iteration->text().toInt();
    string axiom = ui.lineEdit_axiom->text().toStdString();

    if (ui.lineEdit_backColor->text() != "")
    {
        //qDebug() << "Change BackColor" << endl;
        QStringList list = ui.lineEdit_backColor->text().split(',');
        glWidget->backColor_R = list[0].toFloat();
        glWidget->backColor_G = list[1].toFloat();
        glWidget->backColor_B = list[2].toFloat();
    }
    if (ui.lineEdit_lineColor->text() != "")
    {
        //qDebug() << "Change LineColor" << endl;
        QStringList list = ui.lineEdit_lineColor->text().split(',');
        glWidget->lineColor_R = list[0].toFloat();
        glWidget->lineColor_G = list[1].toFloat();
        glWidget->lineColor_B = list[2].toFloat();
    }

    std::map<char, std::string> rules;
    // 分隔规则字符串，并添加到映射中
    QStringList rulesList = ui.lineEdit_rule->text().split(",");
    for (const auto& rule : rulesList) {
        auto key = rule[0].toLatin1();
        auto value = rule.mid(3).toStdString();
        rules[key] = value;
        //qDebug() << "rules[key] = " << QString::fromStdString(value) << endl;
    }

    // 使用输入的参数来更新L系统
    glWidget->updateLSystem(iterations, axiom, rules, angle, length);
}
