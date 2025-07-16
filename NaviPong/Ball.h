#pragma once
#include <d2d1.h>
#include "Paddle.h"

class Ball {
public:
    Ball(float x, float y);
    void Reset();
    void Update(const Paddle& player, const NpcPaddle* npc);
    void Bounce(const Paddle& paddle);
    bool Intersects(const Paddle& paddle) const;
    D2D1_ELLIPSE GetEllipse() const;
    float GetVx() const { return vx; }
    float GetVy() const { return vy; }
    float GetZ() const { return z; }
    void DrawShadow(ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pShadowBrush) const;

private:
    float x, y;
    float vx = -3.0f, vy = 2.5f;
    float z = 50.0f;    // 공의 높이
    float vz = 0.0f;    // 수직 속도
    float radius = 10.0f;
    bool onTable = false; // 탁구대에 닿았는지
};
