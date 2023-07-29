#ifndef PLANE_H
#define PLANE_H

class Plane : public Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m) 
        : Object3D(m), normal(normal), d(d) {
        // 对法向量进行归一化处理
        this->normal.Normalize();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        // 计算射线和平面的交点，公式为 (P0 + t*dir) . normal = d
        // 这里的P0为射线的起点，dir为射线的方向，t为参数，我们需要求解t
        // 首先计算射线的方向和法线的点积
        float dot = r.getDirection().Dot3(normal);
        if (fabs(dot) < 1e-6) {
            // 如果射线的方向和法线平行，那么它们不会相交
            return false;
        } else {
            // 否则求解t
            float t = (d - normal.Dot3(r.getOrigin())) / dot;
            if (t >= tmin && t < h.getT()) {
                // 如果求解出的t在有效范围内，更新Hit
                h.set(t, material, normal, r);
                return true;
            }
        }
        return false;
    }

private:
    Vec3f normal;
    float d;
};

#endif