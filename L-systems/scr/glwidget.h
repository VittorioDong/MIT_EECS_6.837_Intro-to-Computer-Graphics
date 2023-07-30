#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <map>
#include <stack>
#include <string>
using namespace std;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    void updateLSystem(int iterations, const std::string& axiom, const std::map<char, std::string>& rules, float _angle, float lineLength);

    float backColor_R;
    float backColor_G;
    float backColor_B;
    float lineColor_R;
    float lineColor_G;
    float lineColor_B;

    
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    string generateLSystem(int iterations, string axiom, map<char, std::string> rules);
    void updateVertexBuffer();

    std::string lSystem;
    float angle;
    float length;

    QOpenGLShaderProgram* program;
    QOpenGLBuffer* vbo;
    std::vector<GLfloat> vertices;
};
#endif // GLWIDGET_H
