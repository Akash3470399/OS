#include <stdlib.h>
#include <string.h>
#ifndef DATASTRUCTURE_H

struct SymTab
{
    int no, address, defined, used, value;
    char *symbol;
    struct SymTab *next;
} *s_start = NULL, *s_end = NULL, *temp_ptr = NULL;

struct LitTab
{
    int no, address;
    char *literal;
    struct LitTab *next;
} *l_start = NULL, *l_end = NULL, *lit_ptr = NULL;

struct PoolTab
{
    struct LitTab *literal;
    struct PoolTab *next;
} *p_start = NULL, *p_end = NULL;

int LC = 0, symNo = 1, litNo = 1, isFirstLitInPass = 0;

// **************  Symbol table  **************

//
struct SymTab *newSymbolNode(char *symbol, int address, int defined, int used)
{
    struct SymTab *st = (struct SymTab *)malloc(sizeof(struct SymTab));
    st->no = symNo++;
    st->value = -1;
    st->symbol = (char *)malloc(sizeof(char) * 10);
    strcpy(st->symbol, symbol);
    st->address = address;
    st->defined = defined;
    st->used = used;
    st->next = NULL;

    return st;
}

struct SymTab *getSymbolAdd(char *str)
{
    struct SymTab *i;
    for (i = s_start; i != NULL; i = i->next)
    {
        if (strcmp(i->symbol, str) == 0)
            return i;
    }
    return NULL;
}

void addToSymTab(char *sym, int address, int defined, int used)
{
    if (s_end == NULL)
    {
        s_start = newSymbolNode(sym, address, defined, used);
        s_end = s_start;
    }
    else
    {
        s_end->next = newSymbolNode(sym, address, defined, used);
        s_end = s_end->next;
    }
}

void printSymTab()
{
    struct SymTab *i;
    printf("\n\t\tSymbol Table\nNo\tsym\tadd\tdefined\tused\tvalue\n");
    for (i = s_start; i != NULL; i = i->next)
        printf("%d\t%s\t%d\t%d\t%d\t%d\n", i->no, i->symbol, i->address, i->defined, i->used, i->value);
}

int printSymTabError()
{
    struct SymTab *i;
    int err = 0;
    for (i = s_start; i != NULL; i = i->next)
    {
        if (i->defined > 1)
        {
            printf("ERROR : Re-declaration of symbol %s\n", i->symbol);
            err++;
        }
        if (i->defined == 0 && i->used == 1)
        {
            printf("ERROR : Symbol %s is used but not defined. \n", i->symbol);
            err++;
        }
        if (i->defined == 1 && i->used == 0)
            printf("WARNING : %s defined but not used.\n", i->symbol);
    }
    return err ;
}

// Literal table
void addToPoolTable(struct LitTab *lit)
{
    struct PoolTab *p = (struct PoolTab *)malloc(sizeof(struct PoolTab));
    p->literal = lit;
    p->next = NULL;

    if (p_start == NULL)
    {
        p_start = p;
        p_end = p_start;
    }
    else
    {
        p_end->next = p;
        p_end = p_end->next;
    }
}

struct LitTab *newLiteralNode(char *literal)
{
    struct LitTab *lt = (struct LitTab *)malloc(sizeof(struct LitTab));
    lt->literal = (char *)malloc(sizeof(char) * 10);
    strcpy(lt->literal, literal);
    lt->no = litNo++;
    lt->address = -1;
    return lt;
}

void addToLitTab(char *literal)
{
    if (l_end == NULL)
    {
        l_start = newLiteralNode(literal);
        l_end = l_start;
        isFirstLitInPass = 1;
    }
    else
    {
        l_end->next = newLiteralNode(literal);
        l_end = l_end->next;
    }

    if (isFirstLitInPass == 1)
    {
        addToPoolTable(l_end);
        isFirstLitInPass = 0;
    }
}

void printLitTab()
{
    struct LitTab *i;
    printf("\n\t\tLiteral Table\nNo\tLit\tAddress\n");
    for (i = l_start; i != NULL; i = i->next)
        printf("%d\t%s\t%d\t\n", i->no, i->literal, i->address);
}

struct LitTab *getLitAdd(char *lit)
{
    struct LitTab *i;
    for (i = l_start; i != NULL; i = i->next)
    {
        if (strcmp(i->literal, lit) == 0)
            return i;
    }
    return NULL;
}
// Pool table
int isPresentInPool(char *lit)
{
    struct LitTab *i;
    if (p_end != NULL)
    {
        for (i = p_end->literal; i != NULL; i = i->next)
        {
            if (strcmp(i->literal, lit) == 0)
                return 1;
        }
    }
    return 0;
}

void giveAddToLits()
{
    struct LitTab *i;
    if (p_end != NULL)
    {
        for (i = p_end->literal; i != NULL; i = i->next)
            i->address = LC++;
        isFirstLitInPass = 1;
    }
}

#endif