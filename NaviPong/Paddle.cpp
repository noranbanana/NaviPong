#include "pch.h"

Paddle::Paddle(float x, float y) : x(x), y(y) {}

D2D1_RECT_F Paddle::GetRect() const {
    return D2D1::RectF(x, y, x + width, y + height);
}

void Paddle::UpdateFromMouse(int mouseX) {
    // 내 시점 기준 사다리꼴 내에서 이동
    float normalized = (mouseX - 200) / 400.0f;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;

    x = 200 + normalized * (600 - 200 - width);
    y = 340; // 탁구대 중앙부에 위치
}

void Paddle::Swing() {
    swinging = true;
    swingTimer = 0;
}

void Paddle::Update(float dt) {
    if (swinging) {
        swingTimer += dt;
        if (swingTimer > 0.2f) {
            swinging = false;
            swingTimer = 0;
        }
        // 예시: 스윙 시 paddle 각도 변화
        angle = 0.3f; // 0.3 라디안(약 17도)로 임시 설정
    } else {
        angle = 0.0f;
    }
}