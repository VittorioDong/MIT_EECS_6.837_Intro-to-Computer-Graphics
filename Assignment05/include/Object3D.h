#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/gl.h>

#include "material.h"
#include "ray.h"
#include "hit.h"
#include "boundingbox.h"
#include "matrix.h"

class Grid;

class Object3D {

public:

    Object3D() {};
    ~Object3D() {};
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual bool intersectShadowRay(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint(void) = 0;
    BoundingBox* getBoundingBox(){return boundingBox;}

    virtual bool is_triangle() const { return false; } //use a special case for transformed triangles

    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const { return boundingBox; } //use a special case for transformed triangles

    virtual void insertIntoGrid(Grid *g, Matrix *m);

protected:

    Material* material;
    BoundingBox *boundingBox;
};

#endif