#ifndef CAMERA_H
#define CAMERA_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "ray.h"
#include "matrix.h"

class Camera {
public:
    // 构造函数和析构函数
    Camera() {}
    virtual ~Camera() {}

    // 纯虚函数
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

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


class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle) {
        // 对输入的向量进行归一化处理
        direction.Normalize();
        up.Normalize();

        this->center = center;
        this->direction = direction;
        this->angle = angle;

        // 为了创建一个正交坐标系，我们需要一个与direction垂直，与up方向尽可能接近的向量。
        // 叉乘运算可以实现这一点
        Vec3f::Cross3(this->horizontal, direction, up);
        this->horizontal.Normalize();

        // 再次叉乘，得到新的up向量，这保证了horizontal, up, direction三个向量互相正交
        Vec3f::Cross3(this->up, this->horizontal, this->direction);

        // 计算焦距（image plane到center的距离），这可以通过角度和简单的三角函数得到
        this->distance = 0.5f / tan(angle/2.0f);
    }

    virtual Ray generateRay(Vec2f point) {
        // 通过屏幕坐标计算出Ray的方向
        Vec3f screenup = up - direction * up.Dot3(direction);
        screenup.Normalize();
        Vec3f horizontal;
        Vec3f::Cross3(horizontal, direction, screenup);
        horizontal.Normalize();
        Vec3f virtual_center = center + direction * distance;
        Vec3f virtual_point = virtual_center + (point.x() - 0.5) * horizontal + (point.y() - 0.5) * screenup;
        Vec3f rayDir = virtual_point - center;

        rayDir.Normalize();

        // 生成Ray，注意Ray的起点是camera的中心，方向是刚才计算的rayDir
        return Ray(center, rayDir);
    }

    virtual float getTMin() const {
        // 对于透视相机，Ray应该从相机的中心点开始，所以tmin应该是0
        return 0.0f;
    }

    // 为OpenGL添加函数
    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);


private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float distance;
    float angle;
};


#endif