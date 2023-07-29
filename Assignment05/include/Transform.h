#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Object3D.h"
#include "matrix.h"

class Transform: public Object3D {
public:
    Transform(Matrix &m, Object3D *o)
        : o(o), m(m) {
        m.Inverse(m_inv);
        m_inv.Transpose(m_inv_tran);
        boundingBox = o->getBoundingBox();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        // Transform the ray 
        Vec3f origin_os = r.getOrigin();
        Vec3f dir_os = r.getDirection();
        m_inv.Transform(origin_os);
        m_inv.TransformDirection(dir_os);

        Ray tr = Ray(origin_os, dir_os);
        if (o->intersect(tr, h, tmin))
        {
            // Transform normal
            Vec3f normal_ws = h.getNormal();
            m_inv_tran.TransformDirection(normal_ws);
            normal_ws.Normalize();
            h.set(h.getT(), h.getMaterial(), normal_ws, r);
            return true;
        }
        else return false;
        
    }

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin)
    {
        return intersect(r, h, tmin);
    }

    void paint(void)
    {
        // Mat -> glSetMaterial();
        glPushMatrix();
        GLfloat *glMatrix = m.glGet();
        glMultMatrixf(glMatrix);
        delete[] glMatrix;
        o->paint();
        glPopMatrix();
    }

    BoundingBox *getBoundingBox()
    {
        assert(boundingBox != nullptr);
        if (o->is_triangle())
        {
            return o->getTriangleBoundingBox(&m);
        }
        Vec3f obj_min = boundingBox->getMin();
        Vec3f obj_max = boundingBox->getMax();
        float x1 = obj_min.x(), y1 = obj_min.y(), z1 = obj_min.z();
        float x2 = obj_max.x(), y2 = obj_max.y(), z2 = obj_max.z();
        Vec3f v1 = Vec3f(x1, y1, z1);
        Vec3f v2 = Vec3f(x1, y1, z2);
        Vec3f v3 = Vec3f(x1, y2, z1);
        Vec3f v4 = Vec3f(x1, y2, z2);
        Vec3f v5 = Vec3f(x2, y1, z1);
        Vec3f v6 = Vec3f(x2, y1, z2);
        Vec3f v7 = Vec3f(x2, y2, z1);
        Vec3f v8 = Vec3f(x2, y2, z2);
        m.Transform(v1);
        m.Transform(v2);
        m.Transform(v3);
        m.Transform(v4);
        m.Transform(v5);
        m.Transform(v6);
        m.Transform(v7);
        m.Transform(v8);

        BoundingBox *bb_new = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
        bb_new->Extend(v1);
        bb_new->Extend(v2);
        bb_new->Extend(v3);
        bb_new->Extend(v4);
        bb_new->Extend(v5);
        bb_new->Extend(v6);
        bb_new->Extend(v7);
        bb_new->Extend(v8);
        return bb_new;
    }

    void insertIntoGrid(Grid *g, Matrix *_m)
    {
        if (_m)
        {
            Matrix t = (*_m) * m;
            o->insertIntoGrid(g, &t);
        }
        else
        {
            o->insertIntoGrid(g, &m);
        }
    }

protected:
    Object3D* o;
    Matrix m, m_inv, m_inv_tran;
};

#endif