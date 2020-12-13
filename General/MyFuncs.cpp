
#include "MyFuncs.h"
#include <iostream>
#include "IT.h"
using namespace std;

char* substr(char* str, int start, int length)
{
    char* s;
    // Определить длину исходной строки
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++)
        len++;
    // Определить позицию последнего символа подстроки
    if (length > 0)
    {
        if (start + length < len)
            len = start + length;
    }
    else // length < 0
        len = len + length;
    int newlen = len - start + 1; // длина подстроки
    s = new char[newlen];
    // Копирование символов подстроки
    int j = 0;
    for (int i = start; i < len; i++)
    {
        s[j] = str[i]; j++;
    }
    s[j] = '\0';
    return(s);
}

char* makestr(char* str)
{
    
    char* t = new char[TI_STR_MAXSIZE];
    strcpy(t, "\"");
    strcat(t, substr(str, 1, strlen(str) - 2));
    strcat(t, "\"");

    return t;
}