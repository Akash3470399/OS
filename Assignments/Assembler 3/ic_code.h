#include <stdio.h>

#ifndef UTILITYFUNC_H
#define UTILITYFUNC_H
#endif

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#endif

char  *createIcCodeVar1(char *mn, char *op1, char *op2)
{
    int i = 0, index = -1, is_data = 0;
    char *code = (char *)malloc(sizeof(char)*30), temp[10];
    struct LitTab *l_temp;
    struct SymTab *s_temp;

    if (isStrInArr(mn, mnem) != -1)  // checking if mnemonics if of imperative statement
    {
        index = isStrInArr(mn, mnem);
        sprintf(code, "<IS, %d> ", index);
    } 
    else if (isStrInArr(mn, ad) != -1) // checking for assembler directive
    {
        index = isStrInArr(mn, ad);
        sprintf(code, "<AD, %d> ", index+1);
    }
    else if (isStrInArr(mn, ds) != -1)
    {
        index = isStrInArr(mn, ds);
        sprintf(code, "<DL, %d> ", index + 1);
    }
    
    
    if(op1 != NULL)
    {
        if (isStrInArr(op1, registers) != -1)
        {
            index = isStrInArr(mn, registers);
            sprintf(temp, " %d ", index+1);
            strcat(code, temp);
        }
        else if(isStrInArr(op1, cc) != -1)
        {
            index = isStrInArr(op1, cc);
            sprintf(temp, " %d ", index + 1);
            strcat(code, temp);
        }
        else
            is_data = 1;
    }

    if ((is_data == 1 || op2 != NULL) && (strcmp(mn, "START")))
    {
        (is_data)?strcpy(temp, op1):strcpy(temp, op2);

        if (isValidLiteral(temp))
        {
            l_temp = getLitAdd(temp);
            sprintf(temp, "<L, %d>", l_temp->no);
            strcat(code, temp);
        }
        else
        {
            s_temp = getSymbolAdd(temp);
            sprintf(temp, "<S, %d>", s_temp->no);
            strcat(code, temp);
        }
    }

    return code;
}

char  *createIcCodeVar2(char *mn, char *op1, char *op2)
{
    int i = 0, index = -1, is_data = 0;
    char *code = (char *)malloc(sizeof(char)*30), temp[10];
    struct LitTab *l_temp;
    struct SymTab *s_temp;

    if (isStrInArr(mn, mnem) != -1)  // checking if mnemonics if of imperative statement
    {
        index = isStrInArr(mn, mnem);
        sprintf(code, "<IS, %d> ", index);
    } 
    else if (isStrInArr(mn, ad) != -1) // checking for assembler directive
    {
        index = isStrInArr(mn, ad);
        sprintf(code, "<AD, %d> ", index+1);
    }
    else if (isStrInArr(mn, ds) != -1)
    {
        index = isStrInArr(mn, ds);
        sprintf(code, "<DL, %d> ", index + 1);
    }
    
    
    if(op1 != NULL)
    {
        if (isStrInArr(op1, registers) != -1)
            strcat(code, op1);
        else if(isStrInArr(op1, cc) != -1)
            strcat(code, op1);
        else
            is_data = 1;
    }

    if ((is_data == 1 || op2 != NULL) && (strcmp(mn, "START")))
    {
        (is_data)?strcpy(temp, op1):strcpy(temp, op2);

        if (isValidLiteral(temp))
        {
            l_temp = getLitAdd(temp);
            sprintf(temp, " <L, %d>", l_temp->no);
            strcat(code, temp);
        }
        else
         {
             strcat(code, " ");
             strcat(code, temp);
         }       
    }
    return code;
}