#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include"Object3D.h"
#include "grid.h"

extern int theta_steps, phi_steps;
extern bool gouraud;

class Sphere : public Object3D {
public:
    // 构造函数
    Sphere(const Vec3f& center, float radius, Material* m)
        : center(center), radius(radius) 
    {
        material = m;
        boundingBox = new BoundingBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }

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
            Vec3f intersectionPoint = r.pointAtParameter(t1);
            Vec3f normal = (intersectionPoint - center);
            normal.Normalize();
            h.set(t1, material, normal, r);
            return true;
        }

        if (t2 >= tmin && t2 < h.getT()) {
            Vec3f intersectionPoint = r.pointAtParameter(t2);
            Vec3f normal = (intersectionPoint - center);
            normal.Normalize();
            h.set(t2, material, normal, r);
            return true;
        }

        return false; // 射线与球相交，但 t 不在所需范围内
    }

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin)
    {
        return intersect(r, h, tmin);
    }

    void paint(void)
    {
        material->glSetMaterial();
        float theta_per_step = 2 * M_PI / theta_steps;
        float phi_per_step = M_PI / phi_steps;
        float phi = -M_PI / 2;
        float theta = 0;
        glBegin(GL_QUADS);
        for (int iPhi = 0; iPhi < phi_steps; iPhi++)
        {
            theta = 0;
            for (int iTheta = 0; iTheta < theta_steps; iTheta++)
            {
                Vec3f P[] = {
                    Vec3f(cos(phi) * sin(theta), sin(phi), cos(phi) * cos(theta)),
                    Vec3f(cos(phi + phi_per_step) * sin(theta), sin(phi + phi_per_step), cos(phi + phi_per_step) * cos(theta)),
                    Vec3f(cos(phi + phi_per_step) * sin(theta + theta_per_step), sin(phi + phi_per_step), cos(phi + phi_per_step) * cos(theta + theta_per_step)),
                    Vec3f(cos(phi) * sin(theta + theta_per_step), sin(phi), cos(phi) * cos(theta + theta_per_step))};

                if (!gouraud)
                { // necessary ?
                    Vec3f normal;
                    Vec3f::Cross3(normal, P[3] - P[0], P[1] - P[0]);
                    normal.Normalize();
                    glNormal3f(normal.x(), normal.y(), normal.z());
                }
                for (int i = 0; i < 4; i++)
                {
                    if (gouraud)
                    {
                        glNormal3f(P[i].x(), P[i].y(), P[i].z());
                    }
                    P[i] = center + radius * P[i];
                    glVertex3f(P[i].x(), P[i].y(), P[i].z());
                }
                theta += theta_per_step;
            }
            phi += phi_per_step;
        }
        glEnd();
    }

    void insertIntoGrid(Grid *g, Matrix *m)
    {
        if (m)
        {
            Object3D::insertIntoGrid(g, m);
            return;
        }
        BoundingBox *grid_bb = g->getBoundingBox();
        Vec3f grid_min = grid_bb->getMin();
        Vec3f grid_max = grid_bb->getMax();
        int nx = g->nx;
        int ny = g->ny;
        int nz = g->nz;
        float cellx = (grid_max - grid_min).x() / float(nx);
        float celly = (grid_max - grid_min).y() / float(ny);
        float cellz = (grid_max - grid_min).z() / float(nz);
        float diagonal = sqrt(cellx * cellx + celly * celly + cellz * cellz);
        for (int k = 0; k < nz; ++k)
        {
            for (int j = 0; j < ny; ++j)
            {
                for (int i = 0; i < nx; ++i)
                {
                    float x = (i + 0.5) * cellx;
                    float y = (j + 0.5) * celly;
                    float z = (k + 0.5) * cellz;
                    if ((Vec3f(x, y, z) - (center - grid_min)).Length() < radius + diagonal / 2)
                    {
                        int index = nx * ny * k + nx * j + i;
                        g->opaque[index].push_back(this);
                    }
                }
            }
        }
    }

private:
    Vec3f center; // 球的中心
    float radius; // 球的半径
};

#endif
