#include "MainWindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    glWidget = new GLWidget(this);
    ui.horizontalLayout->addWidget(glWidget);
}

void MainWindow::on_pushButton_clicked()
{
    float angle = ui.lineEdit_angle->text().toFloat();
    float length = ui.lineEdit_length->text().toFloat();
    int iterations = ui.lineEdit_iteration->text().toInt();
    std::string axiom = ui.lineEdit_axiom->text().toStdString();

    std::map<char, std::string> rules;
    // 分隔规则字符串，并添加到映射中
    QStringList rulesList = ui.lineEdit_rule->text().split(",");
    for (const auto& rule : rulesList) {
        auto key = rule[0].toLatin1();
        auto value = rule.mid(2).toStdString();
        rules[key] = value;
        qDebug() << "rules[key] = " << QString::fromStdString(value) << endl;
    }

    // 使用输入的参数来更新L系统
    glWidget->updateLSystem(iterations, axiom, rules, angle, length);
}
