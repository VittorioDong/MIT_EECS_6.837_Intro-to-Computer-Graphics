#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "ray.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// ############################################
// # Class Camera
// ############################################
class Camera
{
public:
    // 构造函数和析构函数
    Camera() {}
    virtual ~Camera() {}

    // 纯虚函数
    virtual Ray generateRay(const Vec2f &point) const = 0;
    virtual float getTMin() const = 0;
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

// ############################################
// # Class OrthographicCamera
// ############################################
class OrthographicCamera : public Camera
{
public:
    // 构造函数
    OrthographicCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, float s) : center(c), direction(d), up(u), size(s)
    {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        Vec3f::Cross3(screenUp, horizontal, direction);
        horizontal.Normalize();
        screenUp.Normalize();
    }

    virtual Ray generateRay(const Vec2f &point) const;
    virtual float getTMin() const;
    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    Vec3f screenUp;
    float size;
};

// ############################################
// # Class PerspectiveCamera
// ############################################
class PerspectiveCamera : public Camera
{
public:
    // 构造函数
    PerspectiveCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, float a) : center(c), direction(d), up(u), angle(a)
    {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        Vec3f::Cross3(screenUp, horizontal, direction);
        horizontal.Normalize();
        screenUp.Normalize();
    }

    virtual Ray generateRay(const Vec2f &point) const;
    virtual float getTMin() const;
    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    Vec3f screenUp;
    float angle;
};

#endif
