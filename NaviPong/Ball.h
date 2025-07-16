#pragma once
#include <d2d1.h>
#include "Paddle.h"

class Ball {
public:
    Ball(float x, float y);
    void Reset();
    void Update();
    void Bounce(const Paddle& paddle);
    bool Intersects(const Paddle& paddle) const;
    D2D1_ELLIPSE GetEllipse() const;

private:
    float x, y;
    float vx = -3.0f, vy = 2.5f;
    float radius = 10.0f;
};
