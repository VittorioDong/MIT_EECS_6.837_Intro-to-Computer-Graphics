#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Object3D.h"
#include "grid.h"

class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) 
        : a(a), b(b), c(c) {
        material = m;
        // 计算法向量：b-a和c-a的叉乘，并进行归一化
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
        boundingBox = new BoundingBox(Vec3f(min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z())),
                                      Vec3f(max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z())));
        boundingBox->Extend(c);
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

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin)
    {
        return intersect(r, h, tmin);
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

    virtual bool is_triangle() const { return true; }
    
    BoundingBox *getTriangleBoundingBox(const Matrix *m) const
    {
        if (!m)
            return boundingBox;
        Vec3f aa = a, bb = b, cc = c;
        m->Transform(aa);
        m->Transform(bb);
        m->Transform(cc);
        BoundingBox *t = new BoundingBox(Vec3f(min(aa.x(), bb.x()), min(aa.y(), bb.y()), min(aa.z(), bb.z())),
                                         Vec3f(max(aa.x(), bb.x()), max(aa.y(), bb.y()), max(aa.z(), bb.z())));
        t->Extend(cc);
        return t;
    }

    void insertIntoGrid(Grid *g, Matrix *m)
    {
        BoundingBox *bb_new = getTriangleBoundingBox(m);
        BoundingBox *grid_bb = g->getBoundingBox();
        Vec3f grid_min = grid_bb->getMin();
        Vec3f grid_max = grid_bb->getMax();
        int nx = g->nx;
        int ny = g->ny;
        int nz = g->nz;
        float cellx = (grid_max - grid_min).x() / float(nx);
        float celly = (grid_max - grid_min).y() / float(ny);
        float cellz = (grid_max - grid_min).z() / float(nz);
        Vec3f obj_min = bb_new->getMin();
        Vec3f obj_max = bb_new->getMax();
        int start_i = int((obj_min - grid_min).x() / cellx);
        int start_j = int((obj_min - grid_min).y() / celly);
        int start_k = int((obj_min - grid_min).z() / cellz);
        int end_i = int((obj_max - grid_min).x() / cellx);
        int end_j = int((obj_max - grid_min).y() / celly);
        int end_k = int((obj_max - grid_min).z() / cellz);
        if (start_i > nx || start_j > ny || start_k > nz || end_i < 0 || end_j < 0 || end_k < 0)
            return;
        start_i = max(start_i, 0);
        start_j = max(start_j, 0);
        start_k = max(start_k, 0);
        end_i = min(end_i, nx - 1);
        end_j = min(end_j, ny - 1);
        end_k = min(end_k, nz - 1);
        // note this
        if (start_i == nx)
            start_i--;
        if (start_j == ny)
            start_j--;
        if (start_k == nz)
            start_k--;

        for (int k = start_k; k <= end_k; ++k)
        {
            for (int j = start_j; j <= end_j; ++j)
            {
                for (int i = start_i; i <= end_i; ++i)
                {
                    int index = nx * ny * k + nx * j + i;
                    g->opaque[index].push_back(this);
                }
            }
        }
    }

private:
    Vec3f a, b, c;
    Vec3f normal;
};

#endif