#include "ring_traffic.h"
#include <cstdlib>
#include <ctime>

extern HDC hdc;

// Location
Location::Location(int InitX, int InitY) : x(InitX), y(InitY) {}
int Location::GetX() { return x; }
int Location::GetY() { return y; }
void Location::SetX(int NewX) { x = NewX; }
void Location::SetY(int NewY) { y = NewY; }

// Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false;
    radius = 0;
    borderColor = RGB(255, 255, 255);
    fillColor = RGB(255, 255, 255);
}

void Point::MoveTo(int NewX, int NewY)
{
    bool wasVisible = visible;
    if (wasVisible)
    {
        Erase();
        visible = false;
    }

    x = NewX;
    y = NewY;

    if (wasVisible)
    {
        Draw();
        visible = true;
    }
}

bool Point::IsVisible() { return visible; }
void Point::SetVisible(bool isVisible) { visible = isVisible; }
int Point::GetRadius() { return radius; }
void Point::SetRadius(int rad) { radius = rad; }

void Point::Erase()
{
    if (!visible) return;

    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Rectangle(hdc, cx - r - 2, cy - r - 2, cx + r + 2, cy + r + 2);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);

    visible = false;
}

// Circle (игровое поле)
Circle::Circle(int InitX, int InitY, int rad) : Point(InitX, InitY)
{
    SetRadius(rad);
    SetBorderColor(RGB(0, 0, 0));
    lineWidth = 3;
    SetVisible(true);
}

void Circle::Show()
{
    visible = true;
    Draw();
}

void Circle::Hide()
{
    if (!visible) return;

    HPEN Pen = CreatePen(PS_SOLID, lineWidth + 2, RGB(255, 255, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, Pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, GetX() - GetRadius(), GetY() - GetRadius(),
        GetX() + GetRadius(), GetY() + GetRadius());

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(Pen);

    visible = false;
}

void Circle::Draw()
{
    HPEN Pen = CreatePen(PS_SOLID, lineWidth, GetBorderColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, Pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, GetX() - GetRadius(), GetY() - GetRadius(),
        GetX() + GetRadius(), GetY() + GetRadius());

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(Pen);
}

// Figure
Figure::Figure(int InitX, int InitY, int rad) : Point(InitX, InitY)
{
    SetRadius(rad);
    lineWidth = 2;
}

// Enemy
Enemy::Enemy(int InitX, int InitY) : Figure(InitX, InitY, 3)
{
    SetBorderColor(RGB(255, 0, 0));
    SetFillColor(RGB(255, 0, 0));
}

void Enemy::Show()
{
    visible = true;
    Draw();
}

void Enemy::Hide()
{
    Erase();
}

void Enemy::Move(int centerX, int centerY, int circleRadius)
{
    int newX = GetX();
    int newY = GetY();
    int dir = rand() % 8;

    switch (dir)
    {
    case 0: newX += 3; break;
    case 1: newX -= 3; break;
    case 2: newY += 3; break;
    case 3: newY -= 3; break;
    case 4: newX += 3; newY += 3; break;
    case 5: newX += 3; newY -= 3; break;
    case 6: newX -= 3; newY += 3; break;
    case 7: newX -= 3; newY -= 3; break;
    }

    int dx = newX - centerX;
    int dy = newY - centerY;
    int safeR = circleRadius - GetRadius();

    if (dx * dx + dy * dy <= safeR * safeR)
    {
        MoveTo(newX, newY);
    }
}

void Enemy::Draw()
{
    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, GetLineWidth(), GetBorderColor());
    HBRUSH brush = CreateSolidBrush(GetFillColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

// Bonus
Bonus::Bonus(int InitX, int InitY) : Figure(InitX, InitY, 5)
{
    SetBorderColor(RGB(255, 255, 0));
    SetFillColor(RGB(255, 255, 0));
}

void Bonus::Show()
{
    visible = true;
    Draw();
}

void Bonus::Hide()
{
    Erase();
}

void Bonus::Draw()
{
    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, GetLineWidth(), GetBorderColor());
    HBRUSH brush = CreateSolidBrush(GetFillColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

// Hero
Hero::Hero(int InitX, int InitY, int currentScore) : Figure(InitX, InitY, 5)
{
    score = currentScore;
    visible = true;
}

void Hero::Show()
{
    visible = true;
    Draw();
}

void Hero::Hide()
{
    Erase();
}

void Hero::Draw()
{
    DrawShape();
}

// CircleHero
CircleHero::CircleHero(int InitX, int InitY, int currentScore) : Hero(InitX, InitY, currentScore)
{
    SetRadius(5);
    SetBorderColor(RGB(0, 255, 0));
    SetFillColor(RGB(0, 255, 0));
    SetLineWidth(2);
}

void CircleHero::DrawShape()
{
    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, GetLineWidth(), GetBorderColor());
    HBRUSH brush = CreateSolidBrush(GetFillColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

// SquareHero
SquareHero::SquareHero(int InitX, int InitY, int currentScore) : Hero(InitX, InitY, currentScore)
{
    SetRadius(6);
    SetBorderColor(RGB(255, 165, 0));
    SetFillColor(RGB(255, 165, 0));
    SetLineWidth(2);
}

void SquareHero::DrawShape()
{
    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, GetLineWidth(), GetBorderColor());
    HBRUSH brush = CreateSolidBrush(GetFillColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Rectangle(hdc, cx - r, cy - r, cx + r, cy + r);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

// TriangleHero
TriangleHero::TriangleHero(int InitX, int InitY, int currentScore) : Hero(InitX, InitY, currentScore)
{
    SetRadius(7);
    SetBorderColor(RGB(255, 0, 255));
    SetFillColor(RGB(255, 0, 255));
    SetLineWidth(2);
}

void TriangleHero::DrawShape()
{
    int r = GetRadius();
    int cx = GetX();
    int cy = GetY();

    HPEN pen = CreatePen(PS_SOLID, GetLineWidth(), GetBorderColor());
    HBRUSH brush = CreateSolidBrush(GetFillColor());
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    POINT t[3] = {
        {cx, cy - r},
        {cx - r, cy + r},
        {cx + r, cy + r}
    };
    Polygon(hdc, t, 3);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}
