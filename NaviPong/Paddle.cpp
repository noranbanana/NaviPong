#include "Paddle.h"
#include <cmath>
#include "GameConst.h"
// NPC �е�: ���� ���󰡸� ���� �߾����� �ݻ�

#include "pch.h"
#include "GameConst.h"

void NpcPaddle::UpdateAI(float dt, float ballX, float ballY, float ballVx, float ballVy) {
    // ���� x��ǥ�� ���� �¿� �̵� (������ ����)
    float targetX = ballX - this->width / 2.0f;
    // �ӵ� ���� (������ ����)
    float speed = 600.0f * dt;
    if (std::fabs(targetX - this->x) < speed) this->x = targetX;
    else if (targetX > this->x) this->x += speed;
    else if (targetX < this->x) this->x -= speed;
    // y�� ��� Ź���� ���� ����
    this->y = TABLE_TOP - this->height - 10;
    // ���� npc��(����)���� �� ���� angle�� ����
    if (ballVy < 0) {
        float centerX = this->x + this->width / 2.0f;
        float centerY = this->y + this->height / 2.0f;
        float ballAngle = std::atan2(ballVy, ballVx);
        float myGoalX = (TABLE_LEFT + TABLE_RIGHT) / 2.0f;
        float myGoalY = TABLE_BOTTOM;
        float toGoalAngle = std::atan2(myGoalY - centerY, myGoalX - centerX);
        this->angle = (ballAngle + toGoalAngle) / 2.0f;
    }
}
Paddle::Paddle(float x, float y) : x(x), y(y) {}

D2D1_RECT_F Paddle::GetRect() const {
    // pullOffset��ŭ y��ǥ�� �ݿ� (�ð�+�浹 ��� ����)
    return D2D1::RectF(x, y + pullOffset, x + width, y + height + pullOffset);
}

void Paddle::UpdateFromMouse(int mouseX) {
    // Ź���� �Ʒ��� ���� �������� �̵� (ȭ�� ��ü ����)
    float normalized = (float)mouseX / SCREEN_WIDTH;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;

    x = normalized * (SCREEN_WIDTH - width);
    y = PADDLE_Y; // Ź���� �Ʒ���(��Ʈ���� �Ʒ�) ��ġ
}

void Paddle::Swing() {
    swinging = true;
    swingTimer = 0;
}

void Paddle::Update(float dt, float ballVx, float ballVy) {
    if (this->swinging) {
        this->swingTimer += dt;
        if (this->swingTimer > 0.2f) {
            this->swinging = false;
            this->swingTimer = 0;
        }
    }
    // �Ŀ���(�е� ���) ����/�ð� ȿ��
    if (isPulling) {
        pullOffset += pullSpeed * dt;
        if (pullOffset > maxPull) pullOffset = maxPull;
    } else {
        // ź�� ����: F = -kx - bv
        float force = -returnStiffness * pullOffset - returnDamping * pullVelocity;
        pullVelocity += force * dt;
        pullOffset += pullVelocity * dt;
        // 0 ��ó���� ����
        if (fabs(pullOffset) < 0.5f && fabs(pullVelocity) < 2.0f) {
            pullOffset = 0.0f;
            pullVelocity = 0.0f;
        }
    }
    // ���� �� ��(�Ʒ���)���� �� ���� angle�� ����
    if (ballVy > 0) {
        float centerX = this->x + this->width / 2.0f;
        float centerY = this->y + this->height / 2.0f;
        float ballAngle = atan2(ballVy, ballVx);
        float targetX = (TABLE_LEFT + TABLE_RIGHT) / 2.0f;
        float targetY = TABLE_TOP;
        float toTargetAngle = atan2(targetY - centerY, targetX - centerX);
        this->angle = (ballAngle + toTargetAngle) / 2.0f;
    }
}

// ���콺 ���� ����: �е� �ڷ� ���� ����
void Paddle::SetPulling(bool pulling) {
    if (pulling && !isPulling) {
        // ���� ����: �ӵ� 0
        pullVelocity = 0.0f;
    }
    isPulling = pulling;
    if (!pulling) {
        pullAmount = 0.0f;
        // ���� ����: pullVelocity ����, pullOffset�� ź�� ����
    }
}
