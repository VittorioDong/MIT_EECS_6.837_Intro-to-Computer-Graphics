#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include"Object3D.h"

class Sphere : public Object3D {
public:
    // 构造函数
    Sphere(const Vec3f& center, float radius, Material* material)
        : Object3D(material), center(center), radius(radius) {}

    // 实现交集方法
    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
        Vec3f origin_to_center = r.getOrigin() - center;
        float a = r.getDirection().Dot3(r.getDirection());
        float b = 2 * origin_to_center.Dot3(r.getDirection());
        float c = origin_to_center.Dot3(origin_to_center) - radius * radius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return false; // 射线未与球相交
        }

        float sqrt_discriminant = sqrt(discriminant);
        float t1 = (-b - sqrt_discriminant) / (2 * a);
        float t2 = (-b + sqrt_discriminant) / (2 * a);

        // t1 <= t2
        if (t1 >= tmin && t1 < h.getT()) {
            h.set(t1, material, r);
            return true;
        }

        if (t2 >= tmin && t2 < h.getT()) {
            h.set(t2, material, r);
            return true;
        }

        return false; // 射线与球相交，但 t 不在所需范围内
    }

private:
    Vec3f center; // 球的中心
    float radius; // 球的半径
};

#endif
