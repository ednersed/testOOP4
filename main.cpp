#include <windows.h>
#include <iostream>
#include <locale>
#include <cmath>
using namespace std;

#include "Header.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc;

void SetConsoleSize(int width, int height)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    COORD bufSize = { (SHORT)width, (SHORT)height };
    SetConsoleScreenBufferSize(hOut, bufSize);
    SMALL_RECT winRect = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &winRect);
    HWND hwnd = GetConsoleWindow();
    if (hwnd)
        MoveWindow(hwnd, 30, 30, width * 9, height * 16, TRUE);
}

void SetupConsoleFont() {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

// ??????? ???????? ???????????? ? ??????
void CheckMineCollisions(Mina* mines[], int mineCount, Tank* tank)
{
    if (tank->IsDestroyed()) return;

    for (int i = 0; i < mineCount; i++)
    {
        if (!mines[i]->IsExploded() && mines[i]->CheckCollision(tank))
        {
            cout << "BOOM! Tank hit a mine!" << endl;
            tank->TakeDamage(mines[i]->GetDamage());
            mines[i]->Explode();
        }
    }
}

// ??????? ???????? ???????????? ? ??????????????
void CheckRepairCollisions(Remont* repairs[], int repairCount, Tank* tank)
{
    if (tank->IsDestroyed()) return;

    for (int i = 0; i < repairCount; i++)
    {
        if (!repairs[i]->IsUsed() && repairs[i]->CheckCollision(tank))
        {
            tank->Heal(repairs[i]->GetHealAmount());
            repairs[i]->Use();
        }
    }
}

int main()
{
    setlocale(LC_ALL, "English");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    SetupConsoleFont();
    system("Color 0F");
    SetConsoleSize(2000, 60);

    HWND hwnd = GetConsoleWindow();

    if (hwnd != NULL)
    {
        RECT r;
        GetWindowRect(hwnd, &r);
        hdc = GetDC(hwnd);

        if (hdc != 0)
        {
            cout << "Tank Battle Demonstration" << endl;
            cout << "========================" << endl;

            // ??????? ???? ? ??????????? ? ?????? ?? ????? ?????????
            const int mineCount = 4;
            Mina* mines[mineCount];
            mines[0] = new Mina(800, 600);   // ?????-?????
            mines[1] = new Mina(1000, 600);  // ?????
            mines[2] = new Mina(1200, 600);  // ??????-??????
            mines[3] = new Mina(900, 750);   // ???? ?????

            const int repairCount = 3;
            Remont* repairs[repairCount];
            repairs[0] = new Remont(850, 450);   // ????-?????
            repairs[1] = new Remont(1050, 450);  // ????-??????
            repairs[2] = new Remont(1250, 450);  // ????-??????

            // ?????????? ???? ? ????????????
            for (int i = 0; i < mineCount; i++) mines[i]->Show();
            for (int i = 0; i < repairCount; i++) repairs[i]->Show();

            cout << "Battlefield prepared with mines and repair kits positioned in center!" << endl;
            Sleep(1000);

            // ??????? ??? ????? ?? ?????? ????????
            Tank mainTank(700, 800, 250, 120, 120, 70, 150, 40);
            mainTank.Show();

            HeavyTank heavy(2000, 1000);   // ??????
            heavy.Show();

            LightTank light(900, 200);   // ??????
            light.Show();

            DamageTank damage(1200, 200); // ??????
            damage.Show();

            cout << "All tanks deployed at equal distances!" << endl;
            cout << "Main Tank: " << mainTank.GetHealth() << " HP" << endl;
            cout << "Heavy Tank: " << heavy.GetHealth() << " HP" << endl;
            cout << "Light Tank: " << light.GetHealth() << " HP" << endl;
            cout << "Damage Tank: " << damage.GetHealth() << " HP" << endl;

            cout << "\nPress 1 to control Main Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(49)) break;
                Sleep(100);
            }

            if (!mainTank.IsDestroyed()) {
                cout << "Main Tank control active (arrows, ESC to stop)" << endl;
                int figX = mainTank.GetX(), figY = mainTank.GetY();
                while (1)
                {
                    if (KEY_DOWN(VK_ESCAPE)) break;
                    if (KEY_DOWN(VK_LEFT)) {
                        figX -= 30;
                        mainTank.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &mainTank);
                        CheckRepairCollisions(repairs, repairCount, &mainTank);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_RIGHT)) {
                        figX += 30;
                        mainTank.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &mainTank);
                        CheckRepairCollisions(repairs, repairCount, &mainTank);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_UP)) {
                        figY -= 30;
                        mainTank.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &mainTank);
                        CheckRepairCollisions(repairs, repairCount, &mainTank);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_DOWN)) {
                        figY += 30;
                        mainTank.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &mainTank);
                        CheckRepairCollisions(repairs, repairCount, &mainTank);
                        Sleep(200);
                    }
                }
            }

            cout << "\nPress 2 to control Heavy Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(50)) break;
                Sleep(100);
            }

            if (!heavy.IsDestroyed()) {
                cout << "Heavy Tank control active (arrows, ESC to stop)" << endl;
                int figX = heavy.GetX(), figY = heavy.GetY();
                while (1)
                {
                    if (KEY_DOWN(VK_ESCAPE)) break;
                    if (KEY_DOWN(VK_LEFT)) {
                        figX -= heavy.GetSpeed();
                        heavy.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &heavy);
                        CheckRepairCollisions(repairs, repairCount, &heavy);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_RIGHT)) {
                        figX += heavy.GetSpeed();
                        heavy.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &heavy);
                        CheckRepairCollisions(repairs, repairCount, &heavy);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_UP)) {
                        figY -= heavy.GetSpeed();
                        heavy.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &heavy);
                        CheckRepairCollisions(repairs, repairCount, &heavy);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_DOWN)) {
                        figY += heavy.GetSpeed();
                        heavy.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &heavy);
                        CheckRepairCollisions(repairs, repairCount, &heavy);
                        Sleep(200);
                    }
                }
            }

            cout << "\nPress 3 to control Light Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(51)) break;
                Sleep(100);
            }

            if (!light.IsDestroyed()) {
                cout << "Light Tank control active (arrows, ESC to stop)" << endl;
                int figX = light.GetX(), figY = light.GetY();
                while (1)
                {
                    if (KEY_DOWN(VK_ESCAPE)) break;
                    if (KEY_DOWN(VK_LEFT)) {
                        figX -= light.GetSpeed();
                        light.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &light);
                        CheckRepairCollisions(repairs, repairCount, &light);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_RIGHT)) {
                        figX += light.GetSpeed();
                        light.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &light);
                        CheckRepairCollisions(repairs, repairCount, &light);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_UP)) {
                        figY -= light.GetSpeed();
                        light.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &light);
                        CheckRepairCollisions(repairs, repairCount, &light);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_DOWN)) {
                        figY += light.GetSpeed();
                        light.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &light);
                        CheckRepairCollisions(repairs, repairCount, &light);
                        Sleep(200);
                    }
                }
            }

            cout << "\nPress 4 to control Damage Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(52)) break;
                Sleep(100);
            }

            if (!damage.IsDestroyed()) {
                cout << "Damage Tank control active (arrows, ESC to stop)" << endl;
                int figX = damage.GetX(), figY = damage.GetY();
                while (1)
                {
                    if (KEY_DOWN(VK_ESCAPE)) break;
                    if (KEY_DOWN(VK_LEFT)) {
                        figX -= damage.GetSpeed();
                        damage.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &damage);
                        CheckRepairCollisions(repairs, repairCount, &damage);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_RIGHT)) {
                        figX += damage.GetSpeed();
                        damage.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &damage);
                        CheckRepairCollisions(repairs, repairCount, &damage);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_UP)) {
                        figY -= damage.GetSpeed();
                        damage.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &damage);
                        CheckRepairCollisions(repairs, repairCount, &damage);
                        Sleep(200);
                    }
                    if (KEY_DOWN(VK_DOWN)) {
                        figY += damage.GetSpeed();
                        damage.MoveTo(figX, figY);
                        CheckMineCollisions(mines, mineCount, &damage);
                        CheckRepairCollisions(repairs, repairCount, &damage);
                        Sleep(200);
                    }
                }
            }

            cout << "\n=== ROCKET ATTACK DEMONSTRATION ===" << endl;

            cout << "\nPress 5 to launch rocket at Light Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(53)) break;
                Sleep(100);
            }

            if (!light.IsDestroyed()) {
                cout << "Rocket launched at Light Tank!" << endl;
                Rocket rocket1(100, 100, light.GetX() + light.GetBodyWidth()/2, light.GetY() + light.GetBodyHeight()/2, 50);
                rocket1.Show();

                for (int i = 0; i < 200 && rocket1.IsActive(); i++) {
                    rocket1.MoveToTarget();

                    if (rocket1.CheckCollision(&light)) {
                        cout << "ROCKET HIT! Light Tank!" << endl;
                        light.TakeDamage(rocket1.GetPower());
                        rocket1.Deactivate();

                        // ?????? ??????
                        HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
                        HBRUSH brush = CreateSolidBrush(RGB(255, 150, 0));
                        SelectObject(hdc, pen); SelectObject(hdc, brush);
                        Ellipse(hdc, rocket1.GetX() - 40, rocket1.GetY() - 40,
                                 rocket1.GetX() + 40, rocket1.GetY() + 40);
                        DeleteObject(brush); DeleteObject(pen);
                        Sleep(300);
                        break;
                    }
                    Sleep(50);
                }
            } else {
                cout << "Light Tank already destroyed!" << endl;
            }

            cout << "\nPress 6 to launch rocket at Heavy Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(54)) break;
                Sleep(100);
            }

            if (!heavy.IsDestroyed()) {
                cout << "Rocket launched at Heavy Tank!" << endl;
                Rocket rocket2(1800, 200, heavy.GetX() + heavy.GetBodyWidth()/2, heavy.GetY() + heavy.GetBodyHeight()/2, 80);
                rocket2.Show();

                for (int i = 0; i < 200 && rocket2.IsActive(); i++) {
                    rocket2.MoveToTarget();

                    if (rocket2.CheckCollision(&heavy)) {
                        cout << "ROCKET HIT! Heavy Tank!" << endl;
                        heavy.TakeDamage(rocket2.GetPower());
                        rocket2.Deactivate();

                        // ?????? ??????
                        HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
                        HBRUSH brush = CreateSolidBrush(RGB(255, 150, 0));
                        SelectObject(hdc, pen); SelectObject(hdc, brush);
                        Ellipse(hdc, rocket2.GetX() - 40, rocket2.GetY() - 40,
                                 rocket2.GetX() + 40, rocket2.GetY() + 40);
                        DeleteObject(brush); DeleteObject(pen);
                        Sleep(300);
                        break;
                    }
                    Sleep(50);
                }
            } else {
                cout << "Heavy Tank already destroyed!" << endl;
            }

            cout << "\nPress 7 to launch rocket at Damage Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(55)) break;
                Sleep(100);
            }

            if (!damage.IsDestroyed()) {
                cout << "Rocket launched at Damage Tank!" << endl;
                Rocket rocket3(100, 1000, damage.GetX() + damage.GetBodyWidth()/2, damage.GetY() + damage.GetBodyHeight()/2, 70);
                rocket3.Show();

                for (int i = 0; i < 200 && rocket3.IsActive(); i++) {
                    rocket3.MoveToTarget();

                    if (rocket3.CheckCollision(&damage)) {
                        cout << "ROCKET HIT! Damage Tank!" << endl;
                        damage.TakeDamage(rocket3.GetPower());
                        rocket3.Deactivate();

                        // ?????? ??????
                        HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
                        HBRUSH brush = CreateSolidBrush(RGB(255, 150, 0));
                        SelectObject(hdc, pen); SelectObject(hdc, brush);
                        Ellipse(hdc, rocket3.GetX() - 40, rocket3.GetY() - 40,
                                 rocket3.GetX() + 40, rocket3.GetY() + 40);
                        DeleteObject(brush); DeleteObject(pen);
                        Sleep(300);
                        break;
                    }
                    Sleep(50);
                }
            } else {
                cout << "Damage Tank already destroyed!" << endl;
            }

            cout << "\nPress 8 to launch rocket at Main Tank" << endl;
            while (1)
            {
                if (KEY_DOWN(56)) break;
                Sleep(100);
            }

            if (!mainTank.IsDestroyed()) {
                cout << "Rocket launched at Main Tank!" << endl;
                Rocket rocket4(1500, 100, mainTank.GetX() + mainTank.GetBodyWidth()/2, mainTank.GetY() + mainTank.GetBodyHeight()/2, 60);
                rocket4.Show();

                for (int i = 0; i < 200 && rocket4.IsActive(); i++) {
                    rocket4.MoveToTarget();

                    if (rocket4.CheckCollision(&mainTank)) {
                        cout << "ROCKET HIT! Main Tank!" << endl;
                        mainTank.TakeDamage(rocket4.GetPower());
                        rocket4.Deactivate();

                        // ?????? ??????
                        HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
                        HBRUSH brush = CreateSolidBrush(RGB(255, 150, 0));
                        SelectObject(hdc, pen); SelectObject(hdc, brush);
                        Ellipse(hdc, rocket4.GetX() - 40, rocket4.GetY() - 40,
                                 rocket4.GetX() + 40, rocket4.GetY() + 40);
                        DeleteObject(brush); DeleteObject(pen);
                        Sleep(300);
                        break;
                    }
                    Sleep(50);
                }
            } else {
                cout << "Main Tank already destroyed!" << endl;
            }

            cout << "\n=== FINAL BATTLE REPORT ===" << endl;
            cout << "Main Tank: " << (mainTank.IsDestroyed() ? "DESTROYED" : "ALIVE") << " (" << mainTank.GetHealth() << " HP)" << endl;
            cout << "Heavy Tank: " << (heavy.IsDestroyed() ? "DESTROYED" : "ALIVE") << " (" << heavy.GetHealth() << " HP)" << endl;
            cout << "Light Tank: " << (light.IsDestroyed() ? "DESTROYED" : "ALIVE") << " (" << light.GetHealth() << " HP)" << endl;
            cout << "Damage Tank: " << (damage.IsDestroyed() ? "DESTROYED" : "ALIVE") << " (" << damage.GetHealth() << " HP)" << endl;

            cout << "\nPress ENTER to exit..." << endl;
            while (1)
            {
                if (KEY_DOWN(VK_RETURN)) break;
                Sleep(100);
            }

            // Очистка памяти
            for (int i = 0; i < mineCount; i++) delete mines[i];
            for (int i = 0; i < repairCount; i++) delete repairs[i];

            ReleaseDC(hwnd, hdc);
        }
    }

    system("pause");
    return 0;
}
