#ifndef CAMERA_H
#define CAMERA_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "ray.h"

class Camera {
public:
    // 构造函数和析构函数
    Camera() {}
    virtual ~Camera() {}

    // 纯虚函数
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

#endif