#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Object3D.h"

class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) 
        : a(a), b(b), c(c) {
        material = m;
        // 计算法向量：b-a和c-a的叉乘，并进行归一化
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        // 使用 Möller–Trumbore intersection algorithm 实现射线与三角形的交点检测
        Vec3f S = r.getOrigin() - a;
        Vec3f E1 = b - a;
        Vec3f E2 = c - a;
        Vec3f S1;
        Vec3f S2;
        Vec3f::Cross3(S1, r.getDirection(), E2);
        Vec3f::Cross3(S2, S, E1);

        float S1E1 = S1.Dot3(E1);
        if(abs(S1E1) < 1e-6) return false;
        float t = S2.Dot3(E2) / S1E1;
        float b1 = S1.Dot3(S) / S1E1;
        float b2 = S2.Dot3(r.getDirection()) / S1E1;

        if (t >= 0.f && b1 >= 0.f && b2 >= 0.f && (1 - b1 - b2) >= 0.f)
        {
            if (t >= tmin && t < h.getT())
            {
                // 如果求解出的t在有效范围内，更新Hit
                h.set(t, material, normal, r);
                return true;
            }
        }
        return false;
    }

    // 实现 paint 方法
    virtual void paint(void)
    {
        material->glSetMaterial();
        Vec3f ab = a - b;
        Vec3f ac = a - c;
        Vec3f normal;
        Vec3f::Cross3(normal, ab, ac);
        glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(a.x(), a.y(), a.z());
        glVertex3f(b.x(), b.y(), b.z());
        glVertex3f(c.x(), c.y(), c.z());
        glEnd();
    }

private:
    Vec3f a, b, c;
    Vec3f normal;
};

#endif