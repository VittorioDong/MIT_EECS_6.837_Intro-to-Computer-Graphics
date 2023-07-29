#include "glwidget.h"
#include<cstring>
#include <QVector2D>
#include<iostream>
using namespace std;

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    program(nullptr),
    vbo(nullptr)
{
    // 在这里设置一些默认值
    angle = 25;
    length = 0.01;
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
    qDebug() << "Entering GLWidget::updateLSystem()" << endl;;
    angle = _angle;
    length = lineLength;

    lSystem = generateLSystem(iterations, axiom, rules);

    qDebug() << "lSystem = " << QString::fromStdString(lSystem) << endl;

    updateVertexBuffer();

    update();
    qDebug() << "Exiting GLWidget::updateLSystem()" << endl;
}

void GLWidget::initializeGL()
{
    qDebug() << "Entering GLWidget::initializeGL()" << endl;

    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 设置着色器
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "attribute highp vec4 posAttr;\n"
        "void main() { gl_Position = posAttr; }");
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
        "void main() { gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); }");
    program->link();

    // 设置顶点缓冲区对象
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    vbo->release();

    qDebug() << "Exiting GLWidget::initializeGL()" << endl;
}

void GLWidget::resizeGL(int w, int h)
{
    // 更新投影以匹配窗口宽高比
    // 假设绘图总是适合于x和y的[-1,1]
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
    // 绘制 L-system
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
    // 将 L-system 字符串转换为直线顶点
    QVector2D pos(0, 0);
    float currentAngle = 0; // in radians
    stack<QVector2D> posStack;
    stack<float> angleStack;

    vertices.clear();

    for (char c : lSystem) {
        if (c == 'F') {
            QVector2D newPos = pos + length * QVector2D(cos(currentAngle), sin(currentAngle));
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
