#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Camera.h"
#include "matrix.h"
#include "vectors.h"

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, int s)
        : center(c), size(s)
    {
        d.Normalize();
        this->direction = d;
        Vec3f::Cross3(this->horizontal, this->direction, u);
        this->horizontal.Normalize();
        Vec3f::Cross3(this->up, this->horizontal, this->direction);
    }

    virtual Ray generateRay(Vec2f point)
    {
        // Vec3f screenup = up - direction * up.Dot3(direction);
        // screenup.Normalize();
        // Vec3f horizontal;
        // Vec3f::Cross3(horizontal, direction, screenup);
        // horizontal.Normalize();

        // Vec3f origin = center + (point.x() - 0.5) * size * horizontal + (point.y() - 0.5) * size * up;
        // return Ray(origin, direction);
        float x = point.x();
        float y = point.y(); // normalized coordinate

        Vec3f screenup = up - direction * up.Dot3(direction);
        screenup.Normalize();
        Vec3f horizontal;
        Vec3f::Cross3(horizontal, direction, screenup);
        horizontal.Normalize();

        Vec3f origin = center + (x - 0.5) * size * horizontal + (y - 0.5) * size * screenup;
        Ray ray = Ray(origin, direction);
        return ray;
    }

    virtual float getTMin() const
    {
        return (float)(-3e7); // 一个接近负无穷的大数，表示射线起始于无穷远
    }

    // 为OpenGL添加函数
    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);

private:
    Vec3f center;     // 图像的中心
    Vec3f direction;  // 相机出射的方向
    Vec3f up;         // 垂直于平面的向上向量
    Vec3f horizontal; // 平面上的水平向量
    int size;         // 图像的尺寸
};

#endif