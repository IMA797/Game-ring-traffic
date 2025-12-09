#include <iostream>
#include <Windows.h>
#include <ctime>
#include "ring_traffic.h"

using namespace std;

HDC hdc;

// Функция вычисления квадрата расстояния между двумя точками
int Dist2(int x1, int y1, int x2, int y2);

int main()
{
    setlocale(LC_ALL, "RU");

    srand((unsigned int)time(0));

    // Координаты центра игрового поля
    int x0 = 500;
    int y0 = 350;

    // Радиус игрового поля (круга)
    int radius = 250;

    // Счетчик собранных бонусов
    int count = 0;

    // Получение дескриптора окна консоли
    HWND hwnd = GetConsoleWindow();
    if (!hwnd)
        return 1;  // Ошибка: окно консоли не найдено

    // Получение контекста устройства для рисования
    hdc = GetWindowDC(hwnd);
    if (!hdc)
        return 2;  // Ошибка: контекст устройства не получен

    // Создание игрового поля (круг) - ИЗМЕНЕНО: теперь через указатель
    Circle* gameField = new Circle(x0, y0, radius);
    gameField->Show();  // Отображение игрового поля

    // Создание героя (начальная форма - круг) 
    Hero* player = new CircleHero(x0, y0);
    player->Show();  // Отображение героя

    // Враги
    Enemy* enemies[4] = {
        new Enemy(x0 + 100, y0 + 100),   // Враг 1: справа-внизу от центра
        new Enemy(x0 - 150, y0 - 50),    // Враг 2: слева-вверху от центра  
        new Enemy(x0 + 50, y0 - 120),    // Враг 3: справа-вверху от центра
        new Enemy(x0 - 100, y0 + 100)    // Враг 4: слева-внизу от центра
    };

    // Отображение всех врагов
    for (int i = 0; i < 4; i++)
        enemies[i]->Show();

    // Создание бонуса
    Bonus* bonus = new Bonus(0, 0);

    // Размещение бонуса в случайной позиции вблизи центра
    bonus->MoveTo(x0 + rand() % 100 - 50, y0 + rand() % 100 - 50);
    bonus->Show();  // Отображение бонуса

    // Счетчик кадров 
    int frame = 0;

    while (true)
    {
        // Проверка нажатия клавиши ESC для выхода из игры 
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        // Получение текущих координат героя 
        int px = player->GetX();
        int py = player->GetY();

        // Обработка управления героем стрелками 
        if (GetAsyncKeyState(VK_UP) & 0x8000)
            py -= 5;    // Движение вверх
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            py += 5;    // Движение вниз
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            px -= 5;    // Движение влево
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            px += 5;    // Движение вправо

        // Проверка, чтобы герой не выходил за границы игрового поля (круга) 
        if (Dist2(px, py, x0, y0) <= (radius - player->GetRadius()) * (radius - player->GetRadius()))
            player->MoveTo(px, py);  // Перемещение героя, если он остается внутри поля

        // Проверяем столкновение с врагами
        bool hit = false;  // Флаг столкновения
        for (int i = 0; i < 4; i++)
        {
            int dist = (player->GetRadius() + enemies[i]->GetRadius());
            dist *= dist;

            // Проверка столкновения
            if (Dist2(player->GetX(), player->GetY(),
                enemies[i]->GetX(), enemies[i]->GetY()) <= dist)
            {
                hit = true;  // Столкновение произошло
                break;
            }
        }

        // Если было столкновение с врагом - конец игры 
        if (hit)
        {
            cout << "Столкновение с врагом!\n";
            break;
        }

        if (bonus->IsVisible())  // Проверяем, видим ли бонус 
        {
            // Вычисляем квадрат суммы радиусов (герой + бонус)
            int dist = (player->GetRadius() + bonus->GetRadius());
            dist *= dist;

            // Проверяем сбор бонусов
            if (Dist2(player->GetX(), player->GetY(),
                bonus->GetX(), bonus->GetY()) <= dist)
            {
                count++;  // Увеличение счетчика бонусов

                // Сохранение старого счета для проверки трансформации
                int oldScore = player->GetScore();

                // Увеличение счета героя
                player->AddScore();

                // Трансформация из круга в квадрат (при 10+ очках)
                if (oldScore < 10 && player->GetScore() >= 10)
                {
                    // Сохранение текущих координат героя
                    int hx = player->GetX();
                    int hy = player->GetY();

                    // Удаление старого героя (круга)
                    player->Hide();
                    delete player;

                    player = new SquareHero(hx, hy, oldScore + 1);
                    player->Show();
                }
                // Трансформация из квадрата в треугольник (при 21+ очках)
                else if (oldScore < 21 && player->GetScore() >= 21)
                {
                    // Сохранение текущих координат героя
                    int hx = player->GetX();
                    int hy = player->GetY();

                    // Удаление старого героя (квадрата)
                    player->Hide();
                    delete player;

                    // Создание нового героя (треугольника)
                    player = new TriangleHero(hx, hy, oldScore + 1);
                    player->Show();      // Отображение новой формы
                }

                bonus->Hide();  // Скрытие текущего бонуса

                // Размещение бонуса в новой случайной позиции
                bonus->MoveTo(x0 + rand() % 200 - 100,
                    y0 + rand() % 200 - 100);
                bonus->Show();  // Отображение бонуса в новой позиции
            }
        }

        // Враги двигаются каждый 10-й кадр (500 мс) 
        if (++frame % 10 == 0)
        {
            for (int i = 0; i < 4; i++)
                enemies[i]->Move(x0, y0, radius);  // Движение врага
        }

        Sleep(50);
    }

    for (int i = 0; i < 4; i++)
        delete enemies[i];

    delete bonus;

    delete player;

    delete gameField;

    cout << "Игра окончена! Ваш счет: " << count << endl;

    return 0;
}
