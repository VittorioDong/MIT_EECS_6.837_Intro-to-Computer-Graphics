#ifndef SURFACE_H
#define SURFACE_H

#include "curve.h"

// ############################################
// # Class Surface
// ############################################
class Surface : public Spline {
public:
    Surface() {}

};

// ############################################
// # Class SurfaceOfRevolution
// ############################################
class SurfaceOfRevolution : public Surface {
public:
    // CONSTRUCTOR
    SurfaceOfRevolution(Curve *c) : curve(c) {}

    // SET
    void set(int i, const Vec3f &v) override { curve->set(i, v); }

    // FOR VISUALIZATION
    void Paint(ArgParser *args) override;

    // FOR CONTROL POINT PICKING
    int getNumVertices() override { return curve->getNumVertices(); }
    Vec3f getVertex(int i) override { return curve->getVertex(i); }

    void moveControlPoint(int selectedPoint, float x, float y) override;
    void addControlPoint(int selectedPoint, float x, float y) override;
    void deleteControlPoint(int selectedPoint) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

    void OutputBSpline(FILE *file) override;

private:
    Curve *curve;
};

// ############################################
// # Class BezierPatch
// ############################################
class BezierPatch : public Surface {
public:
    // CONSTRUCTOR
    BezierPatch() {
        num_vertices = 16;
        vertices.resize(num_vertices);
    }

    // SET
    void set(int i, const Vec3f &v) override { vertices[i] = v; }

    // FOR VISUALIZATION
    void Paint(ArgParser *args) override;

    // FOR CONTROL POINT PICKING
    int getNumVertices() override { return num_vertices; }
    Vec3f getVertex(int i) override { return vertices[i]; }
    
    TriangleMesh *OutputTriangles(ArgParser *args) override;

private:
    int num_vertices;
    vector<Vec3f> vertices;
};

#endif