#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Group.h"
#include "Hit.h"
#include "Ray.h"
#include "ray.h"
#include "scene_parser.h"
#include "vectors.h"
#include "rayTree.h"
#include "light.h"

#define epsilon 1e-2    // small constant for self shadowing

class RayTracer
{
public:
    // 构造函数
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows)
        : scene(s), maxBounces(max_bounces), cutoffWeight(cutoff_weight), shadows(shadows) {}

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
                              float indexOfRefraction, Hit &hit) const
    {
        if (bounces > maxBounces || weight < cutoffWeight)
        {
            return Vec3f(0, 0, 0);
        }

        Group *objs = scene->getGroup();
        bool intersected = objs->intersect(ray, hit, tmin);
        if (!intersected)
            return scene->getBackgroundColor();
        if (bounces == 0)
            RayTree::SetMainSegment(ray, 0, hit.getT()); // for debug

        /* Phong shading */
        assert(hit.getMaterial() != NULL);
        Vec3f direction, light_color;
        Vec3f object_color = hit.getMaterial()->getDiffuseColor();
        Vec3f pixel_color = scene->getAmbientLight() * object_color;
        Light *current_light;
        for (int l = 0; l < scene->getNumLights(); ++l)
        {
            current_light = scene->getLight(l);
            bool has_shadow = false;

            /* compute shadow */
            if (shadows)
            {
                Vec3f light_dir, dummy_color;
                float dist;
                current_light->getIllumination(hit.getIntersectionPoint(), light_dir, dummy_color, dist);
                Ray shadow_ray = Ray(hit.getIntersectionPoint(), light_dir);
                Hit shadow_hit(dist, NULL, Vec3f(0, 0, 0));
                has_shadow = objs->intersectShadowRay(shadow_ray, shadow_hit, epsilon);
                RayTree::AddShadowSegment(shadow_ray, 0, shadow_hit.getT());
            }

            if (!has_shadow)
            {
                float dummy_dist;
                current_light->getIllumination(hit.getIntersectionPoint(), direction, light_color, dummy_dist);
                Vec3f phong_color = hit.getMaterial()->Shade(ray, hit, direction, light_color);
                pixel_color += phong_color;
            }
        }

        /* tracing for reflection */
        if (hit.getMaterial()->getReflectiveColor() != Vec3f(0, 0, 0))
        {
            Vec3f normal = hit.getNormal();
            Vec3f reflect_direction = mirrorDirection(normal, ray.getDirection());
            Ray reflected_ray(hit.getIntersectionPoint(), reflect_direction);
            Hit recursive_hit(INFINITY, NULL, Vec3f(0, 0, 0));
            Vec3f reflect_color = traceRay(reflected_ray, epsilon, bounces + 1,
                                           weight * hit.getMaterial()->getReflectiveColor().Length(),
                                           indexOfRefraction, recursive_hit);
            pixel_color += reflect_color * hit.getMaterial()->getReflectiveColor();
            RayTree::AddReflectedSegment(reflected_ray, 0, recursive_hit.getT());
        }

        /* tracing for refraction */
        if (hit.getMaterial()->getTransparentColor() != Vec3f(0, 0, 0))
        {
            Vec3f normal = hit.getNormal();
            float index_i, index_t;
            if (normal.Dot3(ray.getDirection()) > 0)
            {
                normal.Negate();
                index_i = hit.getMaterial()->getIndexOfReflection();
                index_t = 1;
            }
            else
            {
                index_i = 1;
                index_t = hit.getMaterial()->getIndexOfReflection();
            }
            Vec3f refracted_dir;
            bool internal_reflect = transmittedDirection(normal, ray.getDirection(), index_i, index_t, refracted_dir);
            if (!internal_reflect)
            {
                Ray refracted_ray(hit.getIntersectionPoint(), refracted_dir);
                Hit refract_hit(INFINITY, NULL, Vec3f(0, 0, 0));
                Vec3f refract_color = traceRay(refracted_ray, epsilon, bounces + 1,
                                               weight * hit.getMaterial()->getTransparentColor().Length(),
                                               index_t, refract_hit);
                pixel_color += refract_color * hit.getMaterial()->getTransparentColor();
                RayTree::AddTransmittedSegment(refracted_ray, 0, refract_hit.getT());
            }
        }

        return pixel_color;
    }

    // // 计算射线的颜色
    // Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
    //                float indexOfRefraction, Hit &hit) const
    // {
    //     if (bounces > maxBounces || weight < cutoffWeight)
    //     {
    //         // 终止条件：超过最大反弹数或权重过低
    //         return Vec3f(0, 0, 0); // 返回黑色
    //     }

    //     // 找到与射线最近的相交物体
    //     if (scene->getGroup()->intersect(ray, hit, tmin))
    //     {
    //         // 设置主光线
    //         if (bounces == 0) RayTree::SetMainSegment(ray, 0, hit.getT());
    //         // 获取相交点的材质
    //         assert (hit.getMaterial() != NULL);
    //         Material *material = hit.getMaterial();
    //         // 计算 Phong 着色
    //         Vec3f direction, lightColor;
    //         Vec3f objectColor = material->getDiffuseColor();
    //         Vec3f pixelColor = scene->getAmbientLight() * objectColor;
    //         Light *currentLight;
    //         for (int i = 0; i < scene->getNumLights(); i++)
    //         {
    //             currentLight = scene->getLight(i);
    //             bool has_shadow = false;

    //             // 计算阴影
    //             if (shadows)
    //             {
    //                 Vec3f lightDir, dummyColor;
    //                 float dist;
    //                 currentLight->getIllumination(hit.getIntersectionPoint(), lightDir, dummyColor, dist);
    //                 Ray shadowRay = Ray(hit.getIntersectionPoint(), lightDir);
    //                 Hit shadowHit(dist, NULL, Vec3f(0, 0, 0));
    //                 has_shadow = scene->getGroup()->intersectShadowRay(shadowRay, shadowHit, epsilon);
    //                 // 设置阴影光线
    //                 RayTree::AddShadowSegment(shadowRay, 0, shadowHit.getT());
    //             }

    //             // 该光源没有被遮挡
    //             if (!has_shadow)
    //             {
    //                 float dummyDist;
    //                 currentLight->getIllumination(hit.getIntersectionPoint(), direction, lightColor, dummyDist);
    //                 Vec3f phongColor = hit.getMaterial()->Shade(ray, hit, direction, lightColor);
    //                 pixelColor += phongColor;
    //             }
    //         }

    //         // 反射追踪
    //         if (hit.getMaterial()->getReflectiveColor() != Vec3f(0, 0, 0))
    //         {
    //             const Vec3f &reflectiveColor = hit.getMaterial()->getReflectiveColor();

    //             Vec3f mirrorDir = mirrorDirection(hit.getNormal(), ray.getDirection());
    //             Ray mirrorRay(hit.getIntersectionPoint(), mirrorDir); // 反射光线

    //             Hit mirrorHit(INFINITY, NULL, Vec3f(0, 0, 0));

    //             // 递归，深度加1，权重乘以反射颜色的大小
    //             Vec3f mirrorColor = traceRay(mirrorRay, epsilon, bounces + 1, weight * reflectiveColor.Length(),
    //                                          indexOfRefraction, mirrorHit);

    //             pixelColor += mirrorColor * reflectiveColor;
    //             // 设置反射光线
    //             RayTree::AddReflectedSegment(mirrorRay, 0, mirrorHit.getT());
    //         }

    //         // 折射追踪
    //         if (hit.getMaterial()->getTransparentColor() != Vec3f(0, 0, 0))
    //         {
    //             Vec3f normal = hit.getNormal();
    //             float index_i, index_t;
    //             if (normal.Dot3(ray.getDirection()) > 0)
    //             {
    //                 normal.Negate();
    //                 index_i = hit.getMaterial()->getIndexOfReflection();
    //                 index_t = 1;
    //             }
    //             else
    //             {
    //                 index_i = 1;
    //                 index_t = hit.getMaterial()->getIndexOfReflection();
    //             }
    //             Vec3f refractedDir;
    //             bool internalReflect = transmittedDirection(normal, ray.getDirection(), index_i, index_t, refractedDir);
    //             if (!internalReflect)
    //             {
    //                 Ray refractedRay(hit.getIntersectionPoint(), refractedDir);
    //                 Hit refractHit(1e30, NULL, Vec3f(0, 0, 0));
    //                 Vec3f refractColor = traceRay(refractedRay, epsilon, bounces + 1,
    //                                                weight * hit.getMaterial()->getTransparentColor().Length(),
    //                                                index_t, refractHit);
    //                 pixelColor += refractColor * hit.getMaterial()->getTransparentColor();
    //                 // 设置折射光线
    //                 RayTree::AddTransmittedSegment(refractedRay, 0, refractHit.getT());
    //             }
    //         }

    //         return pixelColor;
    //     }
    //     else
    //     {
    //         // 如果没有相交，返回背景色
    //         return scene->getBackgroundColor();
    //     }
    // }

    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const
    {
        Vec3f mirrorDir = incoming - 2 * incoming.Dot3(normal) * normal;
        mirrorDir.Normalize();
        return mirrorDir;
    }

    bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
                              float index_i, float index_t, Vec3f &transmitted) const
    {

        Vec3f N = normal;
        float ratio = index_i / index_t;
        Vec3f I = (-1) * incoming;
        I.Normalize();
        float temp = 1 - ratio * ratio * (1 - N.Dot3(I) * N.Dot3(I));

        // 检查全反射
        if (temp < 0)
        {
            transmitted = incoming - 2 * incoming.Dot3(N) * N;
            transmitted.Normalize();
            return true;
        }
        transmitted = (ratio * N.Dot3(I) - sqrt(temp)) * N - ratio * I;
        transmitted.Normalize();
        return false;
    }

private:
    SceneParser *scene;
    int maxBounces;
    float cutoffWeight;
    bool shadows;
};

#endif