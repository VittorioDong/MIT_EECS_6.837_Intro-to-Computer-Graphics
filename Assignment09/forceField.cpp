#include "forceField.h"

// ############################################
// # Class GravityForceField
// ############################################
Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    // F=ma, therefore a=F/m. Here, the force is the gravity itself.
    return gravity;
}

// ############################################
// # Class ConstantForceField
// ############################################
Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    // F=ma, therefore a=F/m.
    return force * (1 / mass);
}

// ############################################
// # Class RadialForceField
// ############################################
Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    // The direction of the force is towards the origin
    Vec3f direction = Vec3f(-position.x(), -position.y(), -position.z());
    direction.Normalize();
    // The magnitude of the force is proportional to the distance from the origin
    float distance = position.Length();
    // F=ma, therefore a=F/m.
    return direction * magnitude * distance * (1 / mass);
}

// ############################################
// # Class VerticalForceField
// ############################################
Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    // The direction of the force is towards the y=0 plane
    Vec3f direction(0, -position.y(), 0);
    direction.Normalize();
    // The magnitude of the force is proportional to the distance from the plane
    float distance = fabs(position.y());
    // F=ma, therefore a=F/m.
    return direction * magnitude * distance * (1 / mass);
}

// ############################################
// # Class WindForceField
// ############################################
Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    // Perlin Noise
    // Use the noise generator to generate a noise value based on position and time
    double noiseValue = PerlinNoise::noise(t * position.x(), t * position.y(), t * position.z()) / t;
    // Map the noise value to a reasonable range, e.g. [-1, 1]
    float mappedNoiseValue = 3 * noiseValue;
    //printf("mappedNoiseValue = %f\n", mappedNoiseValue);
    // Compute the wind force using the noise value and the given magnitude
    Vec3f force = magnitude * mappedNoiseValue * Vec3f(1, 0, 0);

    // It generates a force that changes periodically with time, making the simulated wind field have a dynamic effect
    // Vec3f force = magnitude * Vec3f(sin(8 * t), cos(8 * t), 0);
    return force * (1 / mass);
}
