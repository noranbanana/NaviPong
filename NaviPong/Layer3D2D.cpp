#include "pch.h"

Entity2D ProjectTo2D(const Entity3D& e3d) {
    Entity2D e2d;
    e2d.screenX = e3d.x;
    e2d.screenY = e3d.y - e3d.z;
    e2d.scale = 1.0f - e3d.z * 0.1f;
    e2d.alpha = 1.0f - e3d.z * 0.5f;
    e2d.angle = e3d.angle;
    return e2d;
}