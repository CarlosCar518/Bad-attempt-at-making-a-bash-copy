#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <curl/curl.h>
#include <Windows.h>
#include <time.h>

#include "commands.h"

void command_print_serie()
{
    for(int i = 1; i <= 100; i++)
        printf("%d\n", i);
}

void command_print_message()
{
    printf("Hoy comi perro\n");
}

void command_print_arg()
{
    char* arg = strtok(NULL, " ");

    printf("The user said %s\n", arg);
}

void command_remove()
{
    char* arg = strtok(NULL, " ");

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

void command_wd()
{
    char* pwd = _getcwd(NULL, 0);

    if (pwd != NULL)
    {
        printf("%s\n",pwd);
        free(pwd);
        return;
    }

    printf("Error");
}

void command_change_dir()
{
    char* token = strtok(NULL, " ");

    if(_chdir(token))
    {
        printf("Path not found\n"); 
    }
}

void command_make_dir()
{
    char* token = strtok(NULL, " ");

    if (_mkdir(token))
    {
        printf("Error\n");
    }
}

void command_remove_dir()
{
    char* token = strtok(NULL, " ");

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
    //I get the cwd and adjust it adding \* in order to get all the files and meet the Win API specifications
    char* path = _getcwd(NULL, 0);
    int pathLen = strlen(path);

    path = realloc(path, pathLen + 3);

    path[pathLen++] = '\\';
    path[pathLen++] = '*';
    path[pathLen] = '\0';

    WIN32_FIND_DATAA pathData;
    HANDLE hFind;

    if ((hFind = FindFirstFile(path, &pathData)) == INVALID_HANDLE_VALUE )
    {
        printf("Erorr\n");
    }

    while (FindNextFile(hFind, &pathData))
    {
        if (strcmp(pathData.cFileName, ".") == 0 || strcmp(pathData.cFileName, "..") == 0 )
            continue;
        printf("%s\n", pathData.cFileName);
    }

    FindClose(hFind);
    free(path);
}

void command_print_date()
{
    time_t currentTime;
    time(&currentTime);

    printf("%s\n",ctime(&currentTime));
}

void command_joke()
{
    curl_global_init(CURL_GLOBAL_WIN32);
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, "https://icanhazdadjoke.com/");

    struct curl_slist *headers = NULL;
   
    headers = curl_slist_append(headers, "Accept: text/plain");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_perform(handle);
    printf("\n");
    curl_global_cleanup();
}

void command_clear()
{
    system("cls");
}