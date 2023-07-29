#ifndef SAMPLER_H
#define SAMPLER_H

#include "vectors.h"

// ############################################
// # Class Sampler
// ############################################
class Sampler {
public:
    // 构造函数
    Sampler(int num_samples) : num_samples(num_samples) {}

    // 纯虚函数
    virtual Vec2f getSamplePosition(int n) = 0;

protected:
    int num_samples;
};

// ############################################
// # Class RandomSampler
// ############################################
class RandomSampler : public Sampler {
public:
    // 构造函数
    RandomSampler(int num_samples) : Sampler(num_samples) {}

    Vec2f getSamplePosition(int n) override {
        return Vec2f(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX);
    }
};

// ############################################
// # Class UniformSampler
// ############################################
class UniformSampler : public Sampler {
public:
    // 构造函数
    UniformSampler(int num_samples) : Sampler(num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / (size + 1);
    }

    Vec2f getSamplePosition(int n) override {
        if (num_samples == 1)
            return Vec2f();
        return Vec2f((n % size + 1) * d, (n / size + 1) * d);
    }

private:
    int size;
    float d;
};

// ############################################
// # Class JitteredSampler
// ############################################
class JitteredSampler : public Sampler {
public:
    // 构造函数
    JitteredSampler(int num_samples) : Sampler(num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / (size + 1);
    }

    Vec2f getSamplePosition(int n) override {
        Vec2f p = Vec2f((n % size + 1) * d, (n / size + 1) * d);
        return Vec2f(p.x() + (2.0f * rand() / RAND_MAX - 1) * d, p.y() + (2.0f * rand() / RAND_MAX - 1) * d);
    }

private:
    int size;
    float d;
};

#endif