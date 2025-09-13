#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>

#include "../include/dirent.h"
#include "../include/commands.h"

void command_print_serie()
{
    for (int i = 1; i <= 100; i++)
        printf("%d\n", i);
}

void command_print_message()
{
    printf("Hoy comi perro\n");
}

void command_print_arg()
{
    char *arg = strtok(NULL, "");
    if (arg == NULL)
        printf("You need to need to say something");

    printf("The user said %s\n", arg);
}

void command_remove()
{
    char *arg = strtok(NULL, " ");

    if (!arg)
    {
        printf("Error: no file specified\n");
        return;
    }

    int result = remove(arg);

    if (result != SUCCESS)
    {
        printf("File not found");
    }
}

void command_print_currentWorkingDir()
{
    char *pwd = _getcwd(NULL, 0);

    if (pwd != NULL)
    {
        printf("%s\n", pwd);
        free(pwd);
        return;
    }

    printf("Error");
}

void command_change_dir()
{
    char *token = strtok(NULL, " ");

    if (_chdir(token))
    {
        printf("Path not found\n");
    }
}

void command_make_dir()
{
    char *token = strtok(NULL, " ");

    if (_mkdir(token))
    {
        printf("Error\n");
    }
}

void command_remove_dir()
{
    char *token = strtok(NULL, " ");

    if (rmdir(token))
    {
        printf("Error\n");
    }
}

void command_help()
{
    printf("        Commands list:\n");
    printf("lt -- prints the numbers from 1 to 100 (dont ask me why)\npm -- prints a nice message\nex -- exits the shell\npr -- prints the next argument\n");

    printf("rm -- deletes the file with the name of the argument\npwd -- prints the current working directory\ncd -- changes the current working directory to the argument\n");
    printf("mkdir -- creates a new directory\nrmdir -- deletes the directory\n");
}

void command_listFiles()
{
    DIR *dir = opendir(".");
    if (!dir)
    {
        printf("Error");
    }

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL)
    {
        if ((strcmp(ent->d_name, ".")) == 0 || (strcmp(ent->d_name, "..")) == 0)
        {
            continue;
        }

        printf("%s  ", ent->d_name);
    }
    printf("\n");
    closedir(dir);
}

void command_print_date()
{
    time_t currentTime;
    time(&currentTime);

    printf("%s\n", ctime(&currentTime));
}

void command_clear()
{
    printf("\x1b[2J\x1b[0;0f");
}

void command_readFile()
{
    char *token = strtok(NULL, " ");
    FILE *f = fopen(token, "r");
    if (!f)
    {
        perror("fopen");
        return;
    }

    int c;
    while ((c = fgetc(f)) != EOF)
        putchar(c);
    fclose(f);
}

void command_execute()
{
    char *token = strtok(NULL, " ");
    int status = system(token);
    if (status)
    {
        printf("Wrong argument\n");
    }
}
