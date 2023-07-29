#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "vectors.h"
#include "Camera.h"
#include "matrix.h"

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
        // Vec3f screenup = up - direction * up.Dot3(direction);
        // screenup.Normalize();
        // Vec3f horizontal;
        // Vec3f::Cross3(horizontal, direction, screenup);
        // horizontal.Normalize();
        // Vec3f virtual_center = center + direction * distance;
        // Vec3f virtual_point = virtual_center + (point.x() - 0.5) * horizontal + (point.y() - 0.5) * screenup;
        // Vec3f rayDir = virtual_point - center;

        // rayDir.Normalize();

        // // 生成Ray，注意Ray的起点是camera的中心，方向是刚才计算的rayDir
        // return Ray(center, rayDir);

        float x = point.x();
        float y = point.y(); // normalized coordinate

        Vec3f screenup = up - direction * up.Dot3(direction);
        screenup.Normalize();
        Vec3f horizontal;
        Vec3f::Cross3(horizontal, direction, screenup);
        horizontal.Normalize();
        Vec3f virtual_center = center + direction * (0.5 / tan(0.5 * angle));
        Vec3f virtual_point = virtual_center + (x - 0.5) * horizontal + (y - 0.5) * screenup;
        Vec3f ray_direction = virtual_point - center;
        ray_direction.Normalize();
        Ray ray = Ray(center, ray_direction);
        return ray;
    }

    virtual float getTMin() const {
        // 对于透视相机，Ray应该从相机的中心点开始，所以tmin应该是0
        return (float)(0.0);
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