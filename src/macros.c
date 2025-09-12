#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "../include/commands.h"
#include "../include/dirent.h"

void dir_complete(flow_struct *st)
{
    if (st->pos == 0)
        return;

    char *prefix = NULL;
    /*Encuentro ultimo token. Si no hay token  igualo al buffer*/
    prefix = (strrchr(st->buff, ' '));
    prefix = (prefix == NULL) ? st->buff : prefix + 1;

    size_t prefix_len = strlen(prefix);

    DIR *dir = opendir(".");
    if (!dir)
        printf("Error");

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        /*Itero sobre la carpeta hasta encontrar coincidencia*/
        if (strnicmp(ent->d_name, prefix, prefix_len) == 0)
        {
            int i;
            /*Puntero al primer char donde NO coinciden*/
            for (i = 0; prefix[i] == ent->d_name[i]; i++)
                ;
            printf("%s", &ent->d_name[i]);
            break;
        }
    }

    if (!ent)
    {
        closedir(dir);
        return;
    }
    /*Añado al buffer el resto de chars*/

    size_t diference = (strlen(ent->d_name) - strlen(prefix));
    char *temp = realloc(st->buff, (st->pos) + diference + 1);
    if (!temp)
    {
        free(st->buff);
        st->buff = NULL;
        return;
    }
    st->buff = temp;
    char *last_token = strrchr(st->buff, ' ');

    if (last_token)
        prefix = last_token + 1;
    else
        prefix = st->buff;

    strcpy(prefix, ent->d_name);
    st->pos = st->cursor = strlen(st->buff);

    closedir(dir);
}

char *return_fixed_current_path()
{
    char *path = _getcwd(NULL, 0);
    int pathLen = strlen(path);

    path = realloc(path, pathLen + 3);

    path[pathLen++] = '\\';
    path[pathLen++] = '*';
    path[pathLen] = '\0';

    return path;
}