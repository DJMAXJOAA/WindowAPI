#include "CObject.h"
#include "WindowAPI.h"

CObject::CObject(POINT point, LONG type)
{
    this->angle[0] = Random(1.0, 359.0);
    this->angle[1] = this->angle[0];
    float angle2 = this->angle[0] * PI / 180;

    this->point = point; // 커서 위치에 생성
    this->direction = fPOINT{ cosf(angle2), sinf(angle2)};
    this->speed = 10;
    this->type = type;
    this->radius = 40;
    this->iscollide = false;
}

CObject::~CObject()
{
}

BOOL CObject::Collision(CObject& v2)
{
    if (sqrt(pow((v2.getX() - this->point.x) , 2)
        + pow((v2.getY() - this->point.y), 2)) < (this->radius+this->speed) + (v2.getRadius() + v2.getSpeed()))
    {
        fPOINT temp = { this->direction.x, this->direction.y };
        this->direction.x = v2.direction.x;
        v2.direction.x = temp.x;

        this->direction.y = v2.direction.y;
        v2.direction.y = temp.y;
        this->angle[1] *= -1;
        v2.angle[1] *= -1;

        this->iscollide = true;
        v2.setCollide();

        return TRUE;
    }
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
        this->direction.x *= -1;
        this->point.x -= 20;
    }
    else if (this->point.x - 20 - this->direction.x * this->speed < rectView.left)
    {
        this->direction.x *= -1;
        this->point.x += 20;
    }
    else if (this->point.y + 10 + this->direction.y * this->speed > rectView.bottom)
    {
        this->direction.y *= -1;
        this->point.y -= 20;
    }
    else if (this->point.y - 10 - this->direction.y * this->speed < rectView.top)
    {
        this->direction.y *= -1;
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


void CRect::Update(RECT rectView)
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

void CRect::Draw(HDC hdc)
{
    float angle2 = angle[1] * PI / 180;

    {
        POINT pt0 = { pt[0].x * cosf(angle2) - pt[0].y * sinf(angle2), pt[0].x * sinf(angle2) + pt[0].y * cosf(angle2) };
        POINT pt1 = { pt[1].x * cosf(angle2) - pt[1].y * sinf(angle2), pt[1].x * sinf(angle2) + pt[1].y * cosf(angle2) };
        POINT pt2 = { pt[2].x * cosf(angle2) - pt[2].y * sinf(angle2), pt[2].x * sinf(angle2) + pt[2].y * cosf(angle2) };
        POINT pt3 = { pt[3].x * cosf(angle2) - pt[3].y * sinf(angle2), pt[3].x * sinf(angle2) + pt[3].y * cosf(angle2) };

        POINT temp[4] = { {pt0.x + point.x, pt0.y + point.y},{pt1.x + point.x, pt1.y + point.y},
            {pt2.x + point.x, pt2.y + point.y} ,{pt3.x + point.x, pt3.y + point.y} };

        Polygon(hdc, temp, 4);
    }


    if(angle[1] + angle[0] <= 360.0)
        angle[1] += angle[0];
    else
    {
        angle[1] = angle[0] - (360.0 - angle[1]);
    }
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
    float angle2 = angle[1] * PI / 180;

    POINT temp_star[10];
    for (int i = 0; i < 10; i++)
    {
        temp_star[i].x = pt[i].x * cosf(angle2) - pt[i].y * sinf(angle2) + point.x,
        temp_star[i].y = pt[i].x * sinf(angle2) + pt[i].y * cosf(angle2) + point.y;
    }

    Polygon(hdc, temp_star, 10);

    if (angle[1] + angle[0] <= 360.0)
        angle[1] += angle[0];
    else
    {
        angle[1] = angle[0] - (360.0 - angle[1]);
    }
}

double CObject::GetAngle(CObject &v2)
{
	double dot = direction.x * v2.point.x + direction.y * v2.point.y;
	
    double v1_length = 1;
    double v2_length = sqrt((pow(v2.point.x - point.x, 2)) + (pow(v2.point.y - point.y, 2)));

	return (acos(dot / (v1_length * v2_length)) * (180.0 / PI));
}

LONG CObject::getX()
{
    return point.x;
}

LONG CObject::getY()
{
    return point.y;
}

LONG CObject::getSpeed()
{
    return speed;
}

LONG CObject::getRadius()
{
    return radius;
}

void CObject::setCollide()
{
    iscollide = true;
}

BOOL CObject::isitCollide()
{
    return iscollide;
}
