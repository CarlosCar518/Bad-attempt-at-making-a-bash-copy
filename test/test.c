#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *path = getenv("PATHEXT");
	if (path)
	{
		printf("PATH = %s\n", path);
	}
	return 0;

}
