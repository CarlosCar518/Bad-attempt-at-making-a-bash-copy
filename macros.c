#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>

#include "commands.h"

void macro_autocomplete(char* prefix, flow_struct* st)
{

    char* path = return_fixed_current_path();
    size_t prefix_len = strlen(prefix);

    WIN32_FIND_DATAA pathData;
    HANDLE hFind;

    if ((hFind = FindFirstFile(path, &pathData)) == INVALID_HANDLE_VALUE )
    {
        printf("Erorr\n");
    }

    while (FindNextFile(hFind, &pathData))
    {
        if(strnicmp(pathData.cFileName, prefix, prefix_len) == 0)
        {
            int i;
            for (i = 0; prefix[i] == pathData.cFileName[i]; i++)
                ;
            while(pathData.cFileName[i] != '\0')
            {
                printf("%c",pathData.cFileName[i++]);
            }
            break;
        }
    }

    size_t difference = strlen(pathData.cFileName) - strlen(prefix);

    char* temp = realloc(st->buff, (st->pos) + difference);
    if (!temp)
    {
        free(temp);
    }

    char* last_token = (strrchr(st->buff, ' '));

    if (last_token)
    {
        prefix = last_token + 1;
    }
    else
    {
        prefix = st->buff;
    }

    strcpy(prefix, pathData.cFileName);

    FindClose(hFind);
}

char* return_fixed_current_path()
{
    char* path = _getcwd(NULL, 0);
    int pathLen = strlen(path);

    path = realloc(path, pathLen + 3);

    path[pathLen++] = '\\';
    path[pathLen++] = '*';
    path[pathLen] = '\0';

    return path;
}
