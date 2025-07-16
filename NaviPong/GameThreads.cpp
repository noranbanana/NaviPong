#include <thread>
#include <queue>
#include "pch.h"

std::queue<std::vector<Entity2D>> SyncQueue;
bool running = true;

void GameLogicThread(std::vector<Entity3D>& entities3D) {
    while (running) {
        for (auto& e : entities3D) {
            e.x += e.vx;
            e.y += e.vy;
            e.z += e.vz;
        }
        std::vector<Entity2D> entities2D;
        for (auto& e : entities3D)
            entities2D.push_back(ProjectTo2D(e));
        SyncQueue.push(entities2D);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void RenderThread() {
    while (running) {
        if (!SyncQueue.empty()) {
            auto entities2D = SyncQueue.front();
            SyncQueue.pop();
            for (auto& e : entities2D) {
                // DrawEntity2D(e); // 실제 렌더 함수 호출
            }
        }
    }
}