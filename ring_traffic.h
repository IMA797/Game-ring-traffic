#pragma once
#include <windows.h>

class Location 
{
protected:
	int x, y;
public:
	Location(int InitX, int InitY);
	~Location();
	int GetX();
	int GetY();
	void SetX(int NewX);
	void SetY(int NewY);
};

class IDraw 
{
public:
	virtual void Show() = 0;
};

//Абстрактный
class Point : public Location
{
protected:
	bool visible;
public:
	Point(int InitX, int InitY);
	~Point();
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void MoveEnemy(int centerX, int centerY, int radius) {};
	void MoveTo(int NewX, int NewY);
};

class Figure : public Point 
{
public:
	Figure(int InitX, int InitY);
	~Figure();
	int id;
	virtual void Show() = 0;
	virtual void Hide() = 0;
};

class Circle : public Figure 
{
public:
	int radius;
public:
	Circle(int InitX, int InitY, int rad);
	~Circle();
	void Show() override;
	void Hide() override;
};

class Hero : public Circle 
{
public:
	Hero(int InitX, int InitY);
	~Hero();
	void Show() override;
	void Hide() override;
};

class Collisions : public Point, public IDraw 
{
public:
	int id;
	Collisions(int InitX, int InitY);
	~Collisions();
	void Show() override;
	void Hide() override;
};

class Enemy : public Collisions 
{
public:
	Enemy(int InitX, int InitY);
	~Enemy();
	void Show() override;
	void MoveEnemy(int centerX, int centerY, int radius) override;
};

class Bonus : public Collisions
{
protected:
	int radius;
public:
	Bonus(int InitX, int InitY);
	~Bonus();
	void Show() override;
	void Hide() override;
	bool isVisible();
	void SetVisible(bool v);
};