#include "ring_traffic.h"
#include <cstdlib>
#include <ctime>

extern HDC hdc;

//start Location::Location 
Location::Location(int InitX, int InitY) 
{
	x = InitX;
	y = InitY;
}//end Location::Location

//start Location::~Location
Location::~Location() 
{
}//end Location::~Location

//start Location::GetX
int Location::GetX() 
{
	return x; 
}//end Location::GetX

//start Location::GetY
int Location::GetY() 
{
	return y; 
}//end Location::GetY

//start Location::SetX
void Location::SetX(int NewX) 
{
	x = NewX; 
}//end Location::SetX

//start Location::SetY
void Location::SetY(int NewY) 
{ 
	y = NewY; 
}//end Location::SetY

//start Point::Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY) 
{
	visible = false;
}//end Point::Point

//start Point::~Point
Point::~Point() 
{
}//end Point::~Point

//start Point::MoveTo
void Point::MoveTo(int NewX, int NewY) 
{
	Hide();
	x = NewX;
	y = NewY;
	Show();
}//end Point::MoveTo

//start Figure::Figure  
Figure::Figure(int InitX, int InitY) : Point(InitX, InitY) 
{
	id = 0;
}//end Figure::Figure  

//start Figure::~Figure
Figure::~Figure() 
{
}//end Figure::~Figure

//start Circle::Circle
Circle::Circle(int InitX, int InitY, int rad) : Figure(InitX, InitY), radius(rad) 
{
	visible = true;
}//end Circle::Circle

//start Circle::~Circle
Circle::~Circle() 
{
}//end Circle::~Circle

//start Circle::Show
void Circle::Show() 
{
	//Черная окружность
	HPEN PenB = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(hdc, PenB);
	Arc(hdc, x - radius, y - radius, x + radius, y + radius, x - radius, y, x - radius, y);
	DeleteObject(PenB);
}//end Circle::Show

//start Circle::Hide
void Circle::Hide() 
{
	//Стираем окружность, рисуя ее белой
	HPEN WhitePen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	SelectObject(hdc, WhitePen);
	Arc(hdc, x - radius, y - radius, x + radius, y + radius, x - radius, y, x - radius, y);
	DeleteObject(WhitePen);
}//end Circle::Hide

//start Hero::Hero 
Hero::Hero(int InitX, int InitY) : Circle(InitX, InitY, 3) 
{
	visible = false;
}//end Hero::Hero

//start Hero::~Hero
Hero::~Hero() 
{
}//end Hero::~Hero

//start Hero::Show
void Hero::Show() 
{
	visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HBRUSH Brush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Hero::Show

//start Hero::Hide
void Hero::Hide() 
{
	visible = false;
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH Brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Hero::Hide

//start Collisions::Collisions 
Collisions::Collisions(int InitX, int InitY) : Point(InitX, InitY), IDraw() 
{
	id = 0;
}//end Collisions::Collisions

//start Collisions::~Collisions
Collisions::~Collisions() 
{
}//end Collisions::~Collisions

//start Collisions::Show
void Collisions::Show() 
{
	visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HBRUSH Brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - 3, y - 3, x + 3, y + 3);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Collisions::Show

//start Collisions::Hide
void Collisions::Hide() 
{
	visible = false;
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH Brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - 3, y - 3, x + 3, y + 3);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Collisions::Hide

//start Enemy::Enemy
Enemy::Enemy(int InitX, int InitY) : Collisions(InitX, InitY) 
{
	visible=false;
}//end Enemy::Enemy

//start Enemy::~Enemy
Enemy::~Enemy() 
{
}//end Enemy::~Enemy

//start Enemy::Show
void Enemy::Show() 
{
	visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HBRUSH Brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - 3, y - 3, x + 3, y + 3);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Enemy::Show

//start Enemy::MoveEnemy
void Enemy::MoveEnemy(int centerX, int centerY, int circleRadius) 
{
	//Генерируем число от 0 до 7
	int direction = rand() % 8;
	int newX = x, newY = y;
	switch (direction) 
	{
	case 0: newX += 3; break;  // Вправо
	case 1: newX -= 3; break;  // Влево
	case 2: newY += 3; break;  // Вниз
	case 3: newY -= 3; break;  // Вверх
	case 4: newX += 3; newY += 3; break;  // Вправо-вниз
	case 5: newX += 3; newY -= 3; break;  // Вправо-вверх
	case 6: newX -= 3; newY += 3; break;  // Влево-вниз
	case 7: newX -= 3; newY -= 3; break;  // Влево-вверх
	}//end switch

	//Проверка что враг оставался внутри круга
	int dx = newX - centerX;
	int dy = newY - centerY;
	//Проверка на расстояние от центра
	if (dx * dx + dy * dy <= (circleRadius - 3) * (circleRadius - 3)) MoveTo(newX, newY);
}//end Enemy::MoveEnemy

//start Bonus::Bonus
Bonus::Bonus(int InitX, int InitY) : Collisions(InitX, InitY)
{
	visible = false;
	radius = 5;
}//end Bonus::Bonus

//start Bonus::~Bonus
Bonus::~Bonus() 
{
}//end Bonus::~Bonus

//start Bonus::Show 
void Bonus::Show() 
{
	if (visible) {
		HPEN PenY = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		HBRUSH BrushY = CreateSolidBrush(RGB(255, 255, 0));
		SelectObject(hdc, PenY);
		SelectObject(hdc, BrushY);
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
		DeleteObject(PenY);
		DeleteObject(BrushY);
	}//end if
}//end Bonus::Show

//start Bonus::Hide
void Bonus::Hide() {
	visible = false;
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH Brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	DeleteObject(Pen);
	DeleteObject(Brush);
}//end Bonus::Hide

//start Bonus::isVisible
bool Bonus::isVisible() 
{ 
	return visible; 
}//end Bonus::isVisible

//start Bonus::SetVisible
void Bonus::SetVisible(bool v) 
{
	visible = v; 
}//end Bonus::SetVisible