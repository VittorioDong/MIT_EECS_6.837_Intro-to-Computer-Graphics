#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.h"
#include "hit.h"
#include "material.h"

// Object3D 类定义
class Object3D {
public:
    // 默认构造函数
    Object3D() : material(nullptr) {}

    // 有参构造函数
    Object3D(Material* m) : material(m) {}

    // 析构函数
    virtual ~Object3D() {
        delete material;
    }

    // 纯虚函数，用于检测与射线的交集
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

protected:
    Material* material; // Material实例的指针
};

#endif