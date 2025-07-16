#include "pch.h"
#include "math.h"

Ball::Ball(float x, float y) : x(x), y(y), z(50.0f), vz(0.0f), onTable(false) {}

void Ball::Reset() {
    x = 400.0f;
    y = 300.0f;
    vx = -3.0f;
    vy = 2.5f;
    z = 50.0f;
    vz = 0.0f;
    onTable = false;
}

void Ball::Update(const Paddle& player, const NpcPaddle* npc) {
    x += vx;
    y += vy;
    // 중력 적용
    vz -= 0.98f;
    z += vz;
    // 바닥(탁구대)에 닿으면 튕김
    if (z < 0.0f) {
        z = 0.0f;
        if (!onTable) {
            vz = -vz * 0.7f;
            onTable = true;
        } else {
            vz = 0.0f;
        }
    } else {
        onTable = false;
    }
    if (y < 0 || y > 600 - radius * 2) vy *= -1;
    // 충돌 판정 및 반사
    if (Intersects(player)) Bounce(player);
    if (npc && Intersects(*npc)) Bounce(*npc);
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

    // 패들에 맞으면 항상 위로 튀게 vz를 일정값(5.0f)으로 설정
    vz = 5.0f;
    // 파워샷: pullOffset이 복귀 중일 때만 속도/높이 추가 반영
    if (!paddle.isPulling && fabs(paddle.pullVelocity) > 10.0f && paddle.pullOffset == 0.0f) {
        float boost = fabs(paddle.pullVelocity) * 0.10f; // 파워 비율 조정
        float speed = sqrt(vx * vx + vy * vy);
        float angle = atan2(vy, vx);
        speed += boost;
        vx = speed * cos(angle);
        vy = speed * sin(angle);
        vz += boost * 0.3f; // 파워샷일 때만 높이 추가(너무 높지 않게 0.3배)
    }
}

bool Ball::Intersects(const Paddle& paddle) const {
    D2D1_RECT_F rect = paddle.GetRect();
    return x < rect.right&& x + radius * 2 > rect.left &&
        y < rect.bottom&& y + radius * 2 > rect.top;
}

D2D1_ELLIPSE Ball::GetEllipse() const {
    // z(높이)에 따라 y좌표를 위로 올려서 시각적 효과 (보정폭 증가)
    float visualY = y + radius - z * 1.2f; // 높이 클수록 위로
    float visualRadius = radius + z * 0.10f; // 높이 클수록 더 커짐
    return D2D1::Ellipse(D2D1::Point2F(x + radius, visualY), visualRadius, visualRadius);
}


void Ball::DrawShadow(ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pShadowBrush) const {
    if (this->z > 1.0f) return; // 공중에 떠 있으면 그림자 없음
    float shadowY = this->y + this->radius; // 바닥(y) 위치
    float shadowRadiusX = this->radius * 1.2f;
    float shadowRadiusY = this->radius * 0.5f;
    D2D1_ELLIPSE shadow = D2D1::Ellipse(D2D1::Point2F(this->x + this->radius, shadowY), shadowRadiusX, shadowRadiusY);
    pRenderTarget->FillEllipse(shadow, pShadowBrush);
}
