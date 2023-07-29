#ifndef PHONEMATERIAL_H
#define PHONEMATERIAL_H

#include "vectors.h"
#include "material.h"
#include "ray.h"
#include "hit.h"



class PhongMaterial : public Material {
private:
    Vec3f specularColor;
    float exponent;

public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent)
        : Material(diffuseColor), specularColor(specularColor), exponent(exponent) {}

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const ;

    virtual Vec3f getSpecularColor() const { return specularColor; }

    void glSetMaterial(void) const;
};

#endif