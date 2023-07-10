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
    this->selection = false;
    this->deleted = false;
    this->devided = false;
}

CObject::~CObject()
{

}

BOOL CObject::Collision(CObject& v2, RECT rectview)
{
    if (sqrt(pow((v2.getX() - this->point.x), 2)
        + pow((v2.getY() - this->point.y), 2)) <
        (this->radius) + (v2.getRadius()))
    {
        fPOINT temp = { this->direction.x, this->direction.y };
        this->direction.x = v2.direction.x;
        v2.direction.x = temp.x;

        this->direction.y = v2.direction.y;
        v2.direction.y = temp.y;
        this->angle[1] *= -1;
        v2.angle[1] *= -1;
        
        while (1)
        {
            if (sqrt(pow((v2.getX() - this->point.x), 2)
                + pow((v2.getY() - this->point.y), 2)) >
                (this->radius) + (v2.getRadius()))
                break;
            this->Update(rectview);
            v2.Update(rectview);
        }

        return TRUE;
    }

    return FALSE;
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

BOOL CObject::InObject(int mx, int my)
{
    double length_pts = sqrt((float)(pow(mx - point.x, 2) + pow(my - point.y, 2)));

    if (length_pts < this->radius) return TRUE;
    else FALSE;
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
    if (selection == TRUE)
        return;

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
    if (selection == TRUE)
        return;

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

    pt[0] = { -radius, -radius };
    pt[1] = { -radius, +radius };
    pt[2] = { +radius, +radius };
    pt[3] = { +radius, -radius };

    {
        POINT pt0 = { pt[0].x * cosf(angle2) - pt[0].y * sinf(angle2), pt[0].x * sinf(angle2) + pt[0].y * cosf(angle2) };
        POINT pt1 = { pt[1].x * cosf(angle2) - pt[1].y * sinf(angle2), pt[1].x * sinf(angle2) + pt[1].y * cosf(angle2) };
        POINT pt2 = { pt[2].x * cosf(angle2) - pt[2].y * sinf(angle2), pt[2].x * sinf(angle2) + pt[2].y * cosf(angle2) };
        POINT pt3 = { pt[3].x * cosf(angle2) - pt[3].y * sinf(angle2), pt[3].x * sinf(angle2) + pt[3].y * cosf(angle2) };

        POINT temp[4] = { {pt0.x + point.x, pt0.y + point.y},{pt1.x + point.x, pt1.y + point.y},
            {pt2.x + point.x, pt2.y + point.y} ,{pt3.x + point.x, pt3.y + point.y} };

        Polygon(hdc, temp, 4);
    }

    if (selection == TRUE)
        return;

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
    if (selection == TRUE)
        return;

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

    for (int i = 0; i < 10; i++)
    {
        temp_star[i].x = pt[i].x * cosf(angle2) - pt[i].y * sinf(angle2) + point.x,
        temp_star[i].y = pt[i].x * sinf(angle2) + pt[i].y * cosf(angle2) + point.y;
    }

    Polygon(hdc, temp_star, 10);

    if (selection == TRUE)
        return;

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

void CObject::setCollideFalse()
{
    iscollide = false;
}

BOOL CObject::isifSelect()
{
    return selection;
}

void CObject::setSelection()
{
    if (selection == false)
        selection = true;
    else if (selection == true)
        selection = false;
}

LONG CObject::getType()
{
    return type;
}

LONG CObject::getCompatibility()
{
    switch (this->type)
    {
    case Circle:
            return REct;
        break;
    case REct:
            return Star;
        break;
    case Star:
            return Circle;
        break;
    }
}

void CObject::setRadius(LONG r)
{
    radius = r;
}

void CObject::setDeleted()
{
    deleted = true;
}

BOOL CObject::isitDeleted()
{
    return deleted;
}

void CObject::setDevide()
{
    devided = true;
}

void CObject::setDevideFalse()
{
    devided = false;
}

BOOL CObject::isitDevide()
{
    return devided;
}

BOOL CObject::isitCollide()
{
    return iscollide;
}
