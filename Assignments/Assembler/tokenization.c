#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define VAL_CONC_LENGTH 300

int tokenizer(char *, char *[]);
char *removeChar(char *, char *);
bool isStrInArr(char *, char *[]);

// VALIDITY FUNCTION
bool valFourTokens(char *[], char *[]);

// GLOBAL KAYWORDS ARRAYS
char *mnem[] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT", NULL};
char *registers[] = {"AREG", "BREG", "CREG", "DREG", NULL};
char *cc[] = {"LT", "LE", "EQ", "GT", "GE", "ANY", NULL};
char *ad[] = {"START", "END", "ORIGIN", "EQU", "LTORG", NULL};
char *ds[] = {"DS", "DC", NULL};
//char *numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL};
//char *punctuations[] = {"!", "#", "$", "%", "&", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "]", "^", "_", "{", "|", "~", "}", NULL};

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char *line, *tokens[5], *valConc[10];
	int tokenCount, lineNo = 1;

	if (argc != 2)
	{
		printf("Please enter source program filename.");
		return 0;
	}

	fp = fopen(argv[1], "r+");

	if (fp == NULL)
	{
		printf("Failed to open '%s' file.", argv[1]);
		return 0;
	}

	line = (char *)malloc(sizeof(char) * 80);

	while (fgets(line, 80, fp) != NULL)
	{
		line[strcspn(line, "\n\r")] = 0;

		if (strstr(line, ",") != NULL)
		{
			line = removeChar(line, ",");
			tokenCount = tokenizer(line, tokens);
		}
		tokenCount = tokenizer(line, tokens);
	
		switch (tokenCount)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			if (valFourTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;
				}
			}
			break;
		default:
			printf("%s ", tokens[0]);
			printf("Invalid statement ");
		}

		lineNo++;
	}
}

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
bool isStrInArr(char *str, char *arr[])
{
	for (int i = 0; arr[i] != NULL; i++)
	{
		if (strcmp(str, arr[i]) == 0)
			return true;
	}

	return false;
}

// function to check for validity of line with 4 tokens
// parameters:
//     tokens : array of strings(tokens)
//     valConc : char pointer to return validation conclusion
// returns false if validation fails else return true
//
bool valFourTokens(char *tokens[], char *valConc[10])
{
	char **arrs[5] = {mnem, registers, ad, ds, cc};
	int temp = 0;

		printf("%s", tokens[3]);
	// return true;

	// checking for label validity
	valConc[temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	for (int i = 0; i < 5; i++)
	{
		if (isStrInArr(tokens[0], arrs[i]) || !isalpha(tokens[0][0]))
		{
			strcpy(valConc[temp], "error: Invalid symbolic name, ");
			strcat(valConc[temp], tokens[0]);
			strcat(valConc[temp], "\nnote: Reserved word, punctuations or numbers can't be use as a label");
			valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			break;
		}
	}

	// checking for mnemonic validation
	char *tempArr[] = {"ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", NULL};
	for (int i = 0; i < 5; i++)
	{
		if (!isStrInArr(tokens[1], tempArr))
		{
			strcpy(valConc[temp], "error: Invalid Mnemonic instruction, ");
			strcat(valConc[temp], tokens[1]);
			strcat(valConc[temp], "\nnote: check if you have entered correct opcode.");
			valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			break;
		}
	}

	// checking for operand 1
	if (isStrInArr(tokens[2], registers) == false)
	{

		strcpy(valConc[temp], "error: Invalid register operand, ");
		strcat(valConc[temp], tokens[2]);
		strcat(valConc[temp], "\nnote: Operand 1 can only be a register or condition code in case of BC.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	// checking for operand 2
	tokens[3][strcspn(tokens[3], "\n")] = 0;
	char **arr[] = {mnem, ad, ds, registers};

	
	for (int i = 0; i < 4; i++)
	{

		if (isStrInArr(tokens[3], arr[i]))
		{
			strcpy(valConc[temp], "error: Invalid symbolic name, ");
			strcat(valConc[temp], tokens[3]);
			strcat(valConc[temp], "\nnote: Check operand 2");
			valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			break;
		}
	}

	valConc[temp] = NULL;
	if (temp > 0)
		return false;
	else
		return true;
}
