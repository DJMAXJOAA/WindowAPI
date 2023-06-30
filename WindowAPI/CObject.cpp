#include "CObject.h"
#include "WindowAPI.h"

CObject::CObject(POINT point, LONG type)
{
    int minus;
    if (Random(-1, 1) > 0) minus = 1;
    else minus = -1;

    this->angle[0] =/* Random(10.0, 45.0);*/ 15.0;
    this->angle[1] = this->angle[0];
    float angle2 = this->angle[0] * PI / 180;

    this->point = point; // 커서 위치에 생성
    this->direction = fPOINT{ cosf(angle2), sinf(angle2)};
    this->speed = Random(5.0, 10.0);
    this->type = type;
}

CObject::~CObject()
{
}

float CObject::Random(float min, float max)
{
    std::random_device rand;
    std::mt19937_64 random(rand());
    std::uniform_real_distribution<float> dist1(min, max);
   
    float temp = 0;
    while (!temp)
    {
        temp = dist1(random);
    }

    return temp;
}

LONG CObject::PrintInfo()
{
    return (LONG)angle;
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

void CCircle::Update(RECT rectView)
{
    if (this->point.x + 10 + this->direction.x * this->speed > rectView.right)
    {
        this->direction.x *= -1.0;
        this->point.x -= 20;
    }
    else if (this->point.x - 10 - this->direction.x * this->speed < rectView.left)
    {
        this->direction.x *= -1.0;
        this->point.x += 20;
    }
    else if (this->point.y + 10 + this->direction.y * this->speed > rectView.bottom)
    {
        this->direction.y *= -1.0;
        this->point.y -= 20;
    }
    else if (this->point.y - 10 - this->direction.y * this->speed < rectView.top)
    {
        this->direction.y *= -1.0;
        this->point.y += 20;
    }
    this->point.x += this->direction.x * this->speed;
    this->point.y += this->direction.y * this->speed;
}

void CCircle::Draw(HDC hdc)
{
    POINT a = { point.x - radius, point.y - radius };
    POINT b = { point.x + radius, point.y + radius };
    Ellipse(hdc, a.x, a.y, b.x, b.y);
}

BOOL CCircle::Collision()
{
    return 0;
}



void CRect::Update(RECT rectView)
{
    //if (this->point.x + 10 + this->direction.x * this->speed > rectView.right)
    //{
    //    this->direction.x *= -1.0;
    //    this->point.x -= 20;
    //}
    //else if (this->point.x - 10 - this->direction.x * this->speed < rectView.left)
    //{
    //    this->direction.x *= -1.0;
    //    this->point.x += 20;
    //}
    //else if (this->point.y + 10 + this->direction.y * this->speed > rectView.bottom)
    //{
    //    this->direction.y *= -1.0;
    //    this->point.y -= 20;
    //}
    //else if (this->point.y - 10 - this->direction.y * this->speed < rectView.top)
    //{
    //    this->direction.y *= -1.0;
    //    this->point.y += 20;
    //}
    //this->point.x += this->direction.x * this->speed;
    //this->point.y += this->direction.y * this->speed;
}

void CRect::Draw(HDC hdc)
{
    pt[0] = { point.x - width, point.y - height };
    pt[1] = { point.x - width, point.y + height };
    pt[2] = { point.x + width, point.y + height };
    pt[3] = { point.x + width, point.y - height };

    float angle2 = angle[1] * PI / 180;

    {
        POINT pt0 = { (-height/2) * cosf(angle2) - (-height/2) * sinf(angle2), (-height/2)*sinf(angle2) + (-height/2)*cosf(angle2)};
        POINT pt1 = { (-height/2) * cosf(angle2) - height/2 * sinf(angle2), (-height/2) * sinf(angle2) + height/2*cosf(angle2)};
        POINT pt2 = { height/2 * cosf(angle2) - height/2 * sinf(angle2), height/2 * sinf(angle2) + height/2 * cosf(angle2)};
        POINT pt3 = { height/2 * cosf(angle2) - (-height/2)*sinf(angle2), height/2 * sinf(angle2) + (-height/2)*cosf(angle2)};

        POINT temp[4] = { {pt0.x+pt[0].x, pt0.y + pt[0].y},{pt1.x + pt[1].x, pt1.y + pt[1].y},
            {pt2.x + pt[2].x, pt2.y + pt[2].y} ,{pt3.x + pt[3].x, pt3.y + pt[3].y}};

        Polygon(hdc, temp, 4);
    }

    if(angle[1] + angle[0] <= 360.0)
        angle[1] += angle[0];
    else
    {
        angle[1] = angle[0] - (360.0 - angle[1]);
    }
}

BOOL CRect::Collision()
{
    return 0;
}

CStar::~CStar()
{
}

void CStar::Update(RECT rectView)
{
    if (this->point.x + 10 + this->direction.x * this->speed > rectView.right)
    {
        this->direction.x *= -1.0;
        this->point.x -= 20;
    }
    else if (this->point.x - 10 - this->direction.x * this->speed < rectView.left)
    {
        this->direction.x *= -1.0;
        this->point.x += 20;
    }
    else if (this->point.y + 10 + this->direction.y * this->speed > rectView.bottom)
    {
        this->direction.y *= -1.0;
        this->point.y -= 20;
    }
    else if (this->point.y - 10 - this->direction.y * this->speed < rectView.top)
    {
        this->direction.y *= -1.0;
        this->point.y += 20;
    }
    this->point.x += this->direction.x * this->speed;
    this->point.y += this->direction.y * this->speed;
}

void CStar::Draw(HDC hdc)
{
    DrawStar(hdc, point, radius);
}

BOOL CStar::Collision()
{
    return 0;
}

