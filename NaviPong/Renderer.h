#pragma once
#include <d2d1.h>
#include <vector>
#include "Paddle.h"
#include "Ball.h"
#include "Layer3D2D.h"
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

class Renderer {
public:
    void Initialize(HWND hwnd);
    void BeginDraw();
    void EndDraw();
    void DrawPaddle(const Paddle& paddle);
    void DrawBall(const Ball& ball);

    void DrawPerspectiveTable();

    void DrawUI();

    void DrawEntities(const std::vector<Entity2D>& entities);

private:
    ID2D1Factory* pFactory = nullptr;
    ID2D1HwndRenderTarget* pRenderTarget = nullptr;
    ID2D1SolidColorBrush* pBrush = nullptr;

    ID2D1SolidColorBrush* pTableBrush = nullptr;
    ID2D1SolidColorBrush* pBrushWhite = nullptr;
    IDWriteFactory* pDWriteFactory = nullptr;
    IDWriteTextFormat* pTextFormat = nullptr;

};
