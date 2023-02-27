#define NOMINMAX
#include "Starter.h"
#include <string>

#include <iostream>
#include <algorithm>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")



static float DifficultyCurve(std::chrono::steady_clock::time_point continueTime)
{
    return 5.0f / (1.0f + exp(-std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - continueTime).count() / 8.0f + 4.0f)) + 1.0f;
}

void Starter::Update()
{
    ball->Speed = DifficultyCurve(ContinueTime);
    if (state == PONG_STATE_COOLDOWN && std::chrono::steady_clock::now() >= ContinueTime)
        state = PONG_STATE_NORMAL;

    if (InputDev->IsKeyDown(Keys::W) && player1->GetY() < 0.89f)
        player1->SetY(player1->GetY() + DeltaTime * player1->Speed);
    if (InputDev->IsKeyDown(Keys::S) && player1->GetY() > -0.89f)
        player1->SetY(player1->GetY() - DeltaTime * player1->Speed);

    if (InputDev->IsKeyDown(Keys::D) && player1->GetX() < -0.1f)
        player1->SetX(player1->GetX() + DeltaTime * player1->Speed);
    if (InputDev->IsKeyDown(Keys::A) && player1->GetX() > -0.9f)
        player1->SetX(player1->GetX() - DeltaTime * player1->Speed);


    if (InputDev->IsKeyDown(Keys::Up) && player2->GetY() < 0.89f)
        player2->SetY(player2->GetY() + DeltaTime * player2->Speed);
    if (InputDev->IsKeyDown(Keys::Down) && player2->GetY() > -0.89f)
        player2->SetY(player2->GetY() - DeltaTime * player2->Speed);
    
    if (InputDev->IsKeyDown(Keys::Right) && player2->GetX() < 0.9f)
        player2->SetX(player2->GetX() + DeltaTime * player2->Speed);
    if (InputDev->IsKeyDown(Keys::Left) && player2->GetX() > 0.1f)
        player2->SetX(player2->GetX() - DeltaTime * player2->Speed);



    Game::Update();
}

Starter::Starter() : Game(L"MyGame", 800, 800)
{
    srand(static_cast<unsigned>(time(nullptr)));
    player1 = new PlayerComponent(this);
    player1->SetX(-0.9f);
    player2 = new PlayerComponent(this);
    player2->SetX(0.9f);
    ball = new BallComponent(this);
    Components.push_back(player1);
    Components.push_back(player2);
    Components.push_back(ball);
    state = PONG_STATE_COOLDOWN;
    ContinueTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
}

void Starter::Draw()
{
    Game::Draw();

    std::wstring s = std::to_wstring(s1) + L"        " + std::to_wstring(s2);

    if (state == PONG_STATE_GAMEOVER)
    {
        if (s1 > 10)
            s = L"P1 WON";
        else
            s = L"P2 WON";
    }

    D2DRenderTarget->BeginDraw();
    D2DRenderTarget->DrawText(
        s.c_str(),
        s.length(),
        DWTextFormat,
        D2D1::RectF(0, -700, 800, 800),
        D2Dbrush
    );
    D2DRenderTarget->EndDraw();
}

void Starter::Initialize()
{
    Game::Initialize();

    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 50;
    HRESULT hr;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);

    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(DWFactory),
            reinterpret_cast<IUnknown**>(&DWFactory)
        );
    }
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = DWFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &DWTextFormat
        );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        DWTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        DWTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&D2DBackBuff);

    // Create the DXGI Surface Render Target.
    float dpi = GetDpiForWindow(Display->hWnd);

    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            0.0f,
            0.0f);

    // Create a Direct2D render target that can draw into the surface in the swap chain

    hr = D2DFactory->CreateDxgiSurfaceRenderTarget(
        D2DBackBuff,
        &props,
        &D2DRenderTarget);

    hr = D2DRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White, 1.0f),
        &D2Dbrush
    );

    hr = D2DFactory->CreateStrokeStyle(
        D2D1::StrokeStyleProperties(
            D2D1_CAP_STYLE_FLAT,
            D2D1_CAP_STYLE_FLAT,
            D2D1_CAP_STYLE_ROUND,
            D2D1_LINE_JOIN_MITER,
            10.0f,
            D2D1_DASH_STYLE_DASH,
            0.0f),
        nullptr,
        0,
        &D2DLineStrokeStyle
    );
}

void Starter::DestroyResources()
{
    D2DBackBuff->Release();
    D2DFactory->Release();
    DWFactory->Release();
    DWTextFormat->Release();
    D2DRenderTarget->Release();
    D2Dbrush->Release();
    D2DLineStrokeStyle->Release();
    Game::DestroyResources();
}

void Starter::IncScore(bool p)
{
    if (p)
        s1 += 1;
    else
        s2 += 1;
    if (s1 >= 10 || s2 >= 10)
        state = PONG_STATE_GAMEOVER;
}

void Starter::Move(DirectX::SimpleMath::Vector3 positionOffset)
{
}
