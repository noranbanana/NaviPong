#include "pch.h"
#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <thread>


Game* g_game = nullptr; // 전역 포인터 (전달용)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void GameLogicThread(std::vector<Entity3D>&);
void RenderThread();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DXPongWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"DXPong", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    Game game;
    g_game = &game;
    game.Initialize(hwnd);

    std::vector<Entity3D> entities3D;
    // ...초기화...

    std::thread logicThread(GameLogicThread, std::ref(entities3D));
    std::thread renderThread(RenderThread);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            game.Update();
            game.Render();
        }
    }

    logicThread.join();
    renderThread.join();

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_MOUSEMOVE:
        if (g_game) {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            g_game->SetMousePosition(x, y);  // 마우스 이동 전달
        }
        return 0;

    case WM_LBUTTONDOWN:
        if (g_game) {
            g_game->Swing();  // 클릭으로 스윙
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
