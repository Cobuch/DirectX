#pragma once
#include <windows.h>
#include <WinUser.h>
#include <iostream>
#include "Keys.h"

class Game;

class DisplayWin32
{
private:
	Game* game;
public:
	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	int ClientHeight;
	int ClientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	DisplayWin32(LPCWSTR applicationName, HINSTANCE hInst, int screenWidth, int screenHeight, Game* g);
	void CreateGameWindow(LPCWSTR applicationName, int windowWidth, int windowHeight);
};
