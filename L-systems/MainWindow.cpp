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
    // �ָ������ַ���������ӵ�ӳ����
    QStringList rulesList = ui.lineEdit_rule->text().split(",");
    for (const auto& rule : rulesList) {
        auto key = rule[0].toLatin1();
        auto value = rule.mid(2).toStdString();
        rules[key] = value;
        qDebug() << "rules[key] = " << QString::fromStdString(value) << endl;
    }

    // ʹ������Ĳ���������Lϵͳ
    glWidget->updateLSystem(iterations, axiom, rules, angle, length);
}
