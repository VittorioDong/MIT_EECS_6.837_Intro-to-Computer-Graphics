#include "MainWindow.h"
#include "glwidget.h"
#include<qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    glWidget = new GLWidget(this);
    ui.horizontalLayout->addWidget(glWidget);
    // Ԥ��
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
    userGuideText += QStringLiteral("1. 'Angle'������Ƕȣ�����������\n");
    userGuideText += QStringLiteral("2. 'Length'�������߶γ��ȣ���������������ֵ��Ҫ���� 'Iteration' ��ֵ���е���������С��1��\n");
    userGuideText += QStringLiteral("3. 'Iteration'�������������������������\n");
    userGuideText += QStringLiteral("4. 'Axiom'��������ʼֵ��һ����д��ĸ����\n");
    userGuideText += QStringLiteral("5. 'Rules'���������ɹ����ַ��������������ʹ��Ӣ�Ķ���','�ָ������磺X->F+[[X]-X]-F[-FX]+X,F->FF��\n");
    userGuideText += QStringLiteral("6. 'Background Color'�����Բ�������뱳����ɫ�����磺100,100,100��Ĭ��Ϊ��ɫ��\n");
    userGuideText += QStringLiteral("7. 'Line Color' �����Բ���������߶���ɫ�����磺100,100,100��Ĭ��Ϊ��ɫ��");
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
    // �ָ������ַ���������ӵ�ӳ����
    QStringList rulesList = ui.lineEdit_rule->text().split(",");
    for (const auto& rule : rulesList) {
        auto key = rule[0].toLatin1();
        auto value = rule.mid(3).toStdString();
        rules[key] = value;
        //qDebug() << "rules[key] = " << QString::fromStdString(value) << endl;
    }

    // ʹ������Ĳ���������Lϵͳ
    glWidget->updateLSystem(iterations, axiom, rules, angle, length);
}
