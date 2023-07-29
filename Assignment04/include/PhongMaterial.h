#ifndef PHONEMATERIAL_H
#define PHONEMATERIAL_H

#include "vectors.h"
#include "material.h"
#include "ray.h"
#include "hit.h"

class PhongMaterial : public Material {
private:
    float exponent;
    
public:
    PhongMaterial(const Vec3f &_diffuseColor, const Vec3f &_specularColor, float _exponent,
                  const Vec3f &_reflectiveColor, const Vec3f &_transparentColor,
                  float _indexOfRefraction)
    {
        diffuseColor = _diffuseColor;
        specularColor = _specularColor;
        exponent = _exponent;
        reflectiveColor = _reflectiveColor;
        transparentColor = _transparentColor;
        indexOfRefraction = _indexOfRefraction;
    }

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const ;

    void glSetMaterial(void) const;
};

#endif