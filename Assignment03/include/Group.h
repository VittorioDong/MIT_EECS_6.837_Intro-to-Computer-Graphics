#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "vectors.h"

class Group : public Object3D {
public:
    // 构造函数
    Group(int num_objects) 
        : num_objects(num_objects), objects(new Object3D*[num_objects]) {}

    // 析构函数
    virtual ~Group() {
        for (int i = 0; i < num_objects; i++) {
            delete objects[i];
        }
        delete[] objects;
    }

    // 添加对象的方法
    void addObject(int index, Object3D* obj) {
        assert(index >= 0 && index < num_objects);
        objects[index] = obj;
    }

    // 实现交集方法
    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
        bool intersect = false;
        for (int i = 0; i < num_objects; i++) {
            if (objects[i]->intersect(r, h, tmin)) {
                intersect = true;
            }
        }
        return intersect;
    }
    
    // 实现 paint 方法
    virtual void paint(void)
    {
        for (int i = 0; i < num_objects; ++i)
        {
            objects[i]->paint();
        }
    }

private:
    int num_objects; // 组内对象的数量
    Object3D** objects; // 对象数组
};

#endif