#pragma once  
#include <windows.h>  

// Интерфейс IShow
class IShow
{
public:
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual ~IShow() {}
};

class Location
{
protected:
    int x, y;
public:
    Location(int InitX, int InitY);
    int GetX();
    int GetY();
    void SetX(int NewX);
    void SetY(int NewY);
};

class Point : public Location
{
protected:
    bool visible;
    int radius;
    COLORREF borderColor;
    COLORREF fillColor;
public:
    Point(int InitX, int InitY);
    virtual ~Point() {}

    void MoveTo(int NewX, int NewY);
    bool IsVisible();
    void SetVisible(bool isVisible);
    int GetRadius();
    void SetRadius(int rad);

    void SetBorderColor(COLORREF color) { borderColor = color; }
    void SetFillColor(COLORREF color) { fillColor = color; }
    COLORREF GetBorderColor() const { return borderColor; }
    COLORREF GetFillColor() const { return fillColor; }

    virtual void Draw() = 0;
    void Erase();
};

// Первая ветка: Point → Circle (игровое поле)
class Circle : public Point, public IShow
{
protected:
    int lineWidth;
public:
    Circle(int InitX, int InitY, int rad);
    void Show() override;
    void Hide() override;
    void Draw() override;
    void SetLineWidth(int width) { lineWidth = width; }
};

// Вторая ветка: Point → Figure → (Bonus, Enemy, Hero)
class Figure : public Point, public IShow
{
protected:
    int lineWidth;
public:
    Figure(int InitX, int InitY, int rad = 0);
    void SetLineWidth(int width) { lineWidth = width; }
    int GetLineWidth() const { return lineWidth; }
};

// Figure → Enemy
class Enemy : public Figure
{
public:
    Enemy(int InitX, int InitY);
    void Show() override;
    void Hide() override;
    void Move(int centerX, int centerY, int circleRadius);
    void Draw() override;
};

// Figure → Bonus
class Bonus : public Figure
{
public:
    Bonus(int InitX, int InitY);
    void Show() override;
    void Hide() override;
    void Draw() override;
};

// Figure → Hero
class Hero : public Figure
{
protected:
    int score;
public:
    Hero(int InitX, int InitY, int currentScore = 0);
    void AddScore() { score++; }
    int GetScore() const { return score; }
    void SetScore(int newScore) { score = newScore; }

    void Show() override;
    void Hide() override;
    virtual void DrawShape() = 0;
    void Draw() override;
};

// Hero → CircleHero
class CircleHero : public Hero
{
public:
    CircleHero(int InitX, int InitY, int currentScore = 0);
    void DrawShape() override;
};

// Hero → SquareHero
class SquareHero : public Hero
{
public:
    SquareHero(int InitX, int InitY, int currentScore = 0);
    void DrawShape() override;
};

// Hero → TriangleHero
class TriangleHero : public Hero
{
public:
    TriangleHero(int InitX, int InitY, int currentScore = 0);
    void DrawShape() override;
};
