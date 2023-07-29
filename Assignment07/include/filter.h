#ifndef FILTER_H
#define FILTER_H

#include "film.h"

// ############################################
// # Class Filter
// ############################################
class Filter {
public:
    // 构造函数
    Filter() {}

    // ACCESSORS
    Vec3f getColor(int i, int j, Film *film);
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

// ############################################
// # Class BoxFilter
// ############################################
class BoxFilter : public Filter {
public:
    // 构造函数
    BoxFilter(float radius) : radius(radius) {}

    // ACCESSORS
    float getWeight(float x, float y) override;
    int getSupportRadius() override;

private:
    float radius;
};

// ############################################
// # Class TentFilter
// ############################################
class TentFilter : public Filter {
public:
    // 构造函数
    TentFilter(float radius) : radius(radius) {}

    // ACCESSORS
    float getWeight(float x, float y) override;
    int getSupportRadius() override;

private:
    float radius;
};

// ############################################
// # Class GaussianFilter
// ############################################
class GaussianFilter : public Filter {
public:
    // 构造函数
    GaussianFilter(float radius) : radius(radius) {}

    // ACCESSORS
    float getWeight(float x, float y) override;
    int getSupportRadius() override;

private:
    float radius;
};

#endif