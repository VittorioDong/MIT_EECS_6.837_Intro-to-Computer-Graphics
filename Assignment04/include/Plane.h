#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#define LARGE 1e3     // 平面的边长

class Plane : public Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m) 
        :normal(normal), d(d) {
        material = m;
        // 对法向量进行归一化处理
        this->normal.Normalize();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        // 计算射线和平面的交点，公式为 (P0 + t*dir) . normal = d
        // 这里的P0为射线的起点，dir为射线的方向，t为参数，我们需要求解t
        // 首先计算射线的方向和法线的点积
        // float dot = r.getDirection().Dot3(normal);
        // if (fabs(dot) < 1e-6) {
        //     // 如果射线的方向和法线平行，那么它们不会相交
        //     return false;
        // } else {
        //     // 否则求解t
        //     float t = (d - normal.Dot3(r.getOrigin())) / dot;
        //     if (t >= tmin && t < h.getT()) {
        //         // 如果求解出的t在有效范围内，更新Hit
        //         h.set(t, material, normal, r);
        //         return true;
        //     }
        // }
        // return false;
        Vec3f direction = r.getDirection();
        Vec3f origin = r.getOrigin();
        if (direction.Dot3(normal) == 0)
            return false;
        float t = (d - origin.Dot3(normal)) / (direction.Dot3(normal));
        if (t > tmin && t < h.getT())
        {
            h.set(t, material, normal, r);
            return true;
        }
        else
            return false;
    }

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin)
    {
        return intersect(r, h, tmin);
    }

    void paint(void)
    {
        material->glSetMaterial();
        // 在平面上生成两个基底
        Vec3f v;
        if (normal.y() == 0 && normal.z() == 0)
        {
            v = Vec3f(0, 1, 0);
        }
        else
        {
            v = Vec3f(1, 0, 0);
        }
        Vec3f b1, b2;
        Vec3f::Cross3(b1, v, normal);
        Vec3f::Cross3(b2, normal, b1);
        b1.Normalize();
        b2.Normalize();
        // 将全局原点投影到平面上
        Vec3f projected_origin = d / (normal.Length() * normal.Length()) * normal;
        Vec3f P1 = projected_origin - LARGE * b1 - LARGE * b2;
        Vec3f P2 = projected_origin - LARGE * b1 + LARGE * b2;
        Vec3f P3 = projected_origin + LARGE * b1 + LARGE * b2;
        Vec3f P4 = projected_origin + LARGE * b1 - LARGE * b2;

        printf("normal = (%f,%f,%f)\n",normal.x(), normal.y(), normal.z());
        printf("P1 = (%f,%f,%f)\n",P1.x(), P1.y(), P1.z());
        printf("P2 = (%f,%f,%f)\n",P2.x(), P2.y(), P2.z());
        printf("P3 = (%f,%f,%f)\n",P3.x(), P3.y(), P3.z());
        printf("P4 = (%f,%f,%f)\n",P4.x(), P4.y(), P4.z());

        glBegin(GL_QUADS);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(P1.x(), P1.y(), P1.z());
        glVertex3f(P2.x(), P2.y(), P2.z());
        glVertex3f(P3.x(), P3.y(), P3.z());
        glVertex3f(P4.x(), P4.y(), P4.z());
        glEnd();
    }

private:
    Vec3f normal;
    float d;
};

#endif