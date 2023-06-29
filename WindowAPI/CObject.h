#pragma once
#include "framework.h"
#define PI 3.14159

enum Type
{
	Circle, Rectangle, Star
};

class CObject
{
protected:
	POINT point;
	POINT direction;
	LONG speed;
	LONG Type;
public:
	CObject(POINT point, POINT direction, LONG speed, LONG type = Circle);
	~CObject();
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual BOOL Collision() = 0;
};

class CCircle : public CObject
{
private :
	LONG radius;
public :
	CCircle(POINT point, POINT direction, LONG speed, LONG type, LONG radius)
		: CObject(point, direction, speed, type = Circle) {}
	~CCircle();
	virtual void Update() override;
	virtual void Draw() override;
	virtual BOOL Collision() override;
};

class CRect : public CObject
{
private :
	LONG width;
	LONG height;
public:
	CRect(POINT point, POINT direction, LONG speed, LONG type, LONG radius)
		: CObject(point, direction, speed, type = Rectangle) {}
	~CRect();
	void Update() override;
	void Draw() override;
	BOOL Collision() override;
};

class CStar : public CCircle
{
public :
	CStar(POINT point, POINT direction, LONG speed, LONG type, LONG radius)
		: CCircle(point, direction, speed, type = Star, radius) {}
	~CStar();
	void Update() override;
	void Draw() override;
	BOOL Collision() override;
};