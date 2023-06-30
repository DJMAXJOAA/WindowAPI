#pragma once
#include "resource.h"
#include "CObject.h"

void DrawGrid(HDC hdc, POINT start, POINT end, LONG number);
void DrawCircle(HDC hdc, POINT point, LONG radius);
void DrawStar(HDC hdc, POINT point, LONG radius);
bool DrawSunFlower(HDC hdc, POINT point, LONG radius, LONG number);
double LengthPts(POINT pt1, POINT pt2);