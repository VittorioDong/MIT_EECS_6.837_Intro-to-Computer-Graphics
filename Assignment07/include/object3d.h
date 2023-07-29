#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>

#include "boundingbox.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"
#include "ray.h"

class Grid;

// ############################################
// # Class Object3D
// ############################################
class Object3D
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    Object3D() : material(nullptr), boundingBox(nullptr) {}
    virtual ~Object3D() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() const = 0;
    virtual Material *getMaterial() const { return material; }
    virtual BoundingBox *getBoundingBox() const { return boundingBox; }
    virtual void insertIntoGrid(Grid *g, Matrix *m);
    virtual bool is_triangle() const { return false; } // use a special case for transformed triangles

    // ACCESSORS
    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const { return boundingBox; } // use a special case for transformed triangles

protected:
    Material *material;
    BoundingBox *boundingBox;
};

// ############################################
// # Class Group
// ############################################
class Group : public Object3D
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    Group(int n) : num_objects(n)
    {
        objects = new Object3D *[num_objects];
        boundingBox = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
    }
    ~Group() override
    {
        for (int i = 0; i < num_objects; ++i)
        {
            delete objects[i];
        }
        delete[] objects;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void addObject(int index, Object3D *obj);
    virtual void paint() const;
    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);
    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    int num_objects;
    Object3D **objects;
};

// ############################################
// # Class Sphere
// ############################################
class Sphere : public Object3D
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    Sphere(const Vec3f &c, float r, Material *m) : center(c), radius(r)
    {
        material = m;
        boundingBox = new BoundingBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }
    ~Sphere() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() const;
    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Vec3f center;
    float radius;
};

// ############################################
// # Class Plane
// ############################################
class Plane : public Object3D
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    Plane(const Vec3f &n, float d, Material *m) : normal(n), distance(d)
    {
        material = m;
        normal.Normalize();
    }
    ~Plane() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() const;
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    // ACCESSORS
    // Plane 没有边界，特殊处理，返回空指针 
    virtual BoundingBox *getBoundingBox() const { return nullptr; }

private:
    Vec3f normal;
    float distance;
};

// ############################################
// # Class Triangle
// ############################################
class Triangle : public Object3D
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *m) : a(a), b(b), c(c)
    {
        material = m;
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
        boundingBox = new BoundingBox(Vec3f(min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z())),
                                      Vec3f(max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z())));
        boundingBox->Extend(c);
        // debug
        // cout << "Triangle: " << boundingBox->getMin() << ", " << boundingBox->getMax() << endl;
    }
    ~Triangle() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() const;
    virtual void insertIntoGrid(Grid *g, Matrix *m);
    virtual bool is_triangle() const { return true; }

    // ACCESSORS
    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const;

private:
    Vec3f a, b, c;
    Vec3f normal;
};

// ############################################
// # Class Transform
// ############################################
class Transform : public Object3D
{
public: 
    // CONSTRUCTORS & DESTRUCTOR
    Transform(const Matrix &m, Object3D *o) : matrix(m), object(o)
    {
        material = object->getMaterial();
        boundingBox = object->getBoundingBox();
        // cout << "Transform: " << boundingBox->getMin() << ", " << boundingBox->getMax() << endl;
    }
    ~Transform() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() const;
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    // ACCESSORS
    virtual BoundingBox *getBoundingBox() const;

private:
    Matrix matrix;
    Object3D *object;
};

#endif