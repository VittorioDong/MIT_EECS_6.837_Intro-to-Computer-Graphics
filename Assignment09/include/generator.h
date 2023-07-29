#pragma once

#include "particle.h"
#include "random.h"

// ############################################
// # Class Generator
// ############################################
class Generator
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    Generator() { random = new Random(); }
    virtual ~Generator() { delete random; }

    // INITIALIZE
    void SetColors(const Vec3f &color, const Vec3f &dead_color, float color_randomness);
    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles);
    void SetMass(float mass, float mass_randomness);

    // PURE VIRTUAL METHOD
    // 在每个时间步上，创建一些粒子
    virtual int numNewParticles(float current_time, float dt) const = 0;
    virtual Particle *Generate(float current_time, int i) = 0;

    // GUI
    virtual void Paint() const {}
    // 为类重新播种随机数生成器，以确保粒子模拟完全可再现
    void Restart()
    {
        delete random;
        random = new Random;
    }

protected:
    Vec3f color;
    Vec3f dead_color;
    float color_randomness;
    float lifespan;
    float lifespan_randomness;
    int desired_num_particles;
    float mass;
    float mass_randomness;
    Random *random;
};

// ############################################
// # Class HoseGenerator
// ############################################
class HoseGenerator : public Generator
{
public:
    // CONSTRUCTOR
    HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity, float velocity_randomness) : position(position), position_randomness(position_randomness), velocity(velocity), velocity_randomness(velocity_randomness) {}

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

private:
    Vec3f position;
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
};

// ############################################
// # Class RingGenerator
// ############################################
class RingGenerator : public Generator
{
public:
    // CONSTRUCTOR
    RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) : position_randomness(position_randomness), velocity(velocity), velocity_randomness(velocity_randomness) {}

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

private:
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
};
