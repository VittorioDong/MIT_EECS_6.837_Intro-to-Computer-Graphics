#include "material.h"

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
  Vec3f camera_dir = ray.getDirection();
  Vec3f normal = hit.getNormal();
  if (normal.Dot3(camera_dir) > 0)
  {
    normal = -1 * normal;
  }
  camera_dir.Negate();
  camera_dir.Normalize();

  // the angular bisector
  Vec3f h = camera_dir + dirToLight;
  h.Normalize();

  // Blinn-Torrance Phong equation
  float diffuse = normal.Dot3(dirToLight);
  float specular = normal.Dot3(h);
#if !SPECULAR_FIX
  specular = (diffuse <= 0) ? 0 : specular;
  specular = pow(specular, exponent);
#else
  specular = pow(specular, exponent);
  specular *= max(diffuse, float(0));
#endif
  diffuse = (diffuse > 0) ? diffuse : 0;
  /* no ambient light and falloff is considered currently */
  Vec3f reflected_color = diffuse * diffuseColor * lightColor + specular * specularColor * lightColor;
  return reflected_color;
}

void PhongMaterial::glSetMaterial(void) const {

  GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat specular[4] = {
    getSpecularColor().r(),
    getSpecularColor().g(),
    getSpecularColor().b(),
    1.0};
  GLfloat diffuse[4] = {
    getDiffuseColor().r(),
    getDiffuseColor().g(),
    getDiffuseColor().b(),
    1.0};
  
  // NOTE: GL uses the Blinn Torrance version of Phong...      
  float glexponent = exponent;
  if (glexponent < 0) glexponent = 0;
  if (glexponent > 128) glexponent = 128;
  
#if !SPECULAR_FIX 
    
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  
#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight 
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    
  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}

// ====================================================================
// ====================================================================
