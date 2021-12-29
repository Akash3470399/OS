#include <stdlib.h>
#include <string.h>

struct SymTab
{
    int address, defined, used;
    char *symbol;
    struct SymTab *next;
}*s_start=NULL, *s_end=NULL;

int LC = 0;

struct SymTab *newSymbolNode(char *symbol,int address, int defined, int used)
{
    struct SymTab *st = (struct SymTab *)malloc(sizeof(struct SymTab));
    st->symbol = (char *)malloc(sizeof(char)*10);
    strcpy(st->symbol, symbol);
    st->address = address;
    st->defined = defined;
    st->used = used;
    st->next = NULL;

    return st;
}