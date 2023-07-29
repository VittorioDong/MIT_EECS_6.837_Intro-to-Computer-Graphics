#include "curve.h"
#include <GL/freeglut.h>

extern double PI;

// ############################################
// # Class Curve
// ############################################
void Curve::Paint(ArgParser *args) {
    // draw lines
    glLineWidth(1);
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (Vec3f &v: vertices) {
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();

    // draw curves
    drawCurves(args);

    // Draw control points
    glPointSize(5);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (Vec3f &v: vertices) {
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();

}

// ############################################
// # Class BezierCurve
// ############################################
void BezierCurve::drawCurves(ArgParser *args) {
    glLineWidth(3);
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    int tessellation = args->curve_tessellation;
    float d = 1.0f / tessellation;
    for (int i = 0; i < num_vertices - 3; i += 3) {
        Matrix P;
        for (int j = 0; j < 4; j++) {
            P.Set(j, 0, vertices[i + j].x());
            P.Set(j, 1, vertices[i + j].y());
            P.Set(j, 2, vertices[i + j].z());
        }
        for (int j = 0; j <= tessellation; j++) {
            float t = d * j;
            float t2 = t * t;
            float t3 = t2 * t;
            Vec4f T(t3, t2, t, 1);
            Bezier_Matrix.Transform(T);
            P.Transform(T);
            glVertex3f(T.x(), T.y(), T.z());
        }
    }
    glEnd();
}

void BezierCurve::OutputBezier(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bezier\nnum_vertices %d\n", num_vertices);
    for (Vec3f &v: vertices) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

void BezierCurve::OutputBSpline(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bspline\nnum_vertices %d\n", num_vertices);
    Matrix P;
    for (int i = 0; i < num_vertices; i++) {
        P.Set(i, 0, vertices[i].x());
        P.Set(i, 1, vertices[i].y());
        P.Set(i, 2, vertices[i].z());
    }
    Matrix inv_BSpline_Matrix = BSpline_Matrix;
    inv_BSpline_Matrix.Inverse();
    Matrix res = P * Bezier_Matrix * inv_BSpline_Matrix;
    for (int i = 0; i < num_vertices; i++) {
        fprintf(file, "%f %f %f\n", res.Get(i, 0), res.Get(i, 1), res.Get(i, 2));
    }
}

void BezierCurve::moveControlPoint(int selectedPoint, float x, float y) {
    vertices[selectedPoint].Set(x, y, 0);
}

// 禁止在Bezier曲线上添加或删除控制点
void BezierCurve::addControlPoint(int selectedPoint, float x, float y) {}
void BezierCurve::deleteControlPoint(int selectedPoint) {}

TriangleMesh *BezierCurve::OutputTriangles(ArgParser *args) {
    int curve_tessellation = args->curve_tessellation;
    int revolution_tessellation = args->revolution_tessellation;
    float curve_d = 1.0f / curve_tessellation;
    float revolution_d = 2.0 * PI / revolution_tessellation;
    TriangleNet *triangleNet = new TriangleNet(revolution_tessellation, num_vertices / 3 * curve_tessellation);
    // 对于Bezier曲线，每次迭代时索引i增加3，而不是1，
    // 这是因为Bezier曲线的每个曲线段由4个控制点定义，
    // 其中最后一个控制点是下一个曲线段的第一个控制点。
    // 因此，对于连续的Bezier曲线段，每3个点就有一个新的曲线段。
    for (int i = 0; i < num_vertices - 3; i += 3) {
        Matrix P;
        for (int j = 0; j < 4; j++) {
            P.Set(j, 0, vertices[i + j].x());
            P.Set(j, 1, vertices[i + j].y());
            P.Set(j, 2, vertices[i + j].z());
        }
        // 在曲线上采样
        for (int j = 0; j <= curve_tessellation; j++) {
            float t = curve_d * j;
            float t2 = t * t;
            float t3 = t2 * t;
            Vec4f T(t3, t2, t, 1);
            Bezier_Matrix.Transform(T);
            P.Transform(T);
            // 绕y轴旋转采样
            for (int k = 0; k <= revolution_tessellation; k++) {
                float theta = revolution_d * k;
                float x = cos(theta) * T.x() + sin(theta) * T.z();
                float y = T.y();
                float z = -sin(theta) * T.x() + cos(theta) * T.z();
                triangleNet->SetVertex(k, j + (i / 3 * curve_tessellation), Vec3f(x, y, z));
            }
        }
    }
    return triangleNet;
}

// ############################################
// # Class BSplineCurve
// ############################################
void BSplineCurve::drawCurves(ArgParser *args) {
    glLineWidth(3);
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    int tessellation = args->curve_tessellation;
    float d = 1.0f / tessellation;
    for (int i = 0; i < num_vertices - 3; i++) {
        Matrix P;
        for (int j = 0; j < 4; j++) {
            P.Set(j, 0, vertices[i + j].x());
            P.Set(j, 1, vertices[i + j].y());
            P.Set(j, 2, vertices[i + j].z());
        }
        for (int j = 0; j <= tessellation; j++) {
            float t = d * j;
            float t2 = t * t;
            float t3 = t2 * t;
            Vec4f T(t3, t2, t, 1);
            BSpline_Matrix.Transform(T);
            P.Transform(T);
            glVertex3f(T.x(), T.y(), T.z());
        }
    }
    glEnd();
}

void BSplineCurve::OutputBezier(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bezier\nnum_vertices %d\n", num_vertices);
    Matrix P;
    for (int i = 0; i < num_vertices; i++) {
        P.Set(i, 0, vertices[i].x());
        P.Set(i, 1, vertices[i].y());
        P.Set(i, 2, vertices[i].z());
    }
    Matrix inv_Bezier_Matrix = Bezier_Matrix;
    inv_Bezier_Matrix.Inverse();
    Matrix res = P * BSpline_Matrix * inv_Bezier_Matrix;
    for (int i = 0; i < num_vertices; i++) {
        fprintf(file, "%f %f %f\n", res.Get(i, 0), res.Get(i, 1), res.Get(i, 2));
    }
}

void BSplineCurve::OutputBSpline(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bspline\nnum_vertices %d\n", num_vertices);
    for (Vec3f &v: vertices) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

void BSplineCurve::moveControlPoint(int selectedPoint, float x, float y) {
    vertices[selectedPoint].Set(x, y, 0);
}

void BSplineCurve::addControlPoint(int selectedPoint, float x, float y) {
    num_vertices++;
    vertices.insert(vertices.begin() + selectedPoint, Vec3f(x, y, 0));
}

void BSplineCurve::deleteControlPoint(int selectedPoint) {
    // 小于等于4个控制点时，禁止删除操作
    if (num_vertices <= 4)
        return;
    vertices.erase(vertices.begin() + selectedPoint);
    num_vertices--;
}

TriangleMesh *BSplineCurve::OutputTriangles(ArgParser *args) {
    int curve_tessellation = args->curve_tessellation;
    int revolution_tessellation = args->revolution_tessellation;
    float curve_d = 1.0f / curve_tessellation;
    float revolution_d = 2.0 * PI / revolution_tessellation;
    TriangleNet *triangleNet = new TriangleNet(revolution_tessellation, (num_vertices - 3) * curve_tessellation);

    // 滑动窗口
    for (int i = 0; i < num_vertices - 3; i++) {
        Matrix P;
        for (int j = 0; j < 4; j++) {
            P.Set(j, 0, vertices[i + j].x());
            P.Set(j, 1, vertices[i + j].y());
            P.Set(j, 2, vertices[i + j].z());
        }
        // 在曲线上采样
        for (int j = 0; j <= curve_tessellation; j++) {
            float t = curve_d * j;
            float t2 = t * t;
            float t3 = t2 * t;
            Vec4f T(t3, t2, t, 1);
            BSpline_Matrix.Transform(T);
            P.Transform(T);
            // 绕y轴旋转采样
            for (int k = 0; k <= revolution_tessellation; k++) {
                float theta = revolution_d * k;
                float x = cos(theta) * T.x() + sin(theta) * T.z();
                float y = T.y();
                float z = -sin(theta) * T.x() + cos(theta) * T.z();
                triangleNet->SetVertex(k, j + (i * curve_tessellation), Vec3f(x, y, z));
            }
        }
    }
    return triangleNet;
}




