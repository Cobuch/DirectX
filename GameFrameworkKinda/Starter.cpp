#include "Starter.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

Starter::Starter():Game(L"MyGame", 800, 800)
{

}

Starter::~Starter()
{
	for (auto c : Components)
	{
		delete c;
	}
}

void Starter::DestroyResources()
{
	for (auto c : Components)
	{
		c->DestroyResources();
	}
}

void Starter::Draw()
{
	float color[] = { TotalTime, 0.1f, 0.1f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);

	for (auto c : Components)
	{
		c->Draw();
	}
}

void Starter::EndFrame()
{
}

void Starter::Initialize()
{
	Components.push_back(new TriangleComponent(this));
	Components.push_back(new TriangleComponent(this));

	for (auto c : Components)
	{
		c->Initialize();
	}
}

void Starter::PrepareFrame()
{
}

void Starter::PrepareResources()
{
}

void Starter::Update()
{
}

void Starter::UpdateInternal()
{
}

int main()
{
	Starter game;
	game.Run();
}