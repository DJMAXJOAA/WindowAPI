#pragma once
#include "resource.h"
#include "CObject.h"

void DrawGrid(HDC hdc, POINT start, POINT end, LONG number);
void DrawCircle(HDC hdc, POINT point, LONG radius);
void DrawStar(HDC hdc, POINT point, LONG radius);
bool DrawSunFlower(HDC hdc, POINT point, LONG radius, LONG number);
double LengthPts(POINT pt1, POINT pt2);
void OutFromFile(TCHAR filename[], HWND hWnd);

void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();

void Animation(int xPos, int yPos, HDC hdc);
void UpdateFrame(HWND hWnd);
VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

void DrawRectText(HDC hdc);

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
