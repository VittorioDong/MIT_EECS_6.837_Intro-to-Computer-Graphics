#include "integrator.h"

// ############################################
// # Class EulerIntegrator
// ############################################
void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f a = forcefield->getAcceleration(particle->getPosition(), particle->getMass(), t);
    Vec3f new_position = particle->getPosition() + particle->getVelocity() * dt;
    Vec3f new_velocity = particle->getVelocity() + a * dt;

    particle->setPosition(new_position);
    particle->setVelocity(new_velocity);
    particle->increaseAge(dt);
}

// ############################################
// # Class MidpointIntegrator
// ############################################
void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f a = forcefield->getAcceleration(particle->getPosition(), particle->getMass(), t);
    Vec3f pm = particle->getPosition() + particle->getVelocity() * (dt / 2);
    Vec3f vm = particle->getVelocity() + a * (dt / 2);

    Vec3f am = forcefield->getAcceleration(pm, particle->getMass(), t + dt / 2.0);
    Vec3f new_position = particle->getPosition() + vm * dt;
    Vec3f new_velocity = particle->getVelocity() + am * dt;
    
    particle->setPosition(new_position);
    particle->setVelocity(new_velocity);
    particle->increaseAge(dt);
}

// ############################################
// # Class RungeKuttaIntegrator
// ############################################
void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    
}
