#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "vectors.h"
#include "perlin_noise.h"
#include <cmath>

// ############################################
// # Class ForceField
// ############################################
class ForceField
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    ForceField() {}
    virtual ~ForceField() {}

    // PURE VIRTUAL METHOD
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

// ############################################
// # Class GravityForceField
// ############################################
class GravityForceField : public ForceField
{
public:
    // CONSTRUCTOR
    GravityForceField(const Vec3f &gravity) : gravity(gravity) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f gravity;
};

// ############################################
// # Class ConstantForceField
// ############################################
class ConstantForceField : public ForceField
{
public:
    // CONSTRUCTOR
    ConstantForceField(const Vec3f &force) : force(force) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f force;
};

// ############################################
// # Class RadialForceField
// ############################################
class RadialForceField : public ForceField
{
public:
    // CONSTRUCTOR
    RadialForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

// ############################################
// # Class VerticalForceField
// ############################################
class VerticalForceField : public ForceField
{
public:
    // CONSTRUCTOR
    VerticalForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

// ############################################
// # Class WindForceField
// ############################################
class WindForceField : public ForceField
{
public:
    // CONSTRUCTOR
    WindForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

#endif