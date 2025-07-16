
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
    float angle = 0.0f; // paddle�� ����(����)
    float pullAmount = 0.0f;
    bool isPulling = false;
    float pullOffset = 0.0f;      // �ð���/������ �е� ���
    float pullVelocity = 0.0f;    // ���ͽ� ź�� ȿ��
    const float maxPull = 40.0f;  // �ִ� ��� �Ÿ�
    const float pullSpeed = 120.0f; // ��� �� �ӵ�
    const float returnStiffness = 400.0f; // ���� ź��
    const float returnDamping = 10.0f;    // ����

protected:
    float x, y;
    float width = 80.0f;   // ���α���
    float height = 10.0f;  // ���α���
};

class NpcPaddle : public Paddle {
public:
    NpcPaddle(float x, float y) : Paddle(x, y) {}
    void UpdateAI(float dt, float ballX, float ballY, float ballVx, float ballVy);
};
