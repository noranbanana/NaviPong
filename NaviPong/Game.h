#include <thread>
#include <atomic>

#pragma once
#include <windows.h>
#include <memory>
#include "Renderer.h"
#include "Paddle.h"
#include "Ball.h"

class Game {
public:
    void Initialize(HWND hwnd);
    void Update();
    void SetMouseDown();
    void SetMouseUp();
    void Render();
    void SetMousePosition(int x, int y);
    void Swing();

private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Paddle> player;
    std::unique_ptr<NpcPaddle> npc;
    std::unique_ptr<Ball> ball;
    std::vector<Entity3D> entities3D;
    std::vector<Entity2D> entities2D;    
    std::thread physicsThread;
    std::atomic<bool> running = false;
    void PhysicsLoop();
public:
    void Shutdown();
};
