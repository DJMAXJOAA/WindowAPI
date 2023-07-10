#pragma once
#include "framework.h"
#define PI 3.14159

enum Type
{
	Circle, REct, Star
	// 별 > 원 > 사각형 > 별
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
	LONG radius;
	BOOL iscollide;
	BOOL selection;
	BOOL deleted;
	BOOL devided;
public:
	CObject(POINT point, LONG type);
	~CObject();
	virtual void Update(RECT rectView) = 0;
	virtual void Draw(HDC hdc) = 0;
	float Random(float min, float max);
	BOOL Collision(CObject& v2, RECT rectview);
	LONG PrintInfo();
	BOOL InObject(int mx, int my);

	double GetAngle(CObject &v2);
	LONG getX();
	LONG getY();
	LONG getSpeed();
	LONG getRadius();
	BOOL isitCollide();
	void setCollide();
	void setCollideFalse();
	BOOL isifSelect();
	void setSelection();
	LONG getType();
	LONG getCompatibility();
	void setRadius(LONG r);
	void setDeleted();
	BOOL isitDeleted();
	void setDevide();
	void setDevideFalse();
	BOOL isitDevide();
};

class CCircle : public CObject
{
public :
	CCircle(POINT point)
		: CObject(point, type = Circle) {
		type = Circle;
		radius = 40;
	}
	~CCircle();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
};

class CRect : public CObject
{
private :
	POINT pt[4];
	LONG height;
public:
	CRect(POINT point)
		: CObject(point, type = REct) {
		type = REct;
		height = radius;
		pt[0] = { -radius, -radius };
		pt[1] = { -radius, +radius };
		pt[2] = { +radius, +radius };
		pt[3] = { +radius, -radius };
	}
	~CRect();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
	
};

class CStar : public CObject
{
private:
	POINT pt[10];
public :
	CStar(POINT point)
		: CObject(point, type = Circle) { 
		radius = 40; 
		type = Star;

		double t_angle = 2.0 * PI / (double)10;
		double temp_angle = 0;
		for (int i = 0; i < 10; i++)
		{
			if (i % 2 == 1)
			{
				pt[i].x = (sin(temp_angle) * radius / 2);
				pt[i].y = (cos(temp_angle) * radius / 2);
			}
			else
			{
				pt[i].x = (sin(temp_angle) * radius);
				pt[i].y = (cos(temp_angle) * radius);
			}
			temp_angle += t_angle;
		}
	}
	~CStar();
	virtual void Update(RECT rectView) override;
	virtual void Draw(HDC hdc) override;
};