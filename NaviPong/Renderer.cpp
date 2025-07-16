#include "pch.h"

// ���̾�� �и��ؼ� �׸���
void Renderer::DrawLayers(const Ball& ball, const Paddle& player, const NpcPaddle* npc) {
    // ��� ���̾�
    pRenderTarget->PushLayer(nullptr, nullptr);
    DrawPerspectiveTable();
    pRenderTarget->PopLayer();

    // �׸��� ���̾�
    pRenderTarget->PushLayer(nullptr, nullptr);
    ball.DrawShadow(pRenderTarget, pBrush);
    pRenderTarget->PopLayer();

    // �� ���̾�
    pRenderTarget->PushLayer(nullptr, nullptr);
    DrawBall(ball);
    pRenderTarget->PopLayer();

    // �е� ���̾�
    pRenderTarget->PushLayer(nullptr, nullptr);
    DrawPaddle(player);
    if (npc) DrawPaddle(*npc);
    pRenderTarget->PopLayer();

    // UI ���̾�
    pRenderTarget->PushLayer(nullptr, nullptr);
    DrawUI();
    pRenderTarget->PopLayer();
}

void Renderer::Initialize(HWND hwnd) {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &pRenderTarget
    );

    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);

    // Table ����� �귯�� (���)
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.5f, 0.0f), &pTableBrush);

    // UI�� ��� �귯��
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrushWhite);

    // �ؽ�Ʈ ��¿� DirectWrite �ʱ�ȭ
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&pDWriteFactory);

    pDWriteFactory->CreateTextFormat(
        L"Segoe UI",            // Font family
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20.0f,
        L"en-us",
        &pTextFormat
    );

}

void Renderer::BeginDraw() {
    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void Renderer::EndDraw() {
    pRenderTarget->EndDraw();
}

void Renderer::DrawPaddle(const Paddle& paddle) {
    // paddle ���� �ݿ��ؼ� ȸ���� �簢�� �׸���
    // pullOffset��ŭ y��(�Ʒ���) �̵����� �ð��� ��� ȿ��
    D2D1_RECT_F rect = paddle.GetRect();
    float cx = (rect.left + rect.right) / 2.0f;
    float cy = (rect.top + rect.bottom) / 2.0f;
    float offsetY = paddle.pullOffset;
    D2D1_MATRIX_3X2_F oldTransform;
    pRenderTarget->GetTransform(&oldTransform);
    // �̵� + ȸ��
    D2D1_MATRIX_3X2_F translate = D2D1::Matrix3x2F::Translation(0, offsetY);
    D2D1_MATRIX_3X2_F rotate = D2D1::Matrix3x2F::Rotation(paddle.angle * 180.0f / 3.141592f, D2D1::Point2F(cx, cy + offsetY));
    pRenderTarget->SetTransform(rotate * translate * oldTransform);
    pRenderTarget->FillRectangle(rect, pBrush);
    pRenderTarget->SetTransform(oldTransform);
}

void Renderer::DrawBall(const Ball& ball) {
    // �ٴ� �׸���(ȸ��) ���� �׸�
    D2D1_COLOR_F shadowColor = D2D1::ColorF(0.1f, 0.1f, 0.1f, 0.25f);
    ID2D1SolidColorBrush* pShadowBrush = nullptr;
    pRenderTarget->CreateSolidColorBrush(shadowColor, &pShadowBrush);
    ball.DrawShadow(pRenderTarget, pShadowBrush);
    pShadowBrush->Release();
    // ���� ��
    pRenderTarget->FillEllipse(ball.GetEllipse(), pBrush);
}

void Renderer::DrawPerspectiveTable() {
    D2D1_POINT_2F p0 = D2D1::Point2F(TABLE_LEFT, TABLE_BOTTOM); // ���� ���� (�߾Ӻ�)
    D2D1_POINT_2F p1 = D2D1::Point2F(TABLE_RIGHT, TABLE_BOTTOM); // ���� ������ (�߾Ӻ�)
    D2D1_POINT_2F p2 = D2D1::Point2F(TABLE_RIGHT-50, TABLE_TOP); // ��� ������ (�߾Ӻ�)
    D2D1_POINT_2F p3 = D2D1::Point2F(TABLE_LEFT+50, TABLE_TOP); // ��� ���� (�߾Ӻ�)

    // ...Ź����(���̺�) �׸��� �ڵ�...

    ID2D1PathGeometry* pGeometry = nullptr;
    pFactory->CreatePathGeometry(&pGeometry);

    ID2D1GeometrySink* pSink = nullptr;
    pGeometry->Open(&pSink);

    pSink->BeginFigure(p0, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(p1);
    pSink->AddLine(p2);
    pSink->AddLine(p3);
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    pRenderTarget->FillGeometry(pGeometry, pTableBrush);

    pSink->Release();
    pGeometry->Release();

    // ��Ʈ(�߾Ӽ�) ���� ���� ���� �簢������ �׸���
    float netLeft = TABLE_LEFT+20, netRight = TABLE_RIGHT-20;
    float netBottom = NET_Y;
    float netTop = NET_Y-NET_HEIGHT;
    D2D1_POINT_2F netP0 = D2D1::Point2F(netLeft, netBottom); // ���ϴ�
    D2D1_POINT_2F netP1 = D2D1::Point2F(netRight, netBottom); // ���ϴ�
    D2D1_POINT_2F netP2 = D2D1::Point2F(netRight, netTop);    // ����
    D2D1_POINT_2F netP3 = D2D1::Point2F(netLeft, netTop);     // �»��

    ID2D1PathGeometry* netGeometry = nullptr;
    pFactory->CreatePathGeometry(&netGeometry);
    ID2D1GeometrySink* netSink = nullptr;
    netGeometry->Open(&netSink);
    netSink->BeginFigure(netP0, D2D1_FIGURE_BEGIN_FILLED);
    netSink->AddLine(netP1);
    netSink->AddLine(netP2);
    netSink->AddLine(netP3);
    netSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    netSink->Close();

    D2D1_COLOR_F netColor = D2D1::ColorF(D2D1::ColorF::White, 0.85f);
    ID2D1SolidColorBrush* netBrush = nullptr;
    pRenderTarget->CreateSolidColorBrush(netColor, &netBrush);
    pRenderTarget->FillGeometry(netGeometry, netBrush);
    netBrush->Release();
    netSink->Release();
    netGeometry->Release();
}

void Renderer::DrawUI() {
    std::wstring text = L"Click to Swing!";
    pRenderTarget->DrawTextW(
        text.c_str(), text.length(),
        pTextFormat, D2D1::RectF(10, 10, 300, 40),
        pBrushWhite);
}

void Renderer::DrawEntities(const std::vector<Entity2D>& entities) {
    for (const auto& e : entities) {
        float cx = e.screenX;
        float cy = e.screenY;
        float w = 40.0f * e.scale; // ���� ũ��
        float h = 40.0f * e.scale;
        D2D1_RECT_F rect = D2D1::RectF(cx - w/2, cy - h/2, cx + w/2, cy + h/2);

        D2D1_MATRIX_3X2_F oldTransform;
        pRenderTarget->GetTransform(&oldTransform);
        D2D1_MATRIX_3X2_F rotate = D2D1::Matrix3x2F::Rotation(e.angle * 180.0f / 3.141592f, D2D1::Point2F(cx, cy));
        pRenderTarget->SetTransform(rotate * oldTransform);

        ID2D1SolidColorBrush* tempBrush = nullptr;
        D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::SkyBlue, e.alpha);
        pRenderTarget->CreateSolidColorBrush(color, &tempBrush);
        pRenderTarget->FillRectangle(rect, tempBrush);
        tempBrush->Release();

        pRenderTarget->SetTransform(oldTransform);
    }
}