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
    // paddle�� ����(���� ����)
    float normalAngle = paddle.angle; // �����̸� 0
    float nx = sin(normalAngle);      // x����
    float ny = -cos(normalAngle);     // y���� (������ -)

    // ���� �ӵ� ����
    float dot = vx * nx + vy * ny;
    vx = vx - 2 * dot * nx;
    vy = vy - 2 * dot * ny;

    // offset(�浹 ��ġ)�� ���� �߰� ���� ��ȭ
    float ballCenter = x + radius;
    D2D1_RECT_F rect = paddle.GetRect();
    float paddleCenter = rect.left + (rect.right - rect.left) / 2.0f;
    float offset = (ballCenter - paddleCenter) / ((rect.right - rect.left) / 2.0f); // -1~1
    float angleOffset = offset * 0.3f; // �ִ� 0.3 ����
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
