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
    void Render();
    void SetMousePosition(int x, int y);
    void Swing();

private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Paddle> player;
    std::unique_ptr<Ball> ball;
};
