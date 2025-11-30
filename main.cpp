#include <iostream>
#include <Windows.h>
#include <ctime>
#include "ring_traffic.h"

using namespace std;

HDC hdc;


int Dist2(int x1, int y1, int x2, int y2);



int main()
{
    setlocale(LC_ALL, "RU");
    srand(time(0));

    int x0 = 500;
    int y0 = 350;
    int radius = 250;
    int score = 0;

    HWND hwnd = GetConsoleWindow();
    if (!hwnd) return 1;
    hdc = GetWindowDC(hwnd);
    if (!hdc) return 2;

    Circle gameField(x0, y0, radius);
    Hero player(x0, y0);

    gameField.Show();
    player.Show();

    Point* enemies[4] =
    {
        new Enemy(x0 + 100, y0 + 100),
        new Enemy(x0 - 150, y0 - 50),
        new Enemy(x0 + 50, y0 - 120),
        new Enemy(x0 - 100, y0 + 100)
    };

    for (int i = 0; i < 4; i++)
        enemies[i]->Show();

    Bonus* bonus = new Bonus(0, 0);
    bonus->MoveTo(x0 + rand() % 100 - 50, y0 + rand() % 100 - 50);
    bonus->SetVisible(true);
    bonus->Show();

    cout << "Управление: стрелки для движения, ESC — выход.\n";

    int frame = 0;

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        int px = player.GetX();
        int py = player.GetY();

        if (GetAsyncKeyState(VK_UP) & 0x8000) py -= 5;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) py += 5;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) px -= 5;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) px += 5;

        if (Dist2(px, py, x0, y0) <= (radius - 5) * (radius - 5))
            player.MoveTo(px, py);

        bool hitEnemy = false;

        for (int i = 0; i < 4; i++)
        {
            if (Dist2(player.GetX(), player.GetY(),
                enemies[i]->GetX(), enemies[i]->GetY()) <= 36)
            {
                hitEnemy = true;
                break;
            }
        }

        if (hitEnemy)
            player.MoveTo(x0, y0);

        if (bonus->isVisible() &&
            Dist2(player.GetX(), player.GetY(), bonus->GetX(), bonus->GetY()) <= 64)
        {
            score++;
            bonus->Hide();

            bonus->MoveTo(x0 + rand() % 200 - 100,
                y0 + rand() % 200 - 100);

            bonus->SetVisible(true);
            bonus->Show();
        }

        if (++frame % 10 == 0)
        {
            for (int i = 0; i < 4; i++)
                enemies[i]->MoveEnemy(x0, y0, radius);
        }

        Sleep(50);
    }

    for (int i = 0; i < 4; i++)
        delete enemies[i];

    delete bonus;

    cout << "Игра окончена! Собрано бонусов: " << score << endl;
    return 0;
}
