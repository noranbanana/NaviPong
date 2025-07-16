#include <vector>
#include "pch.h"

void Game::SetMouseDown() {
    if (player) player->SetPulling(true);
}
void Game::SetMouseUp() {
    if (player) player->SetPulling(false);
}

void Game::Initialize(HWND hwnd) {
    renderer = std::make_unique<Renderer>();
    renderer->Initialize(hwnd);

    player = std::make_unique<Paddle>(100.0f, 250.0f);
    npc = std::make_unique<NpcPaddle>(300.0f, TABLE_TOP - 30.0f);
    ball = std::make_unique<Ball>(400.0f, 300.0f);

    running = true;
    physicsThread = std::thread(&Game::PhysicsLoop, this);
}

void Game::PhysicsLoop() {
    while (running) {
        if (ball && player) ball->Update(*player, npc.get());
        if (npc && ball) npc->UpdateAI(0.016f, ball->GetEllipse().point.x, ball->GetEllipse().point.y, ball->GetVx(), ball->GetVy());
        // 3D Entity 등 추가 가능
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::Update() {
    // 3D 물리 처리 (예시, 멀티스레드와 별개로 필요시)
    for (auto& e : entities3D) {
        e.x += e.vx;
        e.y += e.vy;
        e.z += e.vz;
    }
    // 3D → 2D 변환
    entities2D.clear();
    for (auto& e : entities3D)
        entities2D.push_back(ProjectTo2D(e));

    if (player && ball) player->Update(0.016f, ball->GetVx(), ball->GetVy());
    // 점수/리셋 등만 메인스레드에서 처리
    if (ball->GetEllipse().point.x < 0 || ball->GetEllipse().point.x > 800) {
        ball->Reset();
    }
}

void Game::Shutdown() {
    running = false;
    if (physicsThread.joinable()) physicsThread.join();
}

void Game::Render() {
    renderer->BeginDraw();
    renderer->DrawPerspectiveTable(); 
    renderer->DrawPaddle(*player); 
    if (npc) renderer->DrawPaddle(*npc);
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

