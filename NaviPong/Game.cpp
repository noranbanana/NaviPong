#include <vector>
#include "pch.h"

void Game::Initialize(HWND hwnd) {
    renderer = std::make_unique<Renderer>();
    renderer->Initialize(hwnd);

    player = std::make_unique<Paddle>(100.0f, 250.0f);
    ball = std::make_unique<Ball>(400.0f, 300.0f);
}

void Game::Update() {
    // 3D ���� ó��
    for (auto& e : entities3D) {
        e.x += e.vx;
        e.y += e.vy;
        e.z += e.vz;
    }
    // 3D �� 2D ��ȯ
    entities2D.clear();
    for (auto& e : entities3D)
        entities2D.push_back(ProjectTo2D(e));

    if (player) player->Update(0.016f); // �����Ӵ� paddle ������Ʈ (dt�� �ӽð�)
    ball->Update();

    if (ball->Intersects(*player)) {
        ball->Bounce(*player);
    }

    if (ball->GetEllipse().point.x < 0 || ball->GetEllipse().point.x > 800) {
        ball->Reset();
    }
}

void Game::Render() {
    renderer->BeginDraw();
    renderer->DrawPerspectiveTable(); 
    renderer->DrawPaddle(*player); 
    renderer->DrawBall(*ball);
    renderer->DrawUI(); 
    renderer->EndDraw();
}

void Game::SetMousePosition(int x, int y) {
    if (player)
        player->UpdateFromMouse(x);
}

void Game::Swing() {
    if (player)
        player->Swing();
}

std::vector<Entity3D> entities3D;
std::vector<Entity2D> entities2D;
