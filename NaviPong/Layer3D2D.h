// include guard Ãß°¡
#ifndef LAYER3D2D_H
#define LAYER3D2D_H

struct Entity3D {
    float x, y, z;
    float vx, vy, vz;
    float angle;
};

struct Entity2D {
    float screenX, screenY;
    float scale, alpha;
    float angle;
};

Entity2D ProjectTo2D(const Entity3D& e3d);

#endif // LAYER3D2D_H