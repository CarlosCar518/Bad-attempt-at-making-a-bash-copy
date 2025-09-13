#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/commands.h"

typedef struct
{
    int x, y;
} Point;

typedef struct
{
    Point body[30];
    int lenght;
    int speed;
    int dx, dy;
} Snake;

static HANDLE wHndIn;
static HANDLE wHndOut;
void lost();

int running = 1;

void _Smain()
{
    running = 1;
    printf("\x1b[?1049h\x1b[?25l");

    Snake sn = {0};
    sn.body[0].x = 20;
    sn.body[0].y = 20;
    sn.speed = 1;
    sn.dx = 1;

    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    SMALL_RECT consoleCords;

    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    wHndOut = GetStdHandle(STD_OUTPUT_HANDLE);

    srand(time(NULL));

    GetConsoleScreenBufferInfo(wHndOut, &consoleInfo);
    consoleCords = consoleInfo.srWindow;
    int rows = consoleCords.Bottom - consoleCords.Top + 1;
    int cols = consoleCords.Right - consoleCords.Left + 1;

    int ran0 = (rand() % (cols - 2)) + 1;
    int ran1 = (rand() % (rows - 2)) + 1;
    Point r = {ran0, ran1};

    int points = 0;

    printf("\x1b[2J\x1b[0;0f");
    printf("\x1b[%i;%i\x1b[1m\x1b[34mSCORE: \x1b[0m\x1b[s", 8, 8);

    while (running)
    {
        if (PeekConsoleInput(wHndIn, irInBuf, 128, &cNumRead) && cNumRead > 0)
        {
            if (!ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
            {
                return;
            }

            for (i = 0; i < cNumRead; i++)
            {
                switch (irInBuf[i].EventType)
                {
                case KEY_EVENT:
                    char key = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
                    if (key == 'w' && sn.dy != 1)
                    {
                        sn.dy = -1;
                        sn.dx = 0;
                    }
                    else if (key == 'a' && sn.dx != 1)
                    {
                        sn.dx = -1;
                        sn.dy = 0;
                    }
                    else if (key == 's' && sn.dy != -1)
                    {
                        sn.dy = 1;
                        sn.dx = 0;
                    }
                    else if (key == 'd' && sn.dx != -1)
                    {
                        sn.dx = 1;
                        sn.dy = 0;
                    }
                    else if (key == 'p')
                    {
                        running = 0;
                        lost();
                    }
                    break;

                default:
                    break;
                }
            }
        }
        int touch = 0;

        Point *head = &sn.body[0];
        Point erase = sn.body[sn.lenght];

        for (int i = sn.lenght; i > 0; i--)
            sn.body[i] = sn.body[i - 1];

        head->x += sn.speed * sn.dx;
        head->y += sn.speed * sn.dy;

        if (!sn.dy)
            Sleep(60);
        else
            Sleep(120);

        if (head->x > consoleCords.Right || head->x < consoleCords.Left ||
            head->y > consoleCords.Bottom || head->y < consoleCords.Top)
        {
            lost();
            return;
        }

        for (int i = 1; i < sn.lenght; i++)
            if (head->x == sn.body[i].x && head->y == sn.body[i].y)
                lost();

        if (head->y == r.y && head->x == r.x)
        {
            r.x = (rand() % (cols - 2)) + 2;
            r.y = (rand() % (rows - 2)) + 2;
            touch = 1;
            sn.lenght++;
            points += 1;
        }

        printf("\x1b[u\x1b[31m%i\x1b[0m", points);
        printf("\x1b[%d;%dH\x1b[32m#\x1b[0m", r.y, r.x);
        if (!touch)
        {
            printf("\x1b[%i;%iH ", erase.y, erase.x);
        }
        else
        {
            sn.body[sn.lenght] = erase;
        }

        printf("\x1b[%i;%iH*", head->y, head->x);
    }
    printf("\x1b[?1049l");
}

void lost()
{
    printf("\x1b[2J\x1b[0;0f\x1b[31m");
    printf("____    ____  ______    __    __      __       ______       _______.___________.\n");
    printf("\\   \\  /   //  __  \\  |  |  |  |    |  |      /  __  \\     /       |           |   \n");
    printf("\\   \\/   / |  |  |  | |  |  |  |    |  |     |  |  |  |    |   (----`---|  |----`   \n");
    printf(" \\_    _/  |  |  |  | |  |  |  |    |  |     |  |  |  |     \\   \\       |  |        \n");
    printf("   |  |    |  `--'  | |  `--'  |    |  `----.|  `--'  | .----)   |      |  |        \n");
    printf("   |__|     \\______/   \\______/     |_______| \\______/  |_______/       |__|        \x1b[0m\n");
    Sleep(2000);
    running = 0;
}