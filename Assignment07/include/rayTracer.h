#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene_parser.h"
#include "object3d.h"
#include "grid.h"

// ############################################
// # Class RayTracer
// ############################################
class RayTracer {
public:
    // 构造函数
    RayTracer(SceneParser *s, int max_bounces, float min_weight);

    // 析构函数
    ~RayTracer();

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    Grid *getGrid() { return grid; }

    Vec3f RayCast(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    Vec3f RayCastFast(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

private:
    SceneParser *scene;
    int max_bounces;
    float min_weight;
    Grid *grid;
};

#endif

