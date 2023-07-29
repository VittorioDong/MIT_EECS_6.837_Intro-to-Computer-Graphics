#ifndef CURVE_H
#define CURVE_H

#include "spline.h"
#include "matrix.h"
#include <vector>
using namespace std;

// ############################################
// # Class Curve
// ############################################
class Curve : public Spline {
public:
    // CONSTRUCTOR
    Curve(int num) {
        num_vertices = num;
        vertices.resize(num_vertices);
    }

    // SET
    void set(int i, const Vec3f &v) override { vertices[i] = v; }

    // FOR VISUALIZATION
    void Paint(ArgParser *args) override;

    // FOR CONTROL POINT PICKING
    int getNumVertices() override { return num_vertices; }
    Vec3f getVertex(int i) override { return vertices[i]; }

    virtual void drawCurves(ArgParser *args) = 0;
    
protected:
    int num_vertices;
    vector<Vec3f> vertices; // 控制点数组
};

// ############################################
// # Class BezierCurve
// ############################################
class BezierCurve : public Curve {
public:
    // CONSTRUCTOR
    BezierCurve(int num) : Curve(num) {}

    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;
    void addControlPoint(int selectedPoint, float x, float y) override;
    void deleteControlPoint(int selectedPoint) override;

    void drawCurves(ArgParser *args) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override; 
};

// ############################################
// # Class BSplineCurve
// ############################################
class BSplineCurve : public Curve {
public:
    // CONSTRUCTOR
    BSplineCurve(int num) : Curve(num) {}

    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;
    void addControlPoint(int selectedPoint, float x, float y) override;
    void deleteControlPoint(int selectedPoint) override;

    void drawCurves(ArgParser *args) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;
};

#endif