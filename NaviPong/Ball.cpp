#include "pch.h"
#include "math.h"

Ball::Ball(float x, float y) : x(x), y(y) {}

void Ball::Reset() {
    x = 400.0f;
    y = 300.0f;
    vx = -3.0f;
    vy = 2.5f;
}

void Ball::Update() {
    x += vx;
    y += vy;

    if (y < 0 || y > 600 - radius * 2) vy *= -1;
}

void Ball::Bounce(const Paddle& paddle) {
    // paddle의 각도(법선 벡터)
    float normalAngle = paddle.angle; // 수평이면 0
    float nx = sin(normalAngle);      // x성분
    float ny = -cos(normalAngle);     // y성분 (위쪽이 -)

    // 공의 속도 벡터
    float dot = vx * nx + vy * ny;
    vx = vx - 2 * dot * nx;
    vy = vy - 2 * dot * ny;

    // offset(충돌 위치)에 따라 추가 각도 변화
    float ballCenter = x + radius;
    D2D1_RECT_F rect = paddle.GetRect();
    float paddleCenter = rect.left + (rect.right - rect.left) / 2.0f;
    float offset = (ballCenter - paddleCenter) / ((rect.right - rect.left) / 2.0f); // -1~1
    float angleOffset = offset * 0.3f; // 최대 0.3 라디안
    float speed = sqrt(vx * vx + vy * vy);
    float angle = atan2(vy, vx) + angleOffset;
    vx = speed * cos(angle);
    vy = speed * sin(angle);
}

bool Ball::Intersects(const Paddle& paddle) const {
    D2D1_RECT_F rect = paddle.GetRect();
    return x < rect.right&& x + radius * 2 > rect.left &&
        y < rect.bottom&& y + radius * 2 > rect.top;
}

D2D1_ELLIPSE Ball::GetEllipse() const {
    return D2D1::Ellipse(D2D1::Point2F(x + radius, y + radius), radius, radius);
}
