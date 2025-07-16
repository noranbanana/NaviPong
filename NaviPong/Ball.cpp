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
    // �߷� ����
    vz -= 0.98f;
    z += vz;
    // �ٴ�(Ź����)�� ������ ƨ��
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
    // �浹 ���� �� �ݻ�
    if (Intersects(player)) Bounce(player);
    if (npc && Intersects(*npc)) Bounce(*npc);
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

    // �е鿡 ������ �׻� ���� Ƣ�� vz�� ������(5.0f)���� ����
    vz = 5.0f;
    // �Ŀ���: pullOffset�� ���� ���� ���� �ӵ�/���� �߰� �ݿ�
    if (!paddle.isPulling && fabs(paddle.pullVelocity) > 10.0f && paddle.pullOffset == 0.0f) {
        float boost = fabs(paddle.pullVelocity) * 0.10f; // �Ŀ� ���� ����
        float speed = sqrt(vx * vx + vy * vy);
        float angle = atan2(vy, vx);
        speed += boost;
        vx = speed * cos(angle);
        vy = speed * sin(angle);
        vz += boost * 0.3f; // �Ŀ����� ���� ���� �߰�(�ʹ� ���� �ʰ� 0.3��)
    }
}

bool Ball::Intersects(const Paddle& paddle) const {
    D2D1_RECT_F rect = paddle.GetRect();
    return x < rect.right&& x + radius * 2 > rect.left &&
        y < rect.bottom&& y + radius * 2 > rect.top;
}

D2D1_ELLIPSE Ball::GetEllipse() const {
    // z(����)�� ���� y��ǥ�� ���� �÷��� �ð��� ȿ�� (������ ����)
    float visualY = y + radius - z * 1.2f; // ���� Ŭ���� ����
    float visualRadius = radius + z * 0.10f; // ���� Ŭ���� �� Ŀ��
    return D2D1::Ellipse(D2D1::Point2F(x + radius, visualY), visualRadius, visualRadius);
}


void Ball::DrawShadow(ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pShadowBrush) const {
    if (this->z > 1.0f) return; // ���߿� �� ������ �׸��� ����
    float shadowY = this->y + this->radius; // �ٴ�(y) ��ġ
    float shadowRadiusX = this->radius * 1.2f;
    float shadowRadiusY = this->radius * 0.5f;
    D2D1_ELLIPSE shadow = D2D1::Ellipse(D2D1::Point2F(this->x + this->radius, shadowY), shadowRadiusX, shadowRadiusY);
    pRenderTarget->FillEllipse(shadow, pShadowBrush);
}
