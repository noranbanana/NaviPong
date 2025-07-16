#pragma once
#include <d2d1.h>

class Paddle {
public:
    Paddle(float x, float y);
    D2D1_RECT_F GetRect() const;
    void UpdateFromMouse(int x);
    void Swing();
    void Update(float dt);
    bool swinging = false;
    float swingTimer = 0.0f;
    float angle = 0.0f; // paddle의 각도(라디안)

private:
    float x, y;
    float width = 80.0f;   // 가로로 넓게
    float height = 10.0f;  // 얇게
};
