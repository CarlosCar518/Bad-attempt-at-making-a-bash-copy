#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/commands.h"

typedef struct 
{
    int x,y;
}Point;

typedef struct 
{
    Point *body;
    int lenght;
    int dx,dy;
}Snake;

static HANDLE wHndIn;
static HANDLE wHndOut;

void _Smain()
{
    Snake sn = {NULL,0, 20, 20};
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    printf("\x1b[2J\x1b[?25l");
    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    wHndOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int running = 1;
    while (running)
    {
        if (! ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
        {
            return;
        }

        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT:
                char key = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;

                if (key == 'w')
                {
                    sn.dy--;
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    
                }
                else if (key == 'a')
                {
                    sn.dx--;
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    
                }
                else if( key == 's')
                {
                    sn.dy++;
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                }
                else if ( key == 'd')
                {
                    sn.dx++;
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                }
                
                if ( key == 'p')
                    running = 0;

            default:
                break;
            }
        }
    }     
}