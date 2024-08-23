#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef GLOBEL_ARRS_H
#define GLOBEL_ARRS_H
#endif

#ifndef UTILITYFUNC_H
// function to tokenize a source program line
// returns tokens count in a line & tokens in tokens array
int tokenizer(char *line, char *tokens[5])
{
    char str[80], *token;
    int tokenCount = 0;
    strcpy(str, line);
    token = strtok(str, " ");

    while (token != NULL)
    {
        tokens[tokenCount] = (char *)malloc(sizeof(char) * 80);
        strcpy(tokens[tokenCount], token);
        token = strtok(NULL, " ");
        tokenCount++;
    }
    return tokenCount;
}

// function to remove all ouccurance of char in a string
// parameters :
//     line : String
//     ch   : single char string to remove
char *removeChar(char *line, char *ch)
{
    char *token, str[80];
    strcpy(str, line);
    token = strtok(str, ch);
    strcpy(line, "");

    while (token != NULL)
    {
        line = strcat(line, token);
        token = strtok(NULL, ch);
    }
    return line;
}

// function to check if a string is present in array of strings
// parameters:
//     str: string to search for
//     arr: array of strings in which to search for str string
// returns 1 if found else 0
int isStrInArr(char *str, char *arr[])
{
    for (int i = 0; arr[i] != NULL; i++)
    {
        if (strcmp(str, arr[i]) == 0)
            return i;
    }

    return -1;
}

// function to free a block of memory allocated for a string array
// ****NOTE: Array's last entry should be NULL, (to indicate end of array)
// parameter:
//    *ptr : pointer to array of string which is to be free
void clearStrArr(char *ptr[])
{
    int i = 0;
    while (ptr[i] != NULL)
    {
        free(ptr[i]);
        i++;
    }
}

// function to create a  string is a number
bool isNumber(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

// function to check if address is valid or not
bool isValidAddress(char *add)
{
    int i = 0;
    while (add[i] != '\0')
    {
        if (!isdigit(add[i]))
            return false;
        i++;
    }

    if (strlen(add) > 3)
        return false;

    return true;
}
// function to check if a str is valid label
bool isValidLabel(char *str)
{
    char **arrs[5] = {mnem, registers, ad, ds, cc};
    // checking for label validity
    for (int i = 0; i < 5; i++)
    {
        if (isStrInArr(str, arrs[i]) != -1)
            return false;
    }

    return true;
}

// function to check if a str is valid literal
bool isValidLiteral(char *str)
{
    int i = 0;
    if (str[0] == '=' && str[1] == '\'')
    {
        for (i = 2; str[i] != '\0' && str[i] != '\''; i++)
        {
            if (!isdigit(str[i]))
                return false;
        }
    }
    else
        return false;

    return true;
}

#endif
