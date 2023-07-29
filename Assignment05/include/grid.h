#ifndef GRID_H
#define GRID_H

#include "object3d.h"
#include <vector>
#include "marchingInfo.h"

class Grid : public Object3D {
public:
    // CONSTRUCTORS & DESTRUCTOR
    Grid(BoundingBox *bb, int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz) {
        boundingBox = bb;
        opaque.resize(nx * ny * nz);
    }
    ~Grid() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() override;
    virtual bool intersectShadowRay(const Ray &r, Hit &h, float tmin) override; // add this line
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

    int nx, ny, nz;
    //std::vector<bool> opaque;
    std::vector<std::vector<Object3D*> > opaque;
};

#endif