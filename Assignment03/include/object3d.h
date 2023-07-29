#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/gl.h>

#include "material.h"
#include "ray.h"
#include "hit.h"

class Object3D {

public:

    Object3D() {};
    ~Object3D() {};
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    virtual void paint(void) = 0;

protected:

    Material* material;

};

#endif