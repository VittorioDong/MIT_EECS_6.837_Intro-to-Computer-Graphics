#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/gl.h>

#include "vectors.h"
#include "ray.h"
#include "hit.h"

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight 
// artifact for small specular exponents (wide specular lobe)
// ====================================================================

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"  

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  Material() {}
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const { return specularColor; }
  virtual Vec3f getReflectiveColor() const { return reflectiveColor; }
  virtual Vec3f getTransparentColor() const { return transparentColor; }
  virtual float getIndexOfReflection() const { return indexOfRefraction; }

  // virtual method
  // add in Assignment03
  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;

  // for OpenGL 
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f specularColor;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
private:
    float exponent;
    
public:
  PhongMaterial(const Vec3f &d_color) : Material(d_color), exponent(0) { indexOfRefraction = 0; }

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
