#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool string_isnumber(char *string)
{
    if (string == NULL) return false;
    for (char *i = string; *i != 0; ++i)
        if (!isdigit(*i))
            return false;
    return true;
}

char *stralloc(const char *string)
{
    char *out = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(out, string);
    return out;
}
