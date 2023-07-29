#include "rayTracer.h"
#include "light.h"
#include "rayTree.h"
#include "raytracing_stats.h"

extern bool shadows;
extern bool shade_back;
extern int nx, ny, nz;

// 构造函数
RayTracer::RayTracer(SceneParser *s, int max_bounces, float cutoff_weight)
    : scene(s), maxBounces(max_bounces), cutoffWeight(cutoff_weight)
{
    if (nx != 0)
    {
        grid = new Grid(s->getGroup()->getBoundingBox(), nx, ny, nz);
        s->getGroup()->insertIntoGrid(grid, nullptr);
    }
    else
        grid = nullptr;
}

// 析构函数
RayTracer::~RayTracer()
{
    delete grid;
}

// 追踪光线
Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit)
{
    if (grid)
    {
        // debug
        // cout << "ray = " << ray << endl;
        // cout << "hit = " << hit << endl;
        // cout << "tmin = " << tmin << endl;
        // 网格渲染
        return RayCastFast(ray, tmin, bounces, weight, hit);
    }
    else
    {
        // 普通渲染
        return RayCast(ray, tmin, bounces, weight, hit);
    }
}

// 普通渲染
Vec3f RayTracer::RayCast(Ray &ray, float tmin, int bounces, float weight, Hit &hit)
{
    Group *group = scene->getGroup();
    if (group->intersect(ray, hit, tmin))
    {
        if (bounces == 0)
        {
            RayTree::SetMainSegment(ray, tmin, hit.getT()); // 增加主光线
        }
        Vec3f pixel_color(0.0, 0.0, 0.0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();
        pixel_color += scene->getAmbientLight() * material->getDiffuseColor();

        for (int k = 0; k < scene->getNumLights(); k++)
        {
            Light *light = scene->getLight(k);
            Vec3f l, lightColor;
            float dis;
            light->getIllumination(pos, l, lightColor, dis);
            // cout << "lightColor = " << lightColor << endl;
            //  shadows
            Ray ray_shadows(pos, l);
            Hit hit_shadows(dis);
            if (shadows)
            {
                RayTracingStats::IncrementNumShadowRays(); // 增加阴影光线数量
                if (!group->intersectShadowRay(ray_shadows, hit_shadows, tmin))
                    pixel_color += material->Shade(ray, hit, l, lightColor);
            }
            else
                pixel_color += material->Shade(ray, hit, l, lightColor);
            RayTree::AddShadowSegment(ray_shadows, tmin, hit_shadows.getT()); // 增加阴影光线
        }
        // 反射
        Ray ref_ray;
        Vec3f decay;
        if (material->reflect(ray, hit, decay, ref_ray) && bounces < maxBounces && weight > cutoffWeight)
        {
            Hit hit_ref(INFINITY);
            pixel_color += decay * RayCast(ref_ray, tmin, bounces + 1, weight * decay.Length(), hit_ref);
            RayTree::AddReflectedSegment(ref_ray, tmin, hit_ref.getT()); // 增加反射光线
        }
        // 折射
        if (material->refract(ray, hit, decay, ref_ray) && bounces < maxBounces && weight > cutoffWeight)
        {
            Hit hit_ref(INFINITY);
            pixel_color += decay * RayCast(ref_ray, tmin, bounces + 1, weight * decay.Length(), hit_ref);
            RayTree::AddTransmittedSegment(ref_ray, tmin, hit_ref.getT()); // 增加折射光线
        }
        return pixel_color;
    }
    else
    {
        // 如果不相交，则返回背景颜色
        return scene->getBackgroundColor();
    }
}

// 网格渲染
Vec3f RayTracer::RayCastFast(Ray &ray, float tmin, int bounces, float weight, Hit &hit)
{
    if (grid->intersect(ray, hit, tmin))
    {
        Vec3f pixel_color(0.0, 0.0, 0.0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();
        pixel_color += scene->getAmbientLight() * material->getDiffuseColor();
        for (int k = 0; k < scene->getNumLights(); k++)
        {
            Light *light = scene->getLight(k);
            Vec3f l, lightColor;
            float dis;
            light->getIllumination(pos, l, lightColor, dis);

            if (shadows)
            {
                Ray ray_shadows(pos, l);
                // debug
                // printf("ray_shadows.getDirection().x() = %f, ray_shadows.getOrigin().y() = %f\n",ray_shadows.getDirection().x(), ray_shadows.getDirection().y());
                Hit hit_shadows(dis);
                RayTracingStats::IncrementNumShadowRays(); // 增加阴影线数量
                if (!grid->intersectShadowRay(ray_shadows, hit_shadows, tmin))
                    pixel_color += material->Shade(ray, hit, l, lightColor);
            }
            else
                pixel_color += material->Shade(ray, hit, l, lightColor);
        }
        // 反射
        Ray ref_ray;
        Vec3f decay;
        if (bounces < maxBounces && weight > cutoffWeight && material->reflect(ray, hit, decay, ref_ray))
        {
            Hit hit_ref(INFINITY);
            pixel_color += decay * RayCastFast(ref_ray, tmin, bounces + 1, weight * decay.Length(), hit_ref);
            RayTree::AddReflectedSegment(ref_ray, tmin, hit_ref.getT()); // 增加反射光线
        }
        // 折射
        if (bounces < maxBounces && weight > cutoffWeight && material->refract(ray, hit, decay, ref_ray))
        {
            Hit hit_ref(INFINITY);
            pixel_color += decay * RayCastFast(ref_ray, tmin, bounces + 1, weight * decay.Length(), hit_ref);
            RayTree::AddTransmittedSegment(ref_ray, tmin, hit_ref.getT()); // 增加折射光线
        }
        return pixel_color;
    }
    else
    {
        // 如果不相交，则返回背景颜色
        return scene->getBackgroundColor();
    }
}
