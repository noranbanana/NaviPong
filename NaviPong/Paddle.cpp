#include "pch.h"

Paddle::Paddle(float x, float y) : x(x), y(y) {}

D2D1_RECT_F Paddle::GetRect() const {
    return D2D1::RectF(x, y, x + width, y + height);
}

void Paddle::UpdateFromMouse(int mouseX) {
    // �� ���� ���� ��ٸ��� ������ �̵�
    float normalized = (mouseX - 200) / 400.0f;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;

    x = 200 + normalized * (600 - 200 - width);
    y = 340; // Ź���� �߾Ӻο� ��ġ
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
        // ����: ���� �� paddle ���� ��ȭ
        angle = 0.3f; // 0.3 ����(�� 17��)�� �ӽ� ����
    } else {
        angle = 0.0f;
    }
}