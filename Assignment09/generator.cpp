#include "generator.h"
#include <GL/freeglut.h>

// ############################################
// # Class Generator
// ############################################
void Generator::SetColors(const Vec3f &color, const Vec3f &dead_color, float color_randomness)
{
    this->color = color;
    this->dead_color = dead_color;
    this->color_randomness = color_randomness;
}

void Generator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
{
    this->lifespan = lifespan;
    this->lifespan_randomness = lifespan_randomness;
    this->desired_num_particles = desired_num_particles;
}

void Generator::SetMass(float mass, float mass_randomness)
{
    this->mass = mass;
    this->mass_randomness = mass_randomness;
}

// ############################################
// # Class HoseGenerator
// ############################################
int HoseGenerator::numNewParticles(float current_time, float dt) const
{
    return desired_num_particles * dt / lifespan;
}

Particle *HoseGenerator::Generate(float current_time, int i)
{
    Vec3f p = position + position_randomness * random->randomVector();
    Vec3f v = velocity + velocity_randomness * random->randomVector();
    Vec3f c = color + color_randomness * random->randomVector();
    Vec3f dead_c = dead_color + color_randomness * random->randomVector();
    float m = mass + mass_randomness * random->next();
    float l = lifespan + lifespan_randomness * random->next();
    Particle *particle = new Particle(p, v, c, dead_c, m, l);
    return particle;
}

// ############################################
// # Class RingGenerator
// ############################################
int RingGenerator::numNewParticles(float current_time, float dt) const
{   
    // 为了在模拟开始阶段逐渐增加粒子的生成，从而更平滑地开始模拟
    return (current_time < 3.5 ? current_time / 3.5 : 1) * desired_num_particles;
}

Particle *RingGenerator::Generate(float current_time, int i)
{
    float radius = current_time < 3.5 ? current_time : 3.5;
    Vec3f p = Vec3f(radius * sin(i) + 0.2 * random->next(), -4, radius * cos(i) + 0.2 * random->next());
    Vec3f v = velocity + velocity_randomness * random->randomVector();
    Vec3f c = color + color_randomness * random->randomVector();
    Vec3f dead_c = dead_color + color_randomness * random->randomVector();
    float m = mass + mass_randomness * random->next();
    float l = lifespan + lifespan_randomness * random->next();
    Particle *particle = new Particle(p, v, c, dead_c, m, l);
    return particle;
}

void RingGenerator::Paint() const
{
    //绘制地面
    int size = 6;
    glColor3f(0.27, 0.27, 0.27);
    glBegin(GL_QUADS);
    glVertex3f(size, -4, size);
    glVertex3f(size, -4, -size);
    glVertex3f(-size, -4, -size);
    glVertex3f(-size, -4, size);
    glEnd();
}
