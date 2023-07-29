#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera {
public:
    // 构造函数和析构函数
    Camera() {}
    virtual ~Camera() {}

    // 纯虚函数
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

#endif