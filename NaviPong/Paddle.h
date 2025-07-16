
#pragma once
#include <d2d1.h>
#include <cmath>
#include "GameConst.h"

class Paddle {
public:
    Paddle(float x, float y);
    D2D1_RECT_F GetRect() const;
    void UpdateFromMouse(int x);
    void Swing();
    void Update(float dt, float ballVx = 0.0f, float ballVy = -1.0f);
    void SetPulling(bool pulling);
    bool swinging = false;
    float swingTimer = 0.0f;
    float angle = 0.0f; // paddle의 각도(라디안)
    float pullAmount = 0.0f;
    bool isPulling = false;
    float pullOffset = 0.0f;      // 시각적/물리적 패들 당김
    float pullVelocity = 0.0f;    // 복귀시 탄성 효과
    const float maxPull = 40.0f;  // 최대 당김 거리
    const float pullSpeed = 120.0f; // 당길 때 속도
    const float returnStiffness = 400.0f; // 복귀 탄성
    const float returnDamping = 10.0f;    // 감쇠

protected:
    float x, y;
    float width = 80.0f;   // 가로길이
    float height = 10.0f;  // 세로길이
};

class NpcPaddle : public Paddle {
public:
    NpcPaddle(float x, float y) : Paddle(x, y) {}
    void UpdateAI(float dt, float ballX, float ballY, float ballVx, float ballVy);
};
