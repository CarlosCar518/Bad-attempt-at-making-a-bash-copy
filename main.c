#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "commands.h"

#define global_internal static

typedef void (*cmd_fn)(void);

typedef struct {
    char* name;
    cmd_fn fn;
}commands;

void read(flow_struct * st);
void take_com(flow_struct st);
int find_command(char* token);
void add_char(flow_struct* st, char c);

int main(void)
{
    flow_struct line;
    read(&line);    
    return 0;
}

void read(flow_struct* st)
{
    char* possible_aut = NULL;
    st->buff = NULL;
    st->pos = 0;
    size_t cmd_pos = 0;
    char c;
    printf("?");

    while (1)
    {
        if (_kbhit())
        {
            c = _getch();
            if (cmd_pos == 0 && (c == '\b' || c == '\r'))
            {
                continue;
            }

            if (c == '\t')
            {
                possible_aut = (strrchr(st->buff, ' '));
                possible_aut = (possible_aut == NULL) ? st->buff : possible_aut + 1;

                macro_autocomplete(possible_aut, st);
                continue;
            }

            if (c == '\r')
            {
                st->pos = 0;
                printf("\n");
                if (strcmp(st->buff,"ex") == 0)
                {
                    return;
                }
                take_com(*st);
                printf("?");
                continue;
            }
            else if (c == '\b')
            {
                if (cmd_pos > 0)
                {
                    cmd_pos--;
                    st->pos--;
                    (st->buff)[st->pos] = '\0';
                    printf("\b \b");
                }
                continue;
            }
            add_char(st, c);
            printf("%c",c);
            cmd_pos++;
        }
    }
}

void add_char(flow_struct* st, char c)
{
    char* tmp = realloc(st->buff, st->pos + 2);

    if(!tmp)
    {
        free(st->buff);
        free(tmp);
        st->buff = NULL;
    }
    st->buff = tmp;
    (st->buff)[st->pos++] = c;
    (st->buff)[st->pos] = '\0';
}

void take_com(flow_struct st)
{
    char* token = strtok(st.buff, " ");
    int flag;
    do
    {
        flag = find_command(token);
    } while ((token = strtok(NULL, " ")) && flag);
}

int find_command(char* token)
{
    commands command_list[] = { {"lt" , command_print_serie}, {"pm", command_print_message},
{"pr", command_print_arg}, {"rm", command_remove},{"pwd",command_wd},{"cd", command_change_dir},
{"mkdir", command_make_dir},{"rmdir", command_remove_dir},{"help", command_help},{"dir",command_listFiles},{"time", command_print_date},
{"jk", command_joke},{"clear", command_clear},{NULL, NULL}};

    for (int i = 0; command_list[i].name != NULL ;i++)
    {
        if (strcmp(token, command_list[i].name) == SUCCESS)
        {
            command_list[i].fn();
            return 1;
        }
    }

    printf("Command %s is not valid\n", token);
    return 0;
}