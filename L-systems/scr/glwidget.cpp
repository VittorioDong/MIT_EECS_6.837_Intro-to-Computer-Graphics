#include "glwidget.h"
#include<cstring>
#include <QVector2D>
#include<iostream>
#include <sstream>
using namespace std;

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    program(nullptr),
    vbo(nullptr)
{
    // ����������һЩĬ��ֵ
    angle = 25;
    length = 0.01;
    backColor_R = 0;
    backColor_G = 0;
    backColor_B = 0;
    lineColor_R = 1;
    lineColor_G = 1;
    lineColor_B = 1;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete vbo;
    delete program;
    doneCurrent();
}

void GLWidget::updateLSystem(int iterations, const string& axiom, const map<char, string>& rules, float _angle, float lineLength)
{
    //qDebug() << "Entering GLWidget::updateLSystem()" << endl;;
    angle = _angle;
    length = lineLength;

    lSystem = generateLSystem(iterations, axiom, rules);

    //qDebug() << "lSystem = " << QString::fromStdString(lSystem) << endl;

    updateVertexBuffer();

    update();
    //qDebug() << "Exiting GLWidget::updateLSystem()" << endl;
}

// help function
string createShaderCode(float r, float g, float b) {
    ostringstream os;
    os << "void main() { gl_FragColor = vec4(" << r << ", " << g << ", " << b << ", 1.0); }";
    return os.str();
}

void GLWidget::initializeGL()
{
    //qDebug() << "Entering GLWidget::initializeGL()" << endl;

    initializeOpenGLFunctions();

    // ���ñ�����ɫ
    glClearColor(backColor_R / 255, backColor_G / 255, backColor_B / 255, 1.0f);
    //qDebug() << "backColor = " << backColor_R << "," << backColor_G << "," << backColor_B << endl;

    // ������ɫ��
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "attribute highp vec4 posAttr;\n"
        "void main() { gl_Position = posAttr; }");
    // ���û���ͼ�ε���ɫ
    string shaderCode = createShaderCode(lineColor_R, lineColor_G, lineColor_B);
    //qDebug() << "lineColor = " << lineColor_R << "," << lineColor_G << "," << lineColor_B << endl;
	program->addShaderFromSourceCode(QOpenGLShader::Fragment, shaderCode.c_str());
    //program->addShaderFromSourceCode(QOpenGLShader::Fragment,
    //    "void main() { gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); }");
    program->link();

    // ���ö��㻺��������
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    vbo->release();

    //qDebug() << "Exiting GLWidget::initializeGL()" << endl;
}

void GLWidget::resizeGL(int w, int h)
{
    // ����ͶӰ��ƥ�䴰�ڿ�߱�
    // �����ͼ�����ʺ���x��y��[-1,1]
    float aspect = float(w) / float(h);
    glViewport(0, 0, w, h);
    program->bind();
    program->setUniformValue("projection", QMatrix4x4(aspect, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1));
    //program->setUniformValue("projection", QMatrix4x4(aspect, 0, 0, 0,
    //    0, 1, 0, 0,
    //    0, 0, 1, 0,
    //    0, 0, 0, 1));
    program->release();
}

void GLWidget::paintGL()
{

    // ���ñ�����ɫ
    glClearColor(backColor_R / 255, backColor_G / 255, backColor_B / 255, 1.0f);

    // ������ɫ��
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "attribute highp vec4 posAttr;\n"
        "void main() { gl_Position = posAttr; }");
    // ���û���ͼ�ε���ɫ
    string shaderCode = createShaderCode(lineColor_R / 255, lineColor_G / 255, lineColor_B / 255);
    qDebug() << QString::fromStdString(shaderCode) << endl;
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, shaderCode.c_str());
    program->link();


    // ���� L-system
    glClear(GL_COLOR_BUFFER_BIT);
    program->bind();
    vbo->bind();
    program->enableAttributeArray("posAttr");
    program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 2);
    glDrawArrays(GL_LINES, 0, vertices.size() / 2);
    program->disableAttributeArray("posAttr");
    vbo->release();
    program->release();
}

string GLWidget::generateLSystem(int iterations, string axiom, map<char, string> rules)
{
    string currentString = axiom;

    for (int i = 0; i < iterations; i++) {
        string newString = "";

        for (char& c : currentString) {
            if (rules.count(c) > 0) {
                newString += rules[c];
            }
            else {
                newString += c;
            }
        }

        currentString = newString;
    }

    return currentString;
}

void GLWidget::updateVertexBuffer()
{
    // �� L-system �ַ���ת��Ϊֱ�߶���
    QVector2D pos(0, 0);
    float currentAngle = 0; // ����
    stack<QVector2D> posStack;
    stack<float> angleStack;

    vertices.clear();

    for (char c : lSystem) {
        if (c == 'F') {
            QVector2D newPos = pos + length * QVector2D(sin(currentAngle), cos(currentAngle));
            vertices.push_back(pos.x());
            vertices.push_back(pos.y());
            vertices.push_back(newPos.x());
            vertices.push_back(newPos.y());
            pos = newPos;
        }
        else if (c == '+') {
            currentAngle += angle * 3.1415926535 / 180;
        }
        else if (c == '-') {
            currentAngle -= angle * 3.1415926535 / 180;
        }
        else if (c == '[') {
            posStack.push(pos);
            angleStack.push(currentAngle);
        }
        else if (c == ']') {
            pos = posStack.top();
            posStack.pop();
            currentAngle = angleStack.top();
            angleStack.pop();
        }
    }

    // Update the vertex buffer object
    vbo->bind();
    vbo->allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    vbo->release();
}
