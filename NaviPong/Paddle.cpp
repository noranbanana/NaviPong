#include "Paddle.h"
#include <cmath>
#include "GameConst.h"
// NPC 패들: 공을 따라가며 내쪽 중앙으로 반사

#include "pch.h"
#include "GameConst.h"

void NpcPaddle::UpdateAI(float dt, float ballX, float ballY, float ballVx, float ballVy) {
    // 공의 x좌표를 따라 좌우 이동 (간단한 추적)
    float targetX = ballX - this->width / 2.0f;
    // 속도 제한 (간단한 보정)
    float speed = 600.0f * dt;
    if (std::fabs(targetX - this->x) < speed) this->x = targetX;
    else if (targetX > this->x) this->x += speed;
    else if (targetX < this->x) this->x -= speed;
    // y는 상대 탁구대 뒤쪽 고정
    this->y = TABLE_TOP - this->height - 10;
    // 공이 npc쪽(위쪽)으로 올 때만 angle을 갱신
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
    // pullOffset만큼 y좌표에 반영 (시각+충돌 모두 적용)
    return D2D1::RectF(x, y + pullOffset, x + width, y + height + pullOffset);
}

void Paddle::UpdateFromMouse(int mouseX) {
    // 탁구대 아래쪽 넓은 범위에서 이동 (화면 전체 기준)
    float normalized = (float)mouseX / SCREEN_WIDTH;
    if (normalized < 0) normalized = 0;
    if (normalized > 1) normalized = 1;

    x = normalized * (SCREEN_WIDTH - width);
    y = PADDLE_Y; // 탁구대 아래쪽(네트보다 아래) 위치
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
    // 파워샷(패들 당김) 물리/시각 효과
    if (isPulling) {
        pullOffset += pullSpeed * dt;
        if (pullOffset > maxPull) pullOffset = maxPull;
    } else {
        // 탄성 복귀: F = -kx - bv
        float force = -returnStiffness * pullOffset - returnDamping * pullVelocity;
        pullVelocity += force * dt;
        pullOffset += pullVelocity * dt;
        // 0 근처에서 멈춤
        if (fabs(pullOffset) < 0.5f && fabs(pullVelocity) < 2.0f) {
            pullOffset = 0.0f;
            pullVelocity = 0.0f;
        }
    }
    // 공이 내 쪽(아래쪽)으로 올 때만 angle을 갱신
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

// 마우스 왼쪽 누름: 패들 뒤로 당기기 시작
void Paddle::SetPulling(bool pulling) {
    if (pulling && !isPulling) {
        // 당기기 시작: 속도 0
        pullVelocity = 0.0f;
    }
    isPulling = pulling;
    if (!pulling) {
        pullAmount = 0.0f;
        // 복귀 시작: pullVelocity 유지, pullOffset은 탄성 복귀
    }
}
