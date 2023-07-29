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

protected:
    Object3D* o;
    Matrix m, m_inv, m_inv_tran;
};

#endif