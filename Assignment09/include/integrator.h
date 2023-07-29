#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "forceField.h"
#include "particle.h"

// ############################################
// # Class Integrator
// ############################################
class Integrator
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    Integrator() {}
    virtual ~Integrator() {}

    // PURE VIRTUAL METHOD
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    virtual Vec3f getColor() const = 0;
};

// ############################################
// # Class EulerIntegrator
// ############################################
class EulerIntegrator : public Integrator
{
public:
    // CONSTRUCTOR
    EulerIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
    Vec3f getColor() const override { return Vec3f(1, 0, 0); }
};

// ############################################
// # Class MidpointIntegrator
// ############################################
class MidpointIntegrator : public Integrator
{
public:
    // CONSTRUCTOR
    MidpointIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
    Vec3f getColor() const override { return Vec3f(0, 1, 0); }
};

// ############################################
// # Class RungeKuttaIntegrator
// ############################################
class RungeKuttaIntegrator : public Integrator
{
public:
    // CONSTRUCTOR
    RungeKuttaIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
    Vec3f getColor() const override { return Vec3f(0, 0, 1); }
};

#endif