#pragma once
#include "framework.h"
#define PI 3.14159

enum Type
{
	Circle, Rect, Star
};

struct fPOINT
{
	float x;
	float y;
};

class CObject
{
protected:
	POINT point;
	fPOINT direction;
	float angle[2];
	LONG speed;
	LONG type;
public:
	CObject(POINT point, LONG type = Circle);
	~CObject();
	virtual void Update(RECT rectView) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual BOOL Collision() = 0;
	float Random(float min, float max);
	LONG PrintInfo();
};

class CCircle : public CObject
{
private :
	LONG radius;
public :
	CCircle(POINT point)
		: CObject(point, type = Circle) { radius = 40; }
	~CCircle();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
	virtual BOOL Collision() override;
};

class CRect : public CObject
{
private :
	POINT pt[4];
	LONG width;
	LONG height;
public:
	CRect(POINT point)
		: CObject(point, type = Rect) {
		width = 40;
		height = 40;
		pt[0] = { point.x - width, point.y - height };
		pt[1] = { point.x - width, point.y + height };
		pt[2] = { point.x + width, point.y + height };
		pt[3] = { point.x + width, point.y - height };
	}
	~CRect();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
	virtual BOOL Collision() override;
};

class CStar : public CObject
{
private:
	LONG radius;
public :
	CStar(POINT point)
		: CObject(point, type = Circle) { radius = 40; }
	~CStar();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
	virtual BOOL Collision() override;
};