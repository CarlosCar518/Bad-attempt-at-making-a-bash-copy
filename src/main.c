#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <sys/types.h>

#include "../include/commands.h"

#define KEY_TAB '\t'
#define KEY_ENTER '\r'
#define KEY_BACK_SPACE '\b'
#define EXIT '\x1A'
#define ESC 0x1B
#define global_internal static
#define internal static

typedef void (*cmd_fn)(void);

typedef struct
{
    char *name;
    cmd_fn fn;
} commands;

global_internal HANDLE wHndIn;
global_internal HANDLE wHndOut;
global_internal DWORD fdwSaveOldMode;
global_internal int running;
global_internal char *history;

internal void ShellEx(flow_struct *st);
internal void ShellTakeCommand(flow_struct st);
internal void buffer_add_char(flow_struct *st, char c);
internal void ShellKeyMain(KEY_EVENT_RECORD ker, flow_struct *line);
internal void ShellKeyEventArrow(WORD key, flow_struct *line);
internal void ShellKeyEvent(char key, flow_struct *line);
internal void error_exit(char *error);
internal void ShellKeyEnter(flow_struct *line);
internal void buffer_delete_char(flow_struct *line);
internal void print_cwd();
internal void init_console();

/*
 *
 * `grep <palabra> <archivo>` → buscar palabras en un archivo.
 * `history` → mostrar los últimos comandos ejecutados.
 */

global_internal commands buff_commands[] = {
    {"lt", command_print_serie},
    {"pm", command_print_message},
    {"echo", command_print_arg},
    {"rm", command_remove},
    {"pwd", command_print_currentWorkingDir},
    {"cd", command_change_dir},
    {"mkdir", command_make_dir},
    {"rmdir", command_remove_dir},
    {"help", command_help},
    {"dir", command_listFiles},
    {"time", command_print_date},
    {"clear", command_clear},
    {"snake", _Smain},
    {"cat", command_readFile},
    {"exec", command_execute},
    {NULL, NULL}};

int main(void)
{
    printf("\x1b[2J\x1b[0;0f\x1b[?25h");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    flow_struct line = {NULL, 0, 0};
    ShellEx(&line);
    return 0;
}

internal void ShellEx(flow_struct *st)
{
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    init_console();

    running = 1;

    print_cwd();
    printf("\x1b[0m\x1b[33m∑ \x1b[0m");
    while (running)
    {
        if (!ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
        {
            error_exit("ReadConsoleInput");
        }

        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT:
                ShellKeyMain(irInBuf[i].Event.KeyEvent, st);
                break;

            default:
                break;
            }
        }
    }
}

internal void ShellKeyMain(KEY_EVENT_RECORD ker, flow_struct *line)
{
    if (ker.bKeyDown)
    {
        char key_pressed = ker.uChar.AsciiChar;

        if (ker.wVirtualKeyCode == VK_RIGHT || ker.wVirtualKeyCode == VK_LEFT ||
            ker.wVirtualKeyCode == VK_UP || ker.wVirtualKeyCode == VK_DOWN)
        {
            ShellKeyEventArrow(ker.wVirtualKeyCode, line);
            return;
        }

        if (ker.uChar.AsciiChar != 0)
            ShellKeyEvent(key_pressed, line);
    }
}

internal void ShellKeyEvent(char key, flow_struct *line)
{
    switch (key)
    {
    case KEY_ENTER:
    {
        ShellKeyEnter(line);
    }
    break;

    case KEY_BACK_SPACE:
    {
        buffer_delete_char(line);
    }
    break;

    case KEY_TAB:
    {
        dir_complete(line);
    }
    break;

    case EXIT:
    {
        running = 0;
        printf("\x1b[2J\x1b[0;0f");
    }
    break;

    default:
    {
        buffer_add_char(line, key);
        (line->cursor)++;

        if (line->cursor >= line->pos)
            printf("%c", key);
        else
            printf("\x1b[1@%c", key);
    }
    break;
    }
}

internal void ShellKeyEventArrow(WORD key, flow_struct *line)
{
    switch (key)
    {
    case VK_LEFT:
        if (line->cursor <= 0)
            break;
        printf("\x1b[1D");
        (line->cursor)--;
        break;
    case VK_RIGHT:
        if (line->cursor >= line->pos)
            break;
        printf("\x1b[1C");
        (line->cursor)++;
        break;
    default:
        break;
    }
}

internal void ShellKeyEnter(flow_struct *line)
{
    if (line->pos == 0)
        return;

    printf("\n");
    ShellTakeCommand(*line);
    printf("\n");
    print_cwd();
    printf("\x1b[0m\x1b[33m∑ \x1b[0m\x1b[s");

    line->pos = 0;
    line->cursor = 0;
    line->buff[0] = '\0';
}

internal void buffer_add_char(flow_struct *st, char c)
{
    if (st->cursor < st->pos)
    {
        size_t newSize = st->pos + 2;
        char *tmp = realloc(st->buff, newSize);
        if (!tmp)
        {
            free(st->buff);
            st->buff = NULL;
            return;
        }
        st->buff = tmp;

        size_t lower_limit = st->cursor;
        size_t upper_limit = st->pos + 1;

        for (; upper_limit >= lower_limit; upper_limit--)
            (st->buff)[upper_limit] = (st->buff)[upper_limit - 1]; // This loops moves all the chars 1 pos to the right

        (st->buff)[st->cursor] = c;
        (st->pos)++;
        return;
    }

    char *tmp = realloc(st->buff, st->pos + 2);

    if (!tmp)
    {
        free(st->buff);
        st->buff = NULL;
        return;
    }
    st->buff = tmp;
    (st->buff)[st->pos++] = c;
    (st->buff)[st->pos] = '\0';
}

internal void buffer_delete_char(flow_struct *line)
{
    if (line->cursor == 0)
    {
        return;
    }

    if (line->cursor < line->pos)
    {
        (line->cursor)--;
        size_t upper_limit = line->pos;
        size_t lower_limit = line->cursor;

        for (; lower_limit < upper_limit; lower_limit++)
        {
            (line->buff)[lower_limit] = (line->buff)[lower_limit + 1];
        }

        printf("\b\x1b[1P");
        (line->pos)--;
        line->buff = (char *)realloc(line->buff, line->pos);
        return;
    }

    printf("\b\x1b[1P");
    (line->pos)--;
    (line->cursor)--;
    (line->buff)[line->pos] = '\0';
    line->buff = (char *)realloc(line->buff, line->pos);
}

internal void ShellTakeCommand(flow_struct st)
{
    int i;
    char *isPath = (strrchr(st.buff, ' '));
    char *token = strtok(st.buff, " ");
    do
    {
        for (i = 0; buff_commands[i].name != NULL && (strcmp(token, buff_commands[i].name)) != 0; i++)
            ;
        if (buff_commands[i].name == NULL)
        {
            if (!isPath)
            {
                int result = run_PATH(st.buff);
                if (!result)
                    return;
            }
            printf("Command %s is not valid\n", token);
            return;
        }
        else
            buff_commands[i].fn();

    } while ((token = strtok(NULL, " ")));
}

/*-------------------------------WINDOWS STUFF-----------------------------------------*/
internal void error_exit(char *error)
{
    fprintf(stderr, "%s\n", error);

    SetConsoleMode(wHndIn, fdwSaveOldMode);

    ExitProcess(0);
}

internal void init_console()
{
    DWORD fwModeIn;
    DWORD fwModeOut;

    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    if (wHndIn == INVALID_HANDLE_VALUE)
    {
        error_exit("GetStdHandle");
    }

    wHndOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (wHndIn == INVALID_HANDLE_VALUE)
    {
        error_exit("GetStdHandle");
    }

    if (!GetConsoleMode(wHndIn, &fwModeIn))
    {
        error_exit("GetConsoleMode");
    }

    fwModeIn = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (!SetConsoleMode(wHndIn, fwModeIn))
    {
        error_exit("SetConsoleMode");
    }

    if (!GetConsoleMode(wHndOut, &fwModeOut))
    {
        error_exit("GetConsoleMode");
    }

    fwModeOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(wHndOut, fwModeOut))
    {
        error_exit("SetConsoleMode");
    }
}

internal void print_cwd()
{
    char *cwd;
    cwd = getcwd(NULL, 0);

    printf("\x1b[0m\x1b[32m<%s>\x1b[0m\n", cwd);
}