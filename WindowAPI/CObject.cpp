#include "CObject.h"

CObject::CObject(POINT point, POINT direction, LONG speed, LONG type)
{
    this->point = point;
    this->direction = direction;
    this->speed = speed;
}

CObject::~CObject()
{
}


CCircle::~CCircle()
{
}

CRect::~CRect()
{
}

/// <summary>
/// 
/// </summary>

void CCircle::Update()
{
}

void CCircle::Draw()
{
}

BOOL CCircle::Collision()
{
    return 0;
}



void CRect::Update()
{
}

void CRect::Draw()
{
}

BOOL CRect::Collision()
{
    return 0;
}

CStar::~CStar()
{
}

void CStar::Update()
{
}

void CStar::Draw()
{
}

BOOL CStar::Collision()
{
    return 0;
}

