#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Camera.h"
#include "vectors.h"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, int s)
        : center(c), size(s) {
        d.Normalize();
        this->direction = d;
        Vec3f::Cross3(this->horizontal, this->direction, u);
        this->horizontal.Normalize();
        Vec3f::Cross3(this->up, this->horizontal, this->direction);
        // debug
        // cout << "direction = " << "(" << direction.x() << "," << direction.y() << "," << direction.z() << ")" << endl;
        // cout << "horizontal = " << "(" << horizontal.x() << "," << horizontal.y() << "," << horizontal.z() << ")" << endl;
        // cout << "up = " << "(" << up.x() << "," << up.y() << "," << up.z() << ")" << endl;
    }

    virtual Ray generateRay(Vec2f point) {
        Vec3f origin = center + (point.x() - 0.5) * size * horizontal + (point.y() - 0.5) * size * up;
        return Ray(origin, direction);
    }

    virtual float getTMin() const {
        return -1.0e30; // 一个接近负无穷的大数，表示射线起始于无穷远
    }

    // Vec3f getCenter(){return center;}
    // Vec3f getDirection(){return direction;}
    // Vec3f getUp(){return up;}
    // Vec3f getHorizontal(){return horizontal;}

private:
    Vec3f center; // 图像的中心
    Vec3f direction; // 相机出射的方向
    Vec3f up; // 垂直于平面的向上向量
    Vec3f horizontal; // 平面上的水平向量
    int size; // 图像的尺寸
};

#endif